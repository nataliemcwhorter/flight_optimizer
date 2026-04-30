//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_QUERYENGINE_H
#define FLIGHT_OPTIMIZER_QUERYENGINE_H


#include <vector>
#include <string>
#include "../models/UserQuery.h"
#include "../models/FlightOffer.h"
#include "PathFinder.h"
#include "RankingEngine.h"
#include "../structures/FlightGraph.h"

/**
 * Top-level orchestrator for flight queries.
 * Accepts a UserQuery, delegates path-finding to PathFinder, ranking to RankingEngine,
 * and returns a sorted vector of FlightOffers matching the user's criteria and priority.
 */
class QueryEngine {
private:
	FlightGraph& graph;
	PathFinder pathFinder;
	RankingEngine rankingEngine;

	/**
	 * Maps a Priority enum value to a ranking strategy name string.
	 * Used internally to configure the RankingEngine.
	 * @param p The Priority enum value (CHEAPEST, FASTEST, FEWEST_STOPS, BEST_VALUE).
	 * @return Strategy name string: "cheapest", "fastest", "fewest_stops", or "best_value".
	 */
	std::string priorityToStrategy(Priority p) const;

public:
	/**
	 * Constructor that initializes the QueryEngine with a reference to the flight graph.
	 * @param graph Reference to the FlightGraph containing all airports and flights.
	 */
	explicit QueryEngine(FlightGraph& graph);

	/**
	 * Executes a flight query by finding candidate paths and ranking them by user priority.
	 * @param q The UserQuery containing origin, destination, dates, seat class, priority, and constraints.
	 * @return A vector of FlightOffers sorted according to the user's priority strategy.
	 */
	std::vector<FlightOffer> query(const UserQuery& q);
};


#endif //FLIGHT_OPTIMIZER_QUERYENGINE_H