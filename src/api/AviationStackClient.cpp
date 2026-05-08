
//
// Created by Natalie McWhorter on 4/14/26.
//

#include "AviationStackClient.h"
#include <curl/curl.h>
#include "nlohmann/json.hpp"
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

const std::string AviationStackClient::BASE_URL = "http://api.aviationstack.com/v1";

static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

AviationStackClient::AviationStackClient(const std::string& apiKey, SnapshotManager& snapshots)
    : apiKey(apiKey), snapshots(snapshots) {
}

std::string AviationStackClient::makeCacheKey(const std::string& endpoint,
                                               const std::string& params) const {
    return endpoint + "_" + params;
}

std::string AviationStackClient::httpGet(const std::string& url) const {
    std::string responseBuffer;

    CURL* curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl init failed");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(res));

    return responseBuffer;
}

std::vector<Flight> AviationStackClient::parseFlights(const std::string& jsonStr) const {
    std::vector<Flight> flights;

    auto data = nlohmann::json::parse(jsonStr);
    if (!data.contains("data") || data["data"].is_null())
        return flights;

    for (const auto& entry : data["data"]) {
        Flight f;

        // Flight number
        if (entry.contains("flight") && entry["flight"].contains("iata"))
            f.flightNumber = entry["flight"]["iata"].is_null() ? "" : entry["flight"]["iata"].get<std::string>();

        // Airline code
        if (entry.contains("airline") && entry["airline"].contains("iata"))
            f.airlineCode = entry["airline"]["iata"].is_null() ? "" : entry["airline"]["iata"].get<std::string>();

        // Origin
        if (entry.contains("departure") && entry["departure"].contains("iata"))
            f.origin.iataCode = entry["departure"]["iata"].is_null() ? "" : entry["departure"]["iata"].get<std::string>();

        // Destination
        if (entry.contains("arrival") && entry["arrival"].contains("iata"))
            f.destination.iataCode = entry["arrival"]["iata"].is_null() ? "" : entry["arrival"]["iata"].get<std::string>();

        // Live status
        if (entry.contains("flight_status"))
            f.liveStatus = entry["flight_status"].is_null() ? "" : entry["flight_status"].get<std::string>();

        // Timestamps — parse ISO 8601 to Unix time
        if (entry.contains("departure") && entry["departure"].contains("scheduled")) {
            std::string depTime = entry["departure"]["scheduled"].is_null() ? "" : entry["departure"]["scheduled"].get<std::string>();
            if (!depTime.empty()) f.departureTime = parseISO8601(depTime);
        }
        if (entry.contains("arrival") && entry["arrival"].contains("scheduled")) {
            std::string arrTime = entry["arrival"]["scheduled"].is_null() ? "" : entry["arrival"]["scheduled"].get<std::string>();
            if (!arrTime.empty()) f.arrivalTime = parseISO8601(arrTime);
        }
        f.popularityScore = 1.0;
        f.availableClasses = {SeatClass::ECONOMY, SeatClass::BUSINESS, SeatClass::FIRST};

        if (!f.flightNumber.empty())
            flights.push_back(f);
    }

    return flights;
}

// NEW: filter an already-parsed list by destination IATA and departure date string (YYYY-MM-DD).
// depDate may be empty, in which case only destination filtering is applied.
static std::vector<Flight> filterFlights(const std::vector<Flight>& flights,
                                          const std::string& arrIata,
                                          const std::string& depDate) {
    std::vector<Flight> result;
    for (const Flight& f : flights) {
        // Filter by destination
        if (!arrIata.empty() && f.destination.iataCode != arrIata)
            continue;

        // Filter by date if provided
        if (!depDate.empty() && f.departureTime > 0) {
            std::time_t t = static_cast<std::time_t>(f.departureTime);
            std::tm* tm_info = std::localtime(&t);
            char buf[11]; // "YYYY-MM-DD\0"
            std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm_info);
            if (std::string(buf) != depDate)
                continue;
        }

        result.push_back(f);
    }
    return result;
}

// CHANGED: Now caches ALL departures from depIata as one big snapshot keyed by
// "all_departures_<depIata>_<depDate>". On a cache hit we parse and filter locally —
// zero extra API calls. On a miss we fetch once and store everything.
std::vector<Flight> AviationStackClient::getLiveFlights(const std::string& depIata,
                                                         const std::string& arrIata,
                                                         const std::string& depDate) const {
    // Cache key covers the origin + date so different dates get their own snapshot,
    // but every destination query for the same origin+date reuses the same snapshot.
    std::string key = makeCacheKey("all_departures", depIata + "_" + depDate);

    std::string jsonData;
    if (snapshots.hasSnapshot(key)) {
        jsonData = snapshots.loadSnapshot(key);
    } else {
        // Fetch all departures from this airport (AviationStack free tier returns up to 100)
        std::string url = BASE_URL + "/flights?dep_iata=" + depIata
                                   + "&access_key=" + apiKey;
        jsonData = httpGet(url);
        snapshots.saveSnapshot(key, jsonData);
    }

    // Parse everything, then filter locally — no extra API calls
    std::vector<Flight> allFlights = parseFlights(jsonData);
    return filterFlights(allFlights, arrIata, depDate);
}

std::optional<Flight> AviationStackClient::getFlightStatus(const std::string& flightNumber) const {
    std::string key = makeCacheKey("flight_status", flightNumber);

    if (snapshots.hasSnapshot(key)) {
        auto flights = parseFlights(snapshots.loadSnapshot(key));
        return flights.empty() ? std::nullopt : std::optional<Flight>(flights[0]);
    }

    std::string url = BASE_URL + "/flights?flight_iata=" + flightNumber
                               + "&access_key=" + apiKey;

    std::string response = httpGet(url);
    snapshots.saveSnapshot(key, response);

    auto flights = parseFlights(response);
    return flights.empty() ? std::nullopt : std::optional<Flight>(flights[0]);
}

std::vector<Flight> AviationStackClient::getScheduledFlights(const std::string& airportIata) const {
    std::string key = makeCacheKey("scheduled_flights", airportIata);

    if (snapshots.hasSnapshot(key))
        return parseFlights(snapshots.loadSnapshot(key));

    std::string url = BASE_URL + "/flights?dep_iata=" + airportIata
                               + "&access_key=" + apiKey;

    std::string response = httpGet(url);
    snapshots.saveSnapshot(key, response);
    return parseFlights(response);
}

long long AviationStackClient::parseISO8601(const std::string& iso8601) {
    std::tm tm = {};
    std::istringstream ss(iso8601);

    // Parse format: "2024-05-07T14:30:00+00:00"
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    if (ss.fail()) return 0;

    return static_cast<long long>(std::mktime(&tm));
}