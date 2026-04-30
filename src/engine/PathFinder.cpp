//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PathFinder.h"

/**
 * @brief Constructor for PathFinder.
 *
 * TODO: Initialize the graph reference via member initializer list.
 * Store reference to FlightGraph for use in pathfinding algorithm.
 */
PathFinder::PathFinder(FlightGraph& graph)
    : graph(graph) {
    // TODO: Member initialization complete
}

/**
 * @brief Checks if a FlightOffer meets all UserQuery constraints.
 *
 * TODO Step 1: For each leg in offer.legs, verify that flight.isAvailable(query.seatClass) returns true
 * TODO Step 2: Verify that (offer.legs.size() - 1) <= query.maxStops
 * TODO Step 3: If query.budgetCap > 0, verify that offer.getTotalPrice() <= query.budgetCap
 * TODO Step 4: If query.maxDurationMinutes > 0, verify that sum of leg.getDuration() for all legs <= query.maxDurationMinutes
 *
 * @param offer The FlightOffer to validate
 * @param query The UserQuery with constraints
 * @return true if all constraints are satisfied, false otherwise
 */
bool PathFinder::meetsConstraints(const FlightOffer& offer, const UserQuery& query) const {
    // TODO: Implement constraint checking logic
    return true;
}

/**
 * @brief Finds all valid flight paths matching the given query using Dijkstra's algorithm.
 *
 * Algorithm Overview:
 *
 * TODO Step 1: Create a FlightHeap with a price comparator (cheapest offers first).
 *             Use std::function<bool(const FlightOffer&, const FlightOffer&)> to compare offers by getTotalPrice().
 *
 * TODO Step 2: Seed the heap with initial flights.
 *             - Get all flights departing from query.origin using graph.getFlightsFrom(query.origin)
 *             - For each flight, wrap it as a single-leg FlightOffer
 *             - Push each single-leg offer onto the heap
 *
 * TODO Step 3: Execute Dijkstra's main loop:
 *             - While heap is not empty:
 *               a) Pop the best (lowest-cost) offer from the heap
 *               b) If last leg's destination == query.destination AND meetsConstraints(offer, query):
 *                  - Add offer to results vector
 *                  - Continue to next iteration (skip expansion)
 *               c) Else if (offer.legs.size() - 1) < query.maxStops:
 *                  - Get all flights departing from offer.legs.back().destination
 *                  - For each outgoing flight, create a new FlightOffer by appending this flight to offer.legs
 *                  - Push new offer onto heap
 *               d) Else: offer has maxed out stops, skip expansion
 *
 * TODO Step 4: Return the results vector containing all valid FlightOffers found
 *
 * @param query The user's flight search query
 * @return Vector of FlightOffers satisfying all query constraints
 */
std::vector<FlightOffer> PathFinder::findPaths(const UserQuery& query) {
    std::vector<FlightOffer> results;

    // TODO: Implement Dijkstra's algorithm per steps above

    return results;
}
