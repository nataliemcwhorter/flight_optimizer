//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_AIRPORTINDEX_H
#define FLIGHT_OPTIMIZER_AIRPORTINDEX_H

#include "AirportTrie.h"
#include "FlightGraph.h"
#include <optional>
#include <string>
#include <vector>
#include "../models/Airport.h"

/**
 * Combines FlightGraph and AirportTrie into a single access point for airport lookups.
 * Trie handles prefix search; graph handles flight adjacency.
 */
class AirportIndex {
private:
	AirportTrie trie;
	FlightGraph& graph;

public:
	/**
	 * Constructs an AirportIndex wrapping the provided FlightGraph reference.
	 * @param graph Reference to the FlightGraph to be indexed
	 */
	explicit AirportIndex(FlightGraph& graph);

	/**
	 * Indexes a single airport into both the trie and the graph.
	 * @param airport The Airport to index
	 */
	void indexAirport(const Airport& airport);

	/**
	 * Looks up an airport by its IATA code.
	 * @param iataCode The IATA code to search for
	 * @return std::optional<Airport> containing the airport if found, nullopt otherwise
	 */
	std::optional<Airport> findByCode(const std::string& iataCode) const;

	/**
	 * Searches for airports matching a given prefix.
	 * @param prefix The prefix to search for (case-sensitive)
	 * @return std::vector<Airport> of all matching airports
	 */
	std::vector<Airport> searchByPrefix(const std::string& prefix) const;

	/**
	 * Checks if the index is empty.
	 * @return true if no airports are indexed, false otherwise
	 */
	bool isEmpty() const;
};

#endif //FLIGHT_OPTIMIZER_AIRPORTINDEX_H