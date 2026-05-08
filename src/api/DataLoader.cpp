//
// Created by Natalie McWhorter on 4/14/26.
//

#include "DataLoader.h"

// Strips surrounding double-quotes from a token if present.
// OpenFlights quotes most string fields e.g. "London Heathrow Airport"
static std::string stripQuotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

// Splits a CSV line respecting double-quoted fields that may contain commas.
static std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}

void DataLoader::loadAirports(const std::string& csvPath, FlightGraph& graph) {
    std::ifstream file(csvPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open: " + csvPath);

    // OpenFlights airports.dat has NO header line
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Airport a = parseAirportRow(line);
        if (!a.iataCode.empty())
            graph.addAirport(a);
    }
}

void DataLoader::loadRoutes(const std::string& csvPath, FlightGraph& graph) {
    std::ifstream file(csvPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open: " + csvPath);

    // OpenFlights routes.dat has NO header line
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Flight f = parseRouteRow(line);
        if (!f.airlineCode.empty())
            graph.addFlight(f);
    }
}

void DataLoader::loadAirlines(const std::string& csvPath,
                               std::unordered_map<std::string, Airline>& airlines) {
    std::ifstream file(csvPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open: " + csvPath);

    // OpenFlights airlines.dat has NO header line
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Airline a = parseAirlineRow(line);
        if (!a.iataCode.empty())
            airlines[a.iataCode] = a;
    }
}

Airport DataLoader::parseAirportRow(const std::string& row) {
    // OpenFlights airports.dat — quoted fields, NO header, 14 columns:
    // 0:id, 1:name, 2:city, 3:country, 4:IATA, 5:ICAO, 6:lat, 7:lon,
    // 8:alt, 9:timezone, 10:DST, 11:tz, 12:type, 13:source
    auto cols = splitCSV(row);
    if (cols.size() < 8) return Airport{};

    std::string iataCode = stripQuotes(cols[4]);
    if (iataCode.empty() || iataCode == "\\N") return Airport{};

    Airport a;
    try { a.numericId = std::stoi(stripQuotes(cols[0])); } catch (...) { a.numericId = 0; }
    a.name      = stripQuotes(cols[1]);
    a.city      = stripQuotes(cols[2]);
    a.country   = stripQuotes(cols[3]);
    a.iataCode  = iataCode;
    a.icaoCode  = stripQuotes(cols[5]);
    try { a.latitude  = std::stod(stripQuotes(cols[6])); } catch (...) { a.latitude  = 0.0; }
    try { a.longitude = std::stod(stripQuotes(cols[7])); } catch (...) { a.longitude = 0.0; }
    return a;
}

Flight DataLoader::parseRouteRow(const std::string& row) {
    // OpenFlights routes.dat — unquoted, NO header, 9 columns:
    // 0:airline, 1:airlineId, 2:srcAirport, 3:srcAirportId,
    // 4:dstAirport, 5:dstAirportId, 6:codeshare, 7:stops, 8:equipment
    auto cols = splitCSV(row);
    if (cols.size() < 5) return Flight{};

    std::string airlineCode = cols[0];
    std::string sourceIata  = cols[2];
    std::string destIata    = cols[4];

    if (airlineCode.empty() || airlineCode == "\\N") return Flight{};
    if (sourceIata.empty()  || sourceIata  == "\\N") return Flight{};
    if (destIata.empty()    || destIata    == "\\N") return Flight{};

    Flight f;
    f.flightNumber         = airlineCode + sourceIata + "\u2192" + destIata;
    f.airlineCode          = airlineCode;
    f.origin.iataCode      = sourceIata;
    f.destination.iataCode = destIata;
    f.departureTime        = 0;
    f.arrivalTime          = 0;
    f.popularityScore      = 1.0;
    f.availableClasses     = {SeatClass::ECONOMY, SeatClass::BUSINESS, SeatClass::FIRST};
    return f;
}

Airline DataLoader::parseAirlineRow(const std::string& row) {
    // OpenFlights airlines.dat — quoted fields, NO header, 8 columns:
    // 0:id, 1:name, 2:alias, 3:IATA, 4:ICAO, 5:callsign, 6:country, 7:active
    auto cols = splitCSV(row);
    if (cols.size() < 8) return Airline{};

    std::string iataCode = stripQuotes(cols[3]);
    if (iataCode.empty() || iataCode == "\\N") return Airline{};

    Airline a;
    a.name          = stripQuotes(cols[1]);
    a.iataCode      = iataCode;
    a.icaoCode      = stripQuotes(cols[4]);
    a.country       = stripQuotes(cols[6]);
    a.isActive      = (stripQuotes(cols[7]) == "Y");
    a.baseRatePerKm = 0.10;
    return a;
}