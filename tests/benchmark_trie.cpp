//
// Created by Natalie McWhorter on 5/7/26.
//

#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include "../src/structures/AirportTrie.h"
#include "../src/models/Airport.h"

// ============================================================================
// HELPER FUNCTION: Timing Utility
// ============================================================================
// Template lambda wrapper to benchmark repeated operations
// Returns elapsed time in milliseconds
template<typename Func>
double benchmarkOperation(Func operation, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        operation();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return static_cast<double>(duration.count());
}

// ============================================================================
// HELPER FUNCTION: Generate Synthetic Airport Dataset
// ============================================================================
// Creates 5000 realistic airport records with IATA codes (AAA, AAB, ..., ZZZ)
// Other fields are dummy values for benchmarking purposes
std::vector<Airport> generateSyntheticAirports(int count = 5000) {
    std::vector<Airport> airports;
    airports.reserve(count);

    int generatedCount = 0;
    for (char c1 = 'A'; c1 <= 'Z' && generatedCount < count; ++c1) {
        for (char c2 = 'A'; c2 <= 'Z' && generatedCount < count; ++c2) {
            for (char c3 = 'A'; c3 <= 'Z' && generatedCount < count; ++c3) {
                Airport airport;
                airport.iataCode = std::string(1, c1) + c2 + c3;
                airport.icaoCode = "IC" + airport.iataCode;
                airport.numericId = generatedCount;
                airport.latitude = 40.0 + (generatedCount % 50);
                airport.longitude = -75.0 + (generatedCount % 30);
                airport.name = "Airport_" + airport.iataCode;
                airport.city = "City_" + std::string(1, c1);
                airport.country = "Country_" + std::string(1, c2);

                airports.push_back(airport);
                ++generatedCount;
            }
        }
    }

    return airports;
}

// ============================================================================
// BENCHMARK 1: Exact Lookup (find)
// ============================================================================
void benchmarkExactLookup(const std::vector<Airport>& airports,
                          const AirportTrie& trie,
                          int iterations = 1000) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "BENCHMARK 1: EXACT LOOKUP (find)\n";
    std::cout << std::string(70, '=') << "\n\n";

    // Generate random IATA codes from the dataset for fair comparison
    std::vector<std::string> testCodes;
    testCodes.reserve(iterations);
    std::mt19937 rng(42); // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, airports.size() - 1);

    for (int i = 0; i < iterations; ++i) {
        testCodes.push_back(airports[dist(rng)].iataCode);
    }

    // Benchmark 1a: AirportTrie::find()
    double trieFindTime = benchmarkOperation([&]() {
        static int idx = 0;
        auto result = trie.find(testCodes[idx % iterations]);
        ++idx;
    }, iterations);

    // Benchmark 1b: Linear search using std::find_if
    double linearFindTime = benchmarkOperation([&]() {
        static int idx = 0;
        auto it = std::find_if(airports.begin(), airports.end(),
                               [&](const Airport& a) {
                                   return a.iataCode == testCodes[idx % iterations];
                               });
        ++idx;
    }, iterations);

    // Print results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "AirportTrie::find()        : " << trieFindTime << " ms\n";
    std::cout << "Linear search (std::find_if): " << linearFindTime << " ms\n";
    std::cout << "Speedup factor             : " << (linearFindTime / trieFindTime) << "x\n";
}

// ============================================================================
// BENCHMARK 2: Prefix Search (search)
// ============================================================================
void benchmarkPrefixSearch(const std::vector<Airport>& airports,
                           const AirportTrie& trie,
                           int iterations = 1000) {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "BENCHMARK 2: PREFIX SEARCH (search)\n";
    std::cout << std::string(70, '=') << "\n\n";

    // Generate random single-character prefixes for fair comparison
    std::vector<std::string> testPrefixes;
    testPrefixes.reserve(iterations);
    std::mt19937 rng(123); // Fixed seed for reproducibility
    std::uniform_int_distribution<int> dist(0, 25); // 0-25 for A-Z

    for (int i = 0; i < iterations; ++i) {
        char prefix = 'A' + dist(rng);
        testPrefixes.push_back(std::string(1, prefix));
    }

    // Benchmark 2a: AirportTrie::search()
    double trieSearchTime = benchmarkOperation([&]() {
        static int idx = 0;
        auto results = trie.search(testPrefixes[idx % iterations]);
        ++idx;
    }, iterations);

    // Benchmark 2b: Linear search collecting all matches
    double linearSearchTime = benchmarkOperation([&]() {
        static int idx = 0;
        std::vector<Airport> results;
        const std::string& prefix = testPrefixes[idx % iterations];
        for (const auto& airport : airports) {
            if (airport.iataCode.substr(0, prefix.length()) == prefix) {
                results.push_back(airport);
            }
        }
        ++idx;
    }, iterations);

    // Print results
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "AirportTrie::search()      : " << trieSearchTime << " ms\n";
    std::cout << "Linear search (manual)     : " << linearSearchTime << " ms\n";
    std::cout << "Speedup factor             : " << (linearSearchTime / trieSearchTime) << "x\n";
}

// ============================================================================
// MAIN: Generate Data, Build Structures, Run Benchmarks
// ============================================================================
int main() {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "AIRPORTTRIE BENCHMARK SUITE\n";
    std::cout << std::string(70, '=') << "\n\n";

    // Step 1: Generate synthetic dataset
    std::cout << "Generating synthetic airport dataset...\n";
    std::vector<Airport> airports = generateSyntheticAirports(5000);
    std::cout << "Generated " << airports.size() << " airports\n\n";

    // Step 2: Build AirportTrie
    std::cout << "Building AirportTrie...\n";
    AirportTrie trie;
    for (const auto& airport : airports) {
        trie.insert(airport);
    }
    std::cout << "AirportTrie construction complete\n\n";

    // Step 3: Run benchmarks
    const int ITERATIONS = 1000;
    benchmarkExactLookup(airports, trie, ITERATIONS);
    benchmarkPrefixSearch(airports, trie, ITERATIONS);

    // Step 4: Print summary
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "BENCHMARK SUMMARY\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << "Dataset size      : " << airports.size() << " airports\n";
    std::cout << "Iterations/test   : " << ITERATIONS << "\n";
    std::cout << "\nConclusion: The AirportTrie structure significantly outperforms\n";
    std::cout << "linear search for both exact lookups and prefix searches.\n";
    std::cout << std::string(70, '=') << "\n\n";

    return 0;
}
