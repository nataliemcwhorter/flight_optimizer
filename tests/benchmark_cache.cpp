//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <string>
#include "../src/structures/PriceCache.h"

int main() {
    const int N = 100000;

    std::cout << "=== PriceCache (unordered_map) vs std::map Benchmark ===\n\n";

    // ============ Section 1: PriceCache Insert ============
    std::cout << "Section 1: PriceCache insert(" << N << " items)\n";
    PriceCache cache(N);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        std::string key = "JFK-LAX-" + std::to_string(i);
        double value = i * 1.5;
        cache.put(key, value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto cacheInsertTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << cacheInsertTime << " microseconds\n";
    std::cout << "Cache size: " << cache.size() << "\n\n";

    // ============ Section 2: PriceCache Lookup ============
    std::cout << "Section 2: PriceCache lookup (contains + get, " << N << " items)\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        std::string key = "JFK-LAX-" + std::to_string(i);
        if (cache.contains(key)) {
            auto result = cache.get(key);
            (void)result;
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto cacheLookupTime = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start).count();
    std::cout << "Time: " << cacheLookupTime << " microseconds\n\n";

    // ============ Section 3: std::map Insert ============
    std::cout << "Section 3: std::map insert(" << N << " items)\n";
    std::map<std::string, double> m;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        std::string key = "JFK-LAX-" + std::to_string(i);
        m[key] = i * 1.5;
    }
    auto end3 = std::chrono::high_resolution_clock::now();
    auto mapInsertTime = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start).count();
    std::cout << "Time: " << mapInsertTime << " microseconds\n";
    std::cout << "Map size: " << m.size() << "\n\n";

    // ============ Section 4: std::map Lookup ============
    std::cout << "Section 4: std::map lookup (find, " << N << " items)\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        std::string key = "JFK-LAX-" + std::to_string(i);
        auto it = m.find(key);
        (void)(it != m.end());
    }
    auto end4 = std::chrono::high_resolution_clock::now();
    auto mapLookupTime = std::chrono::duration_cast<std::chrono::microseconds>(end4 - start).count();
    std::cout << "Time: " << mapLookupTime << " microseconds\n\n";

    // ============ Section 5: Comparison Summary ============
    std::cout << "=== Summary ===\n";
    std::cout << "PriceCache Insert:  " << cacheInsertTime << " us (O(1) avg)\n";
    std::cout << "std::map Insert:    " << mapInsertTime << " us (O(log n))\n";
    std::cout << "Insert Speedup:     " << (mapInsertTime / (double)cacheInsertTime) << "x faster with unordered_map\n\n";

    std::cout << "PriceCache Lookup:  " << cacheLookupTime << " us (O(1) avg)\n";
    std::cout << "std::map Lookup:    " << mapLookupTime << " us (O(log n))\n";
    std::cout << "Lookup Speedup:     " << (mapLookupTime / (double)cacheLookupTime) << "x faster with unordered_map\n";

    return 0;
}