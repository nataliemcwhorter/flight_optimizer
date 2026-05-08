//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "../src/structures/FlightGraph.h"
#include "../src/engine/PathFinder.h"
#include "../src/models/Flight.h"
#include "../src/models/Airport.h"
#include "../src/models/UserQuery.h"

// Builds a synthetic grid network of airports connected in a chain
// A0 -> A1 -> A2 -> ... -> A(n-1), plus some cross-links for variety
FlightGraph buildGraph(int numAirports, long long baseTime) {
    FlightGraph graph;

    for (int i = 0; i < numAirports; i++) {
        Airport a;
        a.iataCode  = "A" + std::to_string(i);
        a.latitude  = 30.0 + i * 0.5;
        a.longitude = -120.0 + i * 0.5;
        graph.addAirport(a);
    }

    // Chain: each airport connects to the next
    for (int i = 0; i < numAirports - 1; i++) {
        Flight f;
        f.flightNumber         = "FL" + std::to_string(i);
        f.airlineCode          = "AA";
        f.origin.iataCode      = "A" + std::to_string(i);
        f.destination.iataCode = "A" + std::to_string(i + 1);
        f.departureTime        = baseTime + i * 7200;       // 2h apart
        f.arrivalTime          = baseTime + i * 7200 + 3600; // 1h flight
        f.popularityScore      = 1.0;
        f.availableClasses     = {SeatClass::ECONOMY, SeatClass::BUSINESS};
        f.pricePerClass[SeatClass::ECONOMY]  = 100.0 + i * 10.0;
        f.pricePerClass[SeatClass::BUSINESS] = 300.0 + i * 20.0;
        graph.addFlight(f);
    }

    // Cross-links: every 3 airports, add a skip connection
    for (int i = 0; i < numAirports - 3; i += 3) {
        Flight f;
        f.flightNumber         = "XL" + std::to_string(i);
        f.airlineCode          = "AA";
        f.origin.iataCode      = "A" + std::to_string(i);
        f.destination.iataCode = "A" + std::to_string(i + 3);
        f.departureTime        = baseTime + i * 7200;
        f.arrivalTime          = baseTime + i * 7200 + 7200;
        f.popularityScore      = 1.2;
        f.availableClasses     = {SeatClass::ECONOMY};
        f.pricePerClass[SeatClass::ECONOMY] = 250.0 + i * 5.0;
        graph.addFlight(f);
    }

    return graph;
}

int maind() {
    const int NUM_AIRPORTS = 20;
    const long long BASE_TIME = 1700000000LL;

    std::cout << "=== PathFinder Benchmark ===\n\n";

    FlightGraph graph = buildGraph(NUM_AIRPORTS, BASE_TIME);

    // ============ Section 1: Direct route (0 stops) ============
    std::cout << "Section 1: Direct route A0 -> A1 (0 stops)\n";

    UserQuery directQuery;
    directQuery.origin             = "A0";
    directQuery.destination        = "A1";
    directQuery.seatClass          = SeatClass::ECONOMY;
    directQuery.maxStops           = 0;
    directQuery.budgetCap          = 0.0;
    directQuery.maxDurationMinutes = 0;
    directQuery.priority           = Priority::CHEAPEST;

    PathFinder finder(graph);

    auto start = std::chrono::high_resolution_clock::now();
    auto results = finder.findPaths(directQuery);
    auto end = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Results found: " << results.size() << "\n";
    std::cout << "Time: " << directTime << " microseconds\n\n";

    // ============ Section 2: Multi-hop route (up to 2 stops) ============
    std::cout << "Section 2: Multi-hop A0 -> A3 (up to 2 stops)\n";

    UserQuery multiQuery;
    multiQuery.origin             = "A0";
    multiQuery.destination        = "A3";
    multiQuery.seatClass          = SeatClass::ECONOMY;
    multiQuery.maxStops           = 2;
    multiQuery.budgetCap          = 0.0;
    multiQuery.maxDurationMinutes = 0;
    multiQuery.priority           = Priority::CHEAPEST;

    start = std::chrono::high_resolution_clock::now();
    auto multiResults = finder.findPaths(multiQuery);
    end = std::chrono::high_resolution_clock::now();
    auto multiTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Results found: " << multiResults.size() << "\n";
    std::cout << "Time: " << multiTime << " microseconds\n\n";

    // ============ Section 3: Long route with budget cap ============
    std::cout << "Section 3: Long route A0 -> A6 (up to 3 stops, $500 budget)\n";

    UserQuery budgetQuery;
    budgetQuery.origin             = "A0";
    budgetQuery.destination        = "A6";
    budgetQuery.seatClass          = SeatClass::ECONOMY;
    budgetQuery.maxStops           = 3;
    budgetQuery.budgetCap          = 500.0;
    budgetQuery.maxDurationMinutes = 0;
    budgetQuery.priority           = Priority::CHEAPEST;

    start = std::chrono::high_resolution_clock::now();
    auto budgetResults = finder.findPaths(budgetQuery);
    end = std::chrono::high_resolution_clock::now();
    auto budgetTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Results found: " << budgetResults.size() << "\n";
    std::cout << "Time: " << budgetTime << " microseconds\n\n";

    // ============ Section 4: Summary ============
    std::cout << "=== Summary ===\n";
    std::cout << "Direct route (0 stops):            " << directTime << " us, "
              << results.size() << " result(s)\n";
    std::cout << "Multi-hop (2 stops):               " << multiTime << " us, "
              << multiResults.size() << " result(s)\n";
    std::cout << "Long route w/ budget (3 stops):    " << budgetTime << " us, "
              << budgetResults.size() << " result(s)\n";

    return 0;
}