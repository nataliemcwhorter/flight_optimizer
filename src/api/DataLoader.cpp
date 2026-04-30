//
// Created by Natalie McWhorter on 4/14/26.
//

#include "DataLoader.h"

void DataLoader::loadAirports(const std::string& csvPath, FlightGraph& graph) {
    // TODO Step 1: Open the CSV file using std::ifstream file(csvPath)
    //             Check if file.is_open() is false; if so, throw:
    //             std::runtime_error("Could not open: " + csvPath)

    // TODO Step 2: Read and discard the header line using:
    //             std::string header;
    //             std::getline(file, header);

    // TODO Step 3: Loop through remaining lines:
    //             std::string line;
    //             while (std::getline(file, line)) {
    //               Airport a = parseAirportRow(line);
    //               graph.addAirport(a);
    //             }
}

void DataLoader::loadRoutes(const std::string& csvPath, FlightGraph& graph) {
    // TODO Step 1: Open the CSV file using std::ifstream; if fails, throw
    //             std::runtime_error("Could not open: " + csvPath)

    // TODO Step 2: Read and discard the header line

    // TODO Step 3: Loop through remaining lines:
    //             std::string line;
    //             while (std::getline(file, line)) {
    //               Flight f = parseRouteRow(line);
    //               graph.addFlight(f);
    //             }
}

void DataLoader::loadAirlines(const std::string& csvPath, std::unordered_map<std::string, Airline>& airlines) {
    // TODO Step 1: Open the CSV file using std::ifstream; if fails, throw
    //             std::runtime_error("Could not open: " + csvPath)

    // TODO Step 2: Read and discard the header line

    // TODO Step 3: Loop through remaining lines:
    //             std::string line;
    //             while (std::getline(file, line)) {
    //               Airline a = parseAirlineRow(line);
    //               airlines[a.iataCode] = a;
    //             }
}

Airport DataLoader::parseAirportRow(const std::string& row) {
    // TODO: Create std::stringstream ss(row) and std::string token

    // TODO: Parse CSV columns using std::getline(ss, token, ',') in sequence:
    //       col0 = id (use std::stoi to convert to numericId)
    //       col1 = name
    //       col2 = city
    //       col3 = country
    //       col4 = iataCode
    //       col5 = icaoCode
    //       col6 = latitude (use std::stod to convert to double)
    //       col7 = longitude (use std::stod to convert to double)
    //       (Skip remaining columns if present)

    // TODO: Validate iataCode — if it equals "" or "\\N", return an empty Airport struct
    //       (The caller should skip invalid airports)

    // TODO: Create and return an Airport struct with all parsed fields populated:
    //       Airport a;
    //       a.numericId = numericId;
    //       a.name = name;
    //       a.city = city;
    //       a.country = country;
    //       a.iataCode = iataCode;
    //       a.icaoCode = icaoCode;
    //       a.latitude = latitude;
    //       a.longitude = longitude;
    //       return a;
}

Flight DataLoader::parseRouteRow(const std::string& row) {
    // TODO: Create std::stringstream ss(row) and std::string token

    // TODO: Parse OpenFlights routes CSV columns using std::getline(ss, token, ','):
    //       col0 = airlineCode (airline's IATA code)
    //       col1 = airlineId (numeric ID, skip for now)
    //       col2 = sourceIata (origin airport IATA code)
    //       col3 = sourceId (numeric ID, skip for now)
    //       col4 = destIata (destination airport IATA code)
    //       col5 = destId (numeric ID, skip for now)
    //       col6 = codeshare (optional code-share indicator, skip)
    //       col7 = stops (number of stops, skip)
    //       col8 = equipment (aircraft type, skip)

    // TODO: Create a Flight struct and populate:
    //       flightNumber = airlineCode + sourceIata + "→" + destIata
    //       origin.iataCode = sourceIata
    //       destination.iataCode = destIata
    //       airlineCode = airlineCode (the airline's IATA code)
    //       departureTime = 0 (will be set by real-time data later)
    //       arrivalTime = 0 (will be set by real-time data later)
    //       popularityScore = 1.0 (default, updated dynamically)

    // TODO: Note — full Airport objects (with coordinates, etc.) will be resolved later
    //       by an AirportIndex component. For now, only iataCode is set.

    // TODO: Return the populated Flight struct
}

Airline DataLoader::parseAirlineRow(const std::string& row) {
    // TODO: Create std::stringstream ss(row) and std::string token

    // TODO: Parse OpenFlights airlines CSV columns using std::getline(ss, token, ','):
    //       col0 = id (numeric airline ID, skip for now)
    //       col1 = name (airline name)
    //       col2 = alias (alternate name, skip for now)
    //       col3 = iataCode (airline's IATA code, e.g., "AA")
    //       col4 = icaoCode (airline's ICAO code, e.g., "AAL")
    //       col5 = callsign (airline's radio callsign, skip for now)
    //       col6 = country (airline's home country)
    //       col7 = active (status: "Y" for active, "N" for inactive)

    // TODO: Create an Airline struct and populate:
    //       iataCode = iataCode
    //       icaoCode = icaoCode
    //       name = name
    //       country = country
    //       isActive = (active == "Y") — convert string to boolean
    //       baseRatePerKm = 0.10 (default rate, may be overridden)
    //       (classMultipliers, demandSensitivity, popularityWeight initialized to defaults)

    // TODO: Validate iataCode — if it equals "" or "\\N", return an empty Airline struct
    //       (The caller should skip invalid airlines)

    // TODO: Return the populated Airline struct
}
