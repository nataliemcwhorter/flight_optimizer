//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PathFinder.h"

#include <iostream>

/**
 * @brief Constructor for PathFinder.
 * Store reference to FlightGraph for use in pathfinding algorithm.
 */
PathFinder::PathFinder(FlightGraph& graph)
    : graph(graph) {
}

/**
 * @brief Checks if a FlightOffer meets all UserQuery constraints.
 * @param offer The FlightOffer to validate
 * @param query The UserQuery with constraints
 * @return true if all constraints are satisfied, false otherwise
 */
bool PathFinder::meetsConstraints(const FlightOffer& offer, const UserQuery& query) const {
    for (const Flight& flight : offer.legs) {
        if (!flight.isAvailable(query.seatClass)) return false;
        }
    if (offer.getStopCount() > query.maxStops) return false;
    if (query.budgetCap > 0) {
        if (offer.getTotalPrice() > query.budgetCap) return false;
    }
    if (query.maxDurationMinutes > 0) {
        if (offer.getTotalDurationMinutes() > query.maxDurationMinutes && query.maxDurationMinutes != 0) return false;
    }
    return true;
}

/**
 * @brief Finds all valid flight paths matching the given query using Dijkstra's algorithm.
 * @param query The user's flight search query
 * @return Vector of FlightOffers satisfying all query constraints
 */
std::vector<FlightOffer> PathFinder::findPaths(const UserQuery& query) {
    std::vector<FlightOffer> results;
    std::function<bool(const FlightOffer&, const FlightOffer&)> cmp = [](const FlightOffer& a, const FlightOffer& b) { return a.getTotalPrice() < b.getTotalPrice();};
    FlightHeap heap(cmp);

    std::vector<Flight> initialFlights = graph.getFlightsFrom(query.origin);
    for (Flight flight : initialFlights) {
        // Simulate flight times based on query date if not set
        if (flight.departureTime == 0) {
            flight.departureTime = query.departureDate;
            // Estimate flight duration: ~800 km/h average speed
            double distance = flight.origin.distanceTo(flight.destination);
            long long durationSeconds = static_cast<long long>((distance / 800.0) * 3600);
            flight.arrivalTime = flight.departureTime + durationSeconds;
        }

        FlightOffer offer;
        offer.legs = {flight};
        offer.selectedClass = query.seatClass;
        heap.push(offer);
    }

    while (!heap.isEmpty()) {
        FlightOffer offer = heap.pop();

        // Check if we reached destination
        if (offer.legs.back().destination.iataCode == query.destination) {
            if (meetsConstraints(offer, query)) {
                results.push_back(offer);
            }
            continue;
        }

        // Explore connections if we haven't exceeded max stops
        if (offer.getStopCount() < query.maxStops) {
            std::vector<Flight> connections = graph.getFlightsFrom(offer.legs.back().destination.iataCode);
            for (Flight flight : connections) {
                // Simulate flight times for connections if not set
                if (flight.departureTime == 0) {
                    // Set departure 2 hours after previous arrival (layover time)
                    flight.departureTime = offer.legs.back().arrivalTime + (2 * 3600);
                    // Estimate flight duration
                    double distance = flight.origin.distanceTo(flight.destination);
                    long long durationSeconds = static_cast<long long>((distance / 800.0) * 3600);
                    flight.arrivalTime = flight.departureTime + durationSeconds;
                }

                // Check minimum connection time (45 minutes)
                long long minConnection = 45 * 60;
                if (flight.departureTime < offer.legs.back().arrivalTime + minConnection) {
                    continue;
                }

                FlightOffer connectOffer;
                connectOffer.legs = offer.legs;
                connectOffer.legs.push_back(flight);
                connectOffer.selectedClass = query.seatClass;
                heap.push(connectOffer);
            }
        }
    }
    return results;
}