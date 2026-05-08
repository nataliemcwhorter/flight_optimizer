//
// Created by Natalie McWhorter on 4/14/26.
//

#include "OpenSkyClient.h"
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <curl/curl.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

const std::string OpenSkyClient::BASE_URL = "https://opensky-network.org/api";

OpenSkyClient::OpenSkyClient(FlightGraph& graph)
    : graph(graph) {
}

std::string OpenSkyClient::httpGet(const std::string& url) const {
    std::string responseBuffer;

    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl init failed");

    static auto writeCallback = [](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    };

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t(*)(void*, size_t, size_t, void*)>(writeCallback));
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(res));

    return responseBuffer;
}

std::vector<Flight> OpenSkyClient::parseStateVectors(const std::string& jsonStr) const {
    std::vector<Flight> flights;

    json parsed = json::parse(jsonStr);
    if (!parsed.contains("states") || parsed["states"].is_null())
        return flights;

    for (const auto& state : parsed["states"]) {
        if (!state.is_array() || state.size() < 9)
            continue;

        std::string callsign = state[1].is_null() ? "" : state[1].get<std::string>();
        bool on_ground       = state[8].is_null() ? true : state[8].get<bool>();

        while (!callsign.empty() && callsign.back() == ' ')
            callsign.pop_back();

        if (callsign.empty())
            continue;

        Flight f;
        f.flightNumber = callsign;
        f.liveStatus   = on_ground ? "ON_GROUND" : "IN_FLIGHT";
        f.availableClasses = {SeatClass::ECONOMY, SeatClass::BUSINESS, SeatClass::FIRST};
        flights.push_back(f);
    }

    return flights;
}

std::vector<Flight> OpenSkyClient::getFlightsInBoundingBox(double minLat, double maxLat,
                                                           double minLon, double maxLon) const {
    std::string url = BASE_URL + "/states/all?lamin=" + std::to_string(minLat)
                               + "&lamax="            + std::to_string(maxLat)
                               + "&lomin="            + std::to_string(minLon)
                               + "&lomax="            + std::to_string(maxLon);
    return parseStateVectors(httpGet(url));
}

std::optional<Flight> OpenSkyClient::getFlightByCallsign(const std::string& callsign) const {
    std::string url = BASE_URL + "/states/all?callsign=" + callsign;

    json parsed = json::parse(httpGet(url));
    if (!parsed.contains("states") || parsed["states"].is_null() || parsed["states"].empty())
        return std::nullopt;

    const auto& state = parsed["states"][0];
    if (!state.is_array() || state.size() < 9)
        return std::nullopt;

    std::string cs = state[1].is_null() ? "" : state[1].get<std::string>();
    bool on_ground = state[8].is_null() ? true : state[8].get<bool>();

    while (!cs.empty() && cs.back() == ' ')
        cs.pop_back();

    Flight f;
    f.flightNumber = cs.empty() ? callsign : cs;
    f.liveStatus   = on_ground ? "ON_GROUND" : "IN_FLIGHT";
    return f;
}

std::vector<Flight> OpenSkyClient::getFlightsForAirport(const std::string& iataCode) const {
    std::time_t now        = std::time(nullptr);
    std::time_t begin_time = now - 3600;
    std::time_t end_time   = now + 3600;

    std::string url = BASE_URL + "/flights/departure?airport=" + iataCode
                               + "&begin=" + std::to_string(begin_time)
                               + "&end="   + std::to_string(end_time);

    json parsed = json::parse(httpGet(url));
    std::vector<Flight> flights;

    if (!parsed.is_array())
        return flights;

    for (const auto& entry : parsed) {
        std::string callsign = entry.value("callsign", "");
        while (!callsign.empty() && callsign.back() == ' ')
            callsign.pop_back();

        std::string depIcao = entry.value("estDepartureAirport", "");
        std::string arrIcao = entry.value("estArrivalAirport",   "");

        // Resolve ICAO -> IATA using the loaded airport graph
        std::string depIata = depIcao;
        std::string arrIata = arrIcao;
        if (auto airport = graph.getAirportByIcao(depIcao); airport.has_value())
            depIata = airport->iataCode;
        if (auto airport = graph.getAirportByIcao(arrIcao); airport.has_value())
            arrIata = airport->iataCode;

        Flight f;
        f.flightNumber         = callsign;
        f.airlineCode          = callsign.size() >= 2 ? callsign.substr(0, 2) : "";
        f.origin.iataCode      = depIata;
        f.destination.iataCode = arrIata;
        f.departureTime        = entry.value("firstSeen", 0LL);
        f.arrivalTime          = entry.value("lastSeen",  0LL);
        f.liveStatus           = "SCHEDULED";
        f.popularityScore      = 1.0;

        flights.push_back(f);
    }

    return flights;
}