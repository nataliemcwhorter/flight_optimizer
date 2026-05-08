//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <cstdlib>

#include "../src/engine/PricingModel.h"
#include "../src/structures/PriceCache.h"
#include "../src/models/FlightOffer.h"
#include "../src/models/Flight.h"
#include "../src/models/Airport.h"
#include "../src/models/Airline.h"

// ============================================================================
// HELPERS
// ============================================================================

Airport makeAirport(const std::string& iata, double lat, double lon) {
    Airport a;
    a.iataCode  = iata;
    a.latitude  = lat;
    a.longitude = lon;
    return a;
}

Airline makeAirline() {
    Airline a;
    a.iataCode      = "AA";
    a.name          = "American Airlines";
    a.baseRatePerKm = 0.10;
    a.isActive      = true;
    a.classMultipliers[SeatClass::ECONOMY]         = 1.0;
    a.classMultipliers[SeatClass::PREMIUM_ECONOMY] = 1.5;
    a.classMultipliers[SeatClass::BUSINESS]        = 2.5;
    a.classMultipliers[SeatClass::FIRST]           = 4.0;
    return a;
}

std::vector<Flight> makeFlights(int n) {
    Airport jfk = makeAirport("JFK", 40.6413, -73.7781);
    Airport lax = makeAirport("LAX", 33.9425, -118.4081);

    std::vector<Flight> flights;
    for (int i = 0; i < n; i++) {
        Flight f;
        f.flightNumber    = "AA" + std::to_string(i);
        f.airlineCode     = "AA";
        f.origin          = jfk;
        f.destination     = lax;
        f.departureTime   = 0;
        f.arrivalTime     = 18000;  // 5 hours
        f.popularityScore = 1.0 + (rand() % 10) / 10.0;
        f.availableClasses = {SeatClass::ECONOMY, SeatClass::BUSINESS};
        flights.push_back(f);
    }
    return flights;
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    const int N = 10000;

    std::cout << "=== PricingModel Benchmark: Cached vs Uncached ===\n\n";

    Airline airline = makeAirline();
    PricingConfig config;

    // ========================================================================
    // SECTION 1: PricingModel with cache — first pass (cold cache)
    // ========================================================================
    std::cout << "SECTION 1: PricingModel::populatePrices (cold cache, " << N << " flights)\n";
    std::cout << "------------------------------------------------------------\n";

    PriceCache coldCache(N);
    PricingModel coldModel(coldCache);
    std::vector<Flight> coldFlights = makeFlights(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (Flight& f : coldFlights)
        coldModel.populatePrices(f, airline, config);
    auto end = std::chrono::high_resolution_clock::now();
    auto coldTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << coldTime << " microseconds\n\n";

    // ========================================================================
    // SECTION 2: PricingModel with cache — second pass (warm cache, same flights)
    // ========================================================================
    std::cout << "SECTION 2: PricingModel::populatePrices (warm cache, same flights)\n";
    std::cout << "------------------------------------------------------------\n";

    // Reuse the same cache and same flights — all prices already cached
    start = std::chrono::high_resolution_clock::now();
    for (Flight& f : coldFlights)
        coldModel.populatePrices(f, airline, config);
    end = std::chrono::high_resolution_clock::now();
    auto warmTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << warmTime << " microseconds\n\n";

    // ========================================================================
    // SECTION 3: Inline pricing — no caching, recomputes every time
    // ========================================================================
    std::cout << "SECTION 3: Inline pricing (no caching, " << N << " flights)\n";
    std::cout << "------------------------------------------------------------\n";

    std::vector<Flight> inlineFlights = makeFlights(N);

    double priceSum = 0.0;  // Prevent compiler optimization
    start = std::chrono::high_resolution_clock::now();
    for (const Flight& f : inlineFlights) {
        double baseRate       = 0.10;
        double distance       = 3983.0;  // JFK->LAX km approx
        double seatMultiplier = 1.0;
        double randomFactor   = 0.8 + (rand() % 40) / 100.0;
        double price = baseRate * distance * seatMultiplier * f.popularityScore * randomFactor;
        priceSum += price;
    }
    end = std::chrono::high_resolution_clock::now();
    auto inlineTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << inlineTime << " microseconds\n";
    std::cout << "(Accumulated sum: " << priceSum << " — prevents optimization)\n\n";

    // ========================================================================
    // SECTION 4: Summary
    // ========================================================================
    std::cout << "=== Summary ===\n";
    std::cout << "Cold cache (first pass):   " << coldTime   << " us\n";
    std::cout << "Warm cache (second pass):  " << warmTime   << " us\n";
    std::cout << "Inline (no caching):       " << inlineTime << " us\n";
    std::cout << "Cache speedup (cold->warm):" << (coldTime / (double)warmTime) << "x\n";
    std::cout << "\nNote: Warm cache hits are O(1) — same route reused across queries.\n";
    std::cout << "Note: Inline recomputes from scratch every time with no benefit.\n";

    return 0;
}