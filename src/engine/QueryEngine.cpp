//
// Created by Natalie McWhorter on 4/14/26.
//

#include "QueryEngine.h"

/**
 * Constructor that initializes the QueryEngine with a reference to the flight graph.
 * @param graph Reference to the FlightGraph containing all airports and flights.
 */
QueryEngine::QueryEngine(FlightGraph& graph) : graph(graph), pathFinder(graph), rankingEngine() {}


/**
 * Maps a Priority enum value to a ranking strategy name string.
 * @param p The Priority enum value.
 * @return Strategy name string for the RankingEngine.
 */
std::string QueryEngine::priorityToStrategy(Priority p) const {
	// TODO: Implement switch statement on p:
	//   - Priority::CHEAPEST → return "cheapest"
	//   - Priority::FASTEST → return "fastest"
	//   - Priority::FEWEST_STOPS → return "fewest_stops"
	//   - Priority::BEST_VALUE → return "best_value"
	//   - default → return "cheapest"
	return "";
}

/**
 * Executes a flight query by finding candidate paths and ranking them by user priority.
 * @param q The UserQuery containing origin, destination, dates, seat class, priority, and constraints.
 * @return A vector of FlightOffers sorted according to the user's priority strategy.
 */
std::vector<FlightOffer> QueryEngine::query(const UserQuery& q) {
	// TODO Step 1: Call pathFinder.findPaths(q) and store result in a variable named 'candidates'
	// TODO Step 2: Call priorityToStrategy(q.priority) and store result in a variable named 'strategyName'
	// TODO Step 3: Return rankingEngine.rank(candidates, strategyName)
	return std::vector<FlightOffer>();
}
