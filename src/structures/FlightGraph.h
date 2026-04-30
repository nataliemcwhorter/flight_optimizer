//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_FLIGHTGRAPH_H
#define FLIGHT_OPTIMIZER_FLIGHTGRAPH_H

#include <unordered_map>
#include <vector>
#include <optional>
#include <string>
#include "../models/Airport.h"
#include "../models/Flight.h"

/**
 * @class FlightGraph
 * @brief Adjacency list graph for the flight network.
 *
 * Nodes represent Airport objects (keyed by IATA code as string).
 * Edges represent Flight objects connecting origin to destination airports.
 * Used by PathFinder for Dijkstra traversal and route optimization.
 */
class FlightGraph {
private:
    /// Map of IATA codes to Airport objects
    std::unordered_map<std::string, Airport> airports;

    /// Adjacency list: key = origin airport IATA code, value = vector of Flight objects
    std::unordered_map<std::string, std::vector<Flight>> adjacency;

public:
    /**
     * @brief Add an airport to the graph.
     * @param airport The Airport object to add (keyed by iataCode)
     */
    void addAirport(const Airport& airport);

    /**
     * @brief Add a flight edge to the graph.
     * @param flight The Flight object to add (connects origin to destination)
     * @note Ensure addAirport() is called for both origin and destination before calling this
     */
    void addFlight(const Flight& flight);

    /**
     * @brief Retrieve all outgoing flights from an airport.
     * @param iataCode The IATA code of the origin airport
     * @return Vector of Flight objects departing from the specified airport; empty if not found
     */
    std::vector<Flight> getFlightsFrom(const std::string& iataCode) const;

    /**
     * @brief Retrieve an airport by its IATA code.
     * @param iataCode The IATA code to look up
     * @return Optional containing the Airport if found; std::nullopt otherwise
     */
    std::optional<Airport> getAirport(const std::string& iataCode) const;

    /**
     * @brief Check if an airport exists in the graph.
     * @param iataCode The IATA code to check
     * @return true if airport exists; false otherwise
     */
    bool hasAirport(const std::string& iataCode) const;

    /**
     * @brief Get the total number of airports in the graph.
     * @return Number of airports as int
     */
    int airportCount() const;

    /**
     * @brief Get the total number of flights in the graph.
     * @return Number of flight edges as int
     */
    int flightCount() const;

    /**
     * @brief Clear all airports and flights from the graph.
     */
    void clear();
};


#endif //FLIGHT_OPTIMIZER_FLIGHTGRAPH_H