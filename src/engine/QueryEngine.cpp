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


using namespace std;
std::string QueryEngine::priorityToStrategy(Priority p) const {
	switch (p) {
		case Priority::CHEAPEST:
			return "cheapest";
		case Priority::FASTEST:
			return "fastest";
		case Priority::FEWEST_STOPS:
			return "fewest_stops";
		case Priority::BEST_VALUE:
			return "best_value";
	}
	return "";
}

/**
 * Executes a flight query by finding candidate paths and ranking them by user priority.
 * @param q The UserQuery containing origin, destination, dates, seat class, priority, and constraints.
 * @return A vector of FlightOffers sorted according to the user's priority strategy.
 */
std::vector<FlightOffer> QueryEngine::query(const UserQuery& q) {
	vector<FlightOffer> candidates = pathFinder.findPaths(q);
	string strategyName =priorityToStrategy(q.priority);
	return rankingEngine.rank(candidates, strategyName);
}
