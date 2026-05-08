//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <cstdlib>
#include "../src/structures/FlightGraph.h"
#include "../src/models/Flight.h"
#include "../src/models/Airport.h"

Flight makeFlight(std::string origin, std::string dest) {
    Flight flight;
    flight.origin.iataCode      = origin;
    flight.destination.iataCode = dest;
    flight.flightNumber         = "FL" + origin + dest;
    flight.popularityScore      = 1.0;
    return flight;
}

int main() {
    const int NUM_AIRPORTS = 500;
    const int NUM_FLIGHTS  = 10000;

    std::cout << "=== FlightGraph vs Naive Linear Scan Benchmark ===\n\n";

    // ============ Section 1: FlightGraph Build ============
    std::cout << "Section 1: Building FlightGraph (" << NUM_AIRPORTS << " airports, "
              << NUM_FLIGHTS << " flights)\n";

    FlightGraph graph;
    std::vector<std::string> airportCodes;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_AIRPORTS; i++) {
        std::string code = "A" + std::to_string(i);
        Airport a;
        a.iataCode = code;
        graph.addAirport(a);
        airportCodes.push_back(code);
    }

    for (int j = 0; j < NUM_FLIGHTS; j++) {
        int originIdx = rand() % NUM_AIRPORTS;
        int destIdx   = rand() % NUM_AIRPORTS;
        Flight f = makeFlight(airportCodes[originIdx], airportCodes[destIdx]);
        graph.addFlight(f);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto buildTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Build Time: " << buildTime << " microseconds\n\n";

    // ============ Section 2: FlightGraph Lookup ============
    std::cout << "Section 2: FlightGraph lookup (1000 getFlightsFrom() calls)\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        std::string code = airportCodes[rand() % NUM_AIRPORTS];
        auto result = graph.getFlightsFrom(code);
        (void)result;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto graphLookupTime = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start).count();
    std::cout << "Lookup Time: " << graphLookupTime << " us (1000 queries)\n";
    std::cout << "Average per query: " << (graphLookupTime / 1000.0) << " us\n\n";

    // ============ Section 3: Naive Linear Scan ============
    std::cout << "Section 3: Naive linear scan (1000 searches on vector)\n";

    std::vector<Flight> allFlights;
    for (const std::string& code : airportCodes) {
        auto flights = graph.getFlightsFrom(code);
        allFlights.insert(allFlights.end(), flights.begin(), flights.end());
    }

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        std::string code = airportCodes[rand() % NUM_AIRPORTS];
        std::vector<Flight> matches;
        for (const Flight& f : allFlights) {
            if (f.origin.iataCode == code)
                matches.push_back(f);
        }
        (void)matches;
    }
    auto end3 = std::chrono::high_resolution_clock::now();
    auto naiveScanTime = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start).count();
    std::cout << "Scan Time: " << naiveScanTime << " us (1000 searches)\n";
    std::cout << "Average per search: " << (naiveScanTime / 1000.0) << " us\n\n";

    // ============ Section 4: Comparison Summary ============
    std::cout << "=== Summary ===\n";
    std::cout << "FlightGraph Build:         " << buildTime << " us\n";
    std::cout << "FlightGraph Lookup (avg):  " << (graphLookupTime / 1000.0) << " us\n";
    std::cout << "Naive Scan (avg):          " << (naiveScanTime / 1000.0) << " us\n";
    std::cout << "Speedup (naive / graph):   " << (naiveScanTime / (double)graphLookupTime) << "x\n";
    std::cout << "Graph is " << (graphLookupTime < naiveScanTime ? "FASTER" : "SLOWER") << "\n";

    return 0;
}