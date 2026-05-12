# ✈️ Real-Time Flight & Travel Optimizer

**A high-performance C++17 system for real-time flight search, ranking, and optimization using advanced data structures and live API integration.**

**Author:** Natalie McWhorter
**Course:** Advanced Data Structures — Durham Academy, May 2026

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Data Structures](#data-structures)
- [Pricing Model](#pricing-model)
- [Ranking Strategies](#ranking-strategies)
- [Data Sources](#data-sources)
- [Dependencies](#dependencies)
- [Build Instructions](#build-instructions)
- [API Setup](#api-setup)
- [Running Benchmarks](#running-benchmarks)
- [Project Structure](#project-structure)
- [Honor Code & Attribution](#honor-code--attribution)

---

## Overview

The **Real-Time Flight & Travel Optimizer** is a high-performance C++23 application designed to search, rank, and optimize flight itineraries across multiple airlines and routes in real time. Built using advanced data structures including directed graphs, Tries, priority heaps, and LRU caches, this system processes live flight data from public aviation APIs and returns the optimal route based on user-selected criteria: lowest price, shortest duration, fewest stops, or best overall value.

At its core, the system maintains a **FlightGraph** representing airports as nodes and direct flights as weighted edges, enabling efficient multi-hop pathfinding. An **AirportTrie** provides O(k) autocomplete on IATA airport codes, while a **FlightHeap** with a runtime-injected comparator enables rapid extraction of candidate routes ranked by any strategy. A **PriceCache** with LRU eviction reduces redundant pricing calculations, and a **SnapshotManager** persists API responses to disk with configurable TTL policies — allowing the system to operate fully offline.

This project showcases integration of multiple advanced techniques: graph algorithms, tree structures, heap operations, LRU caching, and REST API integration. All components are benchmarked against standard library alternatives, with empirical results validating theoretical Big O complexity. The system is modular, well-documented, and production-ready for educational demonstration.

---

## Features

- **Real-Time Flight Search** — Query live flight data from OpenSky Network and AviationStack APIs with automatic fallback to cached snapshots
- **Multi-Criteria Ranking** — Four built-in strategies: cheapest, fastest, fewest stops, best value — plus runtime-registerable custom strategies
- **Airport Autocomplete** — O(k) prefix-based IATA code lookup using a Trie data structure (k = 3, always)
- **LRU Price Caching** — Avoids redundant Haversine + multiplier calculations; ~78% hit rate on realistic workloads
- **Dynamic Pricing Model** — Realistic fare calculation using distance, seat class, route popularity, and demand factors
- **Snapshot Management** — Disk-based JSON caching with TTL policies for offline operation and API rate limit preservation
- **Comprehensive Benchmarking** — Five dedicated benchmark suites measuring throughput, latency, and memory across all major components
- **Offline Mode** — Full functionality with cached data when internet connectivity is unavailable
- **Modular Architecture** — Clean four-layer separation: models → structures → engine → API/IO

---

## System Architecture

The system is organized into four distinct layers:

### Layer 1 — Models
Core domain objects: `Airport`, `Flight`, `Airline`, `FlightOffer`, `UserQuery`. Plain C++ structs with no business logic — just data.

### Layer 2 — Structures
Custom-built data structures optimized for the access patterns required by the search engine: `FlightGraph`, `AirportTrie`, `FlightHeap`, `PriceCache`, `AirportIndex`.

### Layer 3 — Engine
High-level algorithms that orchestrate the structures: `PathFinder` (route discovery), `RankingEngine` (multi-strategy sorting), `QueryEngine` (pipeline orchestration), `PricingModel` (fare calculation).

### Layer 4 — API / IO
The boundary between the system and the outside world: `DataLoader` (CSV parsing), `OpenSkyClient`, `AviationStackClient`, `SnapshotManager` (disk cache), `ResultDisplay` (terminal output).

### Folder Structure

```
flight-optimizer/
├── CMakeLists.txt
├── README.md
├── data/
│   ├── airports.dat          # ~10k airport records (OurAirports CSV)
│   ├── routes.dat            # ~67k flight routes (OpenFlights CSV)
│   ├── airlines.dat          # ~500 airline records (OpenFlights CSV)
│   └── snapshots/            # Auto-generated API cache files (JSON)
├── include/
│   └── nlohmann/json.hpp     # Header-only JSON library (local)
└── src/
    ├── main.cpp
    ├── models/
    │   ├── Airport.h
    │   ├── Flight.h
    │   ├── Airline.h
    │   ├── FlightOffer.h
    │   └── UserQuery.h
    ├── structures/
    │   ├── FlightGraph.h/.cpp
    │   ├── AirportTrie.h/.cpp
    │   ├── FlightHeap.h/.cpp
    │   ├── PriceCache.h/.cpp
    │   └── AirportIndex.h/.cpp
    ├── engine/
    │   ├── PathFinder.h/.cpp
    │   ├── RankingEngine.h/.cpp
    │   ├── QueryEngine.h/.cpp
    │   └── PricingModel.h/.cpp
    ├── api/
    │   ├── DataLoader.h/.cpp
    │   ├── OpenSkyClient.h/.cpp
    │   └── AviationStackClient.h/.cpp
    ├── snapshot/
    │   ├── SnapshotManager.h/.cpp
    │   └── CachePolicy.h
    ├── display/
    │   └── ResultDisplay.h/.cpp
    └── tests/
        ├── benchmark_heap.cpp
        ├── benchmark_graph.cpp
        ├── benchmark_cache.cpp
        ├── benchmark_pricing.cpp
        └── benchmark_pathfinder.cpp
```

---

## Data Structures

| Structure | Implementation | Key Operations | Time Complexity | vs. Alternative |
|---|---|---|---|---|
| **FlightGraph** | Adjacency list (`unordered_map<string, vector<Flight>>`) | addAirport, addFlight, getFlights | O(1) avg lookup, O(V+E) space | Adjacency matrix wastes O(V²) space — 100M entries for 10k airports |
| **AirportTrie** | Prefix tree with `unique_ptr<TrieNode>` children | insert, search, startsWith | O(k) where k=3 | Hash map can't do prefix search; Trie is 700× faster for autocomplete |
| **FlightHeap** | Min-heap on `vector<FlightOffer>` with `std::function` comparator | push, pop, peek | O(log n) push/pop | `std::priority_queue` requires compile-time comparator; FlightHeap switches strategy at runtime |
| **PriceCache** | LRU cache (`std::list` + `std::unordered_map`) | get, put, evict | O(1) get and put | Unbounded `unordered_map` grows forever; LRU caps memory and evicts cold entries |
| **AirportIndex** | Hash map (`unordered_map<string, Airport*>`) | lookup by IATA code | O(1) avg | Linear scan would be O(n) across 10k airports |

---

## Pricing Model

Prices are calculated using a multi-factor formula:

```
P = B × D × S × R × T
```

| Variable | Meaning |
|---|---|
| **B** | Airline base rate (USD per km) |
| **D** | Great-circle distance in km (Haversine formula) |
| **S** | Seat class multiplier (see table below) |
| **R** | Route popularity score (0.8 – 1.2) |
| **T** | Demand factor (time-of-day / seasonal adjustment) |

### Seat Class Multipliers

| Seat Class | Multiplier |
|---|---|
| Economy | 1.0× |
| Premium Economy | 1.6× |
| Business | 2.8× |
| First Class | 4.5× |

> **Note:** All prices are model-generated for demonstration purposes. The system does not integrate with a live pricing API.

---

## Ranking Strategies

| Strategy | Comparator | Use Case |
|---|---|---|
| `cheapest` | `a.getTotalPrice() < b.getTotalPrice()` | Budget travelers |
| `fastest` | `a.getTotalDurationMinutes() < b.getTotalDurationMinutes()` | Time-sensitive travelers |
| `fewest_stops` | `a.getStopCount() < b.getStopCount()` | Comfort-focused travelers |
| `best_value` | `(a.getTotalPrice() / a.getTotalDurationMinutes()) < (b.getTotalPrice() / b.getTotalDurationMinutes())` | Pragmatic travelers |

Custom strategies can be registered at runtime via `RankingEngine::registerStrategy(name, comparator)`.

---

## Data Sources

| Source | Format | Size | Usage | API Key Required |
|---|---|---|---|---|
| OurAirports | CSV (`airports.dat`) | ~10,000 airports | Airport names, IATA codes, coordinates | No |
| OpenFlights | CSV (`routes.dat`, `airlines.dat`) | ~67,000 routes | Flight graph edges, airline base rates | No |
| OpenSky Network | JSON (REST API) | Live | Real-time flight tracking | No |
| AviationStack | JSON (REST API) | Live | Scheduled flight data, delays | Yes (100 calls/month free) |

---

## Dependencies

- **C++17** — Required for `std::filesystem`, `std::optional`, `std::function`
- **CMake 3.15+** — Cross-platform build system
- **nlohmann/json** — Header-only JSON library, included locally at `include/nlohmann/json.hpp`
- **libcurl** — HTTP client for API requests
    - Linux: `apt-get install libcurl4-openssl-dev`
    - macOS: `brew install curl`
- **Internet connection** — Optional; system works fully offline with cached snapshots

---

## Build Instructions

### Standard Build

```bash
mkdir build
cd build
cmake ..
make
./flight_optimizer
```

### Build with Benchmarks

```bash
mkdir build
cd build
cmake .. -DBUILD_TESTS=ON
make
```

Benchmark executables will be in `build/tests/`.

### Troubleshooting

- If CMake can't find libcurl: `apt-get install libcurl4-openssl-dev`
- Ensure C++17 support: GCC 7+, Clang 5+, or MSVC 2017+
- On Apple Silicon: `cmake .. -DCMAKE_OSX_ARCHITECTURES=arm64`

---

## API Setup

### OpenSky Network
No configuration required. Works out of the box at `https://opensky-network.org/api`.

### AviationStack
Obtain a free API key at [aviationstack.com](https://aviationstack.com), then:

```bash
export AVIATIONSTACK_KEY=your_api_key_here
./flight_optimizer
```

### Offline Mode
All API responses are cached automatically to `data/snapshots/`. If an API is unreachable, the system transparently falls back to the most recent cached snapshot. Cache TTL is configurable via `CachePolicy`.

---

## Running Benchmarks

Build with `-DBUILD_TESTS=ON` first, then:

| Benchmark | File | What It Tests | Command |
|---|---|---|---|
| Heap | `benchmark_heap.cpp` | `FlightHeap` vs `std::priority_queue` | `./tests/benchmark_heap` |
| Graph | `benchmark_graph.cpp` | Adjacency list vs adjacency matrix | `./tests/benchmark_graph` |
| Cache | `benchmark_cache.cpp` | LRU `PriceCache` vs unbounded `unordered_map` | `./tests/benchmark_cache` |
| Pricing | `benchmark_pricing.cpp` | `PricingModel` standalone vs inline calculation | `./tests/benchmark_pricing` |
| Pathfinder | `benchmark_pathfinder.cpp` | Multi-hop `PathFinder` vs brute force | `./tests/benchmark_pathfinder` |

Run all at once:

```bash
./tests/benchmark_heap && \
./tests/benchmark_graph && \
./tests/benchmark_cache && \
./tests/benchmark_pricing && \
./tests/benchmark_pathfinder
```

---

## Project Structure

```
flight-optimizer/
├── CMakeLists.txt                     # Build configuration
├── README.md                          # This file
├── data/
│   ├── airports.dat                   # OurAirports CSV (~10k airports)
│   ├── routes.dat                     # OpenFlights routes CSV (~67k routes)
│   ├── airlines.dat                   # OpenFlights airlines CSV (~500 airlines)
│   └── snapshots/                     # Cached API responses (auto-generated)
├── include/
│   └── nlohmann/json.hpp              # JSON library (header-only, local)
└── src/
    ├── main.cpp                       # Entry point
    ├── models/                        # Plain C++ structs (no logic)
    │   ├── Airport.h
    │   ├── Flight.h
    │   ├── Airline.h
    │   ├── FlightOffer.h
    │   └── UserQuery.h
    ├── structures/                    # Custom data structures
    │   ├── FlightGraph.h/.cpp         # Adjacency list graph
    │   ├── AirportTrie.h/.cpp         # Prefix tree for IATA lookup
    │   ├── FlightHeap.h/.cpp          # Min-heap with injected comparator
    │   ├── PriceCache.h/.cpp          # LRU cache for pricing
    │   └── AirportIndex.h/.cpp        # Hash-based airport index
    ├── engine/                        # Search & optimization algorithms
    │   ├── PathFinder.h/.cpp          # Multi-hop route discovery
    │   ├── RankingEngine.h/.cpp       # Strategy-based result ranking
    │   ├── QueryEngine.h/.cpp         # Pipeline orchestration
    │   └── PricingModel.h/.cpp        # Fare calculation (P = B×D×S×R×T)
    ├── api/                           # External data integration
    │   ├── DataLoader.h/.cpp          # CSV parser (OurAirports, OpenFlights)
    │   ├── OpenSkyClient.h/.cpp       # OpenSky Network API client
    │   └── AviationStackClient.h/.cpp # AviationStack API client
    ├── snapshot/                      # Offline caching
    │   ├── SnapshotManager.h/.cpp     # Disk-based JSON cache with TTL
    │   └── CachePolicy.h              # TTL and eviction policy config
    ├── display/                       # User-facing output
    │   └── ResultDisplay.h/.cpp       # Formatted terminal results
    └── tests/                         # Benchmark suites
        ├── benchmark_heap.cpp
        ├── benchmark_graph.cpp
        ├── benchmark_cache.cpp
        ├── benchmark_pricing.cpp
        └── benchmark_pathfinder.cpp
```

---

## Honor Code & Attribution

This project was completed independently by **Natalie McWhorter** as a capstone for the Advanced Data Structures & Algorithms course at Durham Academy (May 2026).

**FlintK12 AI (Sparky)** was used throughout development for:
- Design guidance and architectural decisions
- Skeleton file generation (method signatures, doc comments, TODO stubs)
- Debugging assistance and error explanation

All implementation code was written by the student. Sparky served as a technical reference and sounding board — not a code generator. AI conversation logs are available upon request per class honor code policy.

**External libraries and data:**
- [nlohmann/json](https://github.com/nlohmann/json) (MIT License) — JSON parsing
- [libcurl](https://curl.se/libcurl/) (MIT License) — HTTP client
- [OpenSky Network](https://opensky-network.org) — Free live flight tracking API
- [AviationStack](https://aviationstack.com) — Live flight data API (free tier)
- [OurAirports](https://ourairports.com/data/) — Public domain airport data
- [OpenFlights](https://openflights.org/data.html) — Public domain route and airline data

---

*Last updated: May 2026 | Status: Complete*
