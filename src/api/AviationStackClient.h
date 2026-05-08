//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_AMADEUSCLIENT_H
#define FLIGHT_OPTIMIZER_AMADEUSCLIENT_H

#include <string>
#include <vector>
#include <optional>
#include "../models/Flight.h"
#include "../snapshot/SnapshotManager.h"

/**
 * Client for the AviationStack REST API (http://api.aviationstack.com/v1).
 * Provides live flight status, schedules, and airline data.
 *
 * Free tier: 100 calls/month — use sparingly. All responses are cached via
 * SnapshotManager to avoid wasting quota. Use ONLY for presentation demo enrichment.
 */
class AviationStackClient {
private:
    /// API key for AviationStack authentication
    std::string apiKey;

    /// Reference to SnapshotManager for caching responses
    SnapshotManager& snapshots;

    /// Base URL for AviationStack API
    static const std::string BASE_URL;

    /**
     * Performs HTTP GET request to the given URL.
     * @param url Full URL to request
     * @return Raw JSON response as string
     */
    std::string httpGet(const std::string& url) const;

    /**
     * Parses AviationStack flights JSON into Flight objects.
     * @param json Raw JSON string from AviationStack endpoint
     * @return Vector of parsed Flight objects
     */
    std::vector<Flight> parseFlights(const std::string& json) const;

    /**
     * Builds a cache key for snapshots storage.
     * @param endpoint API endpoint name (e.g., "flights", "status", "schedule")
     * @param params Query parameters concatenated (e.g., "JFK_LAX")
     * @return Cache key string suitable for SnapshotManager
     */
    std::string makeCacheKey(const std::string& endpoint, const std::string& params) const;

    /**
     * Parses ISO 8601 timestamp to Unix time (seconds since epoch).
     * @param iso8601 ISO 8601 formatted string (e.g., "2024-05-07T14:30:00+00:00")
     * @return Unix timestamp in seconds
     */
    static long long parseISO8601(const std::string& iso8601);

public:
    /**
     * Constructor.
     * @param apiKey AviationStack API key
     * @param snapshots Reference to SnapshotManager for caching
     */
    AviationStackClient(const std::string& apiKey, SnapshotManager& snapshots);

    /**
     * Fetch live flights between two airports.
     * Endpoint: /flights?dep_iata=<depIata>&arr_iata=<arrIata>
     * Checks snapshots cache first before making API call.
     * @param depIata Departure airport IATA code
     * @param arrIata Arrival airport IATA code
     * @return Vector of Flight objects between the airports
     */
    std::vector<Flight> getLiveFlights(const std::string& depIata,
                                       const std::string& arrIata) const;

    /**
     * Fetch current status of a specific flight by IATA flight number.
     * Checks snapshots cache first before making API call.
     * @param flightNumber IATA flight number (e.g., "BA747")
     * @return Flight object if found, std::nullopt otherwise
     */
    std::optional<Flight> getFlightStatus(const std::string& flightNumber) const;

    /**
     * Fetch today's scheduled departures from an airport.
     * Endpoint: /flights?dep_iata=<airportIata>
     * Checks snapshots cache first before making API call.
     * @param airportIata Departure airport IATA code
     * @return Vector of Flight objects scheduled to depart from airport
     */
    std::vector<Flight> getScheduledFlights(const std::string& airportIata) const;
};

#endif //FLIGHT_OPTIMIZER_AMADEUSCLIENT_H