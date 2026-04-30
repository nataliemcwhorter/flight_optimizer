//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_PATHFINDER_H
#define FLIGHT_OPTIMIZER_PATHFINDER_H

#include <optional>
#include <vector>
#include <string>
#include <unordered_map>
#include "../models/FlightOffer.h"
#include "../models/UserQuery.h"
#include "../structures/FlightGraph.h"
#include "../structures/FlightHeap.h"

/**
 * @class PathFinder
 * @brief Implements Dijkstra's algorithm over FlightGraph.
 *
 * Uses FlightHeap internally during search. Returns all valid FlightOffers
 * meeting UserQuery constraints such as seat class availability, maximum stops,
 * budget cap, and duration limits.
 */
class PathFinder {
private:
	FlightGraph& graph;

	/**
	 * @brief Checks if a FlightOffer satisfies all UserQuery constraints.
	 *
	 * Validates:
	 * - SeatClass availability on all legs
	 * - Number of stops does not exceed maxStops
	 * - Total price does not exceed budgetCap (if budgetCap > 0)
	 * - Total duration does not exceed maxDurationMinutes (if maxDurationMinutes > 0)
	 *
	 * @param offer The FlightOffer to validate
	 * @param query The UserQuery with constraints
	 * @return true if offer meets all constraints, false otherwise
	 */
	bool meetsConstraints(const FlightOffer& offer, const UserQuery& query) const;

public:
	/**
	 * @brief Constructor for PathFinder.
	 *
	 * @param graph Reference to the FlightGraph containing all airports and flights
	 */
	explicit PathFinder(FlightGraph& graph);

	/**
	 * @brief Finds all valid flight paths matching the given query.
	 *
	 * Uses Dijkstra's algorithm with a priority queue (FlightHeap) to explore
	 * flight combinations from origin to destination while respecting all constraints.
	 *
	 * @param query The user's flight search query
	 * @return Vector of FlightOffers that satisfy the query constraints
	 */
	std::vector<FlightOffer> findPaths(const UserQuery& query);
};


#endif //FLIGHT_OPTIMIZER_PATHFINDER_H