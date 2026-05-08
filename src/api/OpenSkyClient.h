//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_OPENSKYCLIENT_H
#define FLIGHT_OPTIMIZER_OPENSKYCLIENT_H

#include <string>
#include <vector>
#include <optional>
#include "../models/Flight.h"
#include "../models/Airport.h"
#include "../structures/FlightGraph.h"

/**
 * Client for the OpenSky Network REST API (https://opensky-network.org/api).
 * Fetches live aircraft state vectors. Free tier — no authentication required.
 * Used to enrich Flight objects with real-time position and status data.
 */
class OpenSkyClient {
private:
    /// Base URL for OpenSky Network API
    static const std::string BASE_URL;

    /// Reference to the loaded flight graph for ICAO -> IATA airport resolution
    FlightGraph& graph;

    /**
     * Performs HTTP GET request to the given URL.
     * @param url Full URL to request
     * @return Raw JSON response as string
     */
    std::string httpGet(const std::string& url) const;

    /**
     * Parses OpenSky state vector JSON into Flight objects.
     * @param jsonStr Raw JSON string from OpenSky /states/all endpoint
     * @return Vector of parsed Flight objects
     */
    std::vector<Flight> parseStateVectors(const std::string& jsonStr) const;

public:
    /**
     * Constructor.
     * @param graph Reference to the loaded FlightGraph for ICAO->IATA resolution
     */
    explicit OpenSkyClient(FlightGraph& graph);

    /**
     * Fetch all aircraft currently in a geographic bounding box.
     * Endpoint: /states/all?lamin=<minLat>&lamax=<maxLat>&lomin=<minLon>&lomax=<maxLon>
     */
    std::vector<Flight> getFlightsInBoundingBox(double minLat, double maxLat,
                                                double minLon, double maxLon) const;

    /**
     * Fetch a single aircraft's state by ICAO24 callsign.
     * @param callsign ICAO24 callsign (e.g., "DAL123")
     * @return Flight object if found, std::nullopt otherwise
     */
    std::optional<Flight> getFlightByCallsign(const std::string& callsign) const;

    /**
     * Fetch all flights currently departing from an airport.
     * Resolves ICAO airport codes in the response to IATA using the loaded FlightGraph.
     * @param iataCode IATA airport code (e.g., "RDU")
     * @return Vector of Flight objects scheduled to depart from the airport
     */
    std::vector<Flight> getFlightsForAirport(const std::string& iataCode) const;
};

#endif //FLIGHT_OPTIMIZER_OPENSKYCLIENT_H