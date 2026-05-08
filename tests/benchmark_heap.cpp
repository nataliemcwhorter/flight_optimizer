//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <chrono>
#include <vector>
#include <queue>
#include <functional>
#include "../src/structures/FlightHeap.h"
#include "../src/models/FlightOffer.h"

// Creates a FlightOffer with a single leg priced at `price` for ECONOMY
FlightOffer makeOffer(double price) {
    Flight leg;
    leg.flightNumber   = "FL001";
    leg.popularityScore = 1.0;
    leg.availableClasses = {SeatClass::ECONOMY};
    leg.pricePerClass[SeatClass::ECONOMY] = price;

    FlightOffer offer;
    offer.selectedClass = SeatClass::ECONOMY;
    offer.legs = {leg};
    return offer;
}

int main() {
    const int N = 100000;

    std::cout << "=== FlightHeap vs std::priority_queue Benchmark ===\n\n";

    // ============ Section 1: FlightHeap Push ============
    std::cout << "Section 1: FlightHeap push(" << N << " items)\n";
    auto cmp = [](const FlightOffer& a, const FlightOffer& b) {
        return a.getTotalPrice() < b.getTotalPrice();
    };
    FlightHeap heap(cmp);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        heap.push(makeOffer(i * 0.01));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto heapPushTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << heapPushTime << " microseconds\n\n";

    // ============ Section 2: FlightHeap Pop ============
    std::cout << "Section 2: FlightHeap pop(" << N << " items)\n";
    start = std::chrono::high_resolution_clock::now();
    while (!heap.isEmpty()) {
        auto result = heap.pop();
        (void)result;
    }
    end = std::chrono::high_resolution_clock::now();
    auto heapPopTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << heapPopTime << " microseconds\n\n";

    // ============ Section 3: std::priority_queue Push ============
    std::cout << "Section 3: std::priority_queue push(" << N << " items)\n";
    std::priority_queue<double, std::vector<double>, std::greater<double>> pq;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        pq.push(i * 0.01);
    }
    end = std::chrono::high_resolution_clock::now();
    auto pqPushTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << pqPushTime << " microseconds\n\n";

    // ============ Section 4: std::priority_queue Pop ============
    std::cout << "Section 4: std::priority_queue pop(" << N << " items)\n";
    start = std::chrono::high_resolution_clock::now();
    while (!pq.empty()) {
        pq.pop();
    }
    end = std::chrono::high_resolution_clock::now();
    auto pqPopTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time: " << pqPopTime << " microseconds\n\n";

    // ============ Section 5: Comparison Summary ============
    std::cout << "=== Summary ===\n";
    std::cout << "FlightHeap Push:           " << heapPushTime << " us\n";
    std::cout << "std::priority_queue Push:  " << pqPushTime << " us\n";
    std::cout << "Faster: " << (heapPushTime < pqPushTime ? "FlightHeap" : "std::priority_queue") << "\n\n";

    std::cout << "FlightHeap Pop:            " << heapPopTime << " us\n";
    std::cout << "std::priority_queue Pop:   " << pqPopTime << " us\n";
    std::cout << "Faster: " << (heapPopTime < pqPopTime ? "FlightHeap" : "std::priority_queue") << "\n";

    return 0;
}