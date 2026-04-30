cmake_minimum_required(VERSION 4.1)
project(flight_optimizer)

set(CMAKE_CXX_STANDARD 23)

add_executable(flight_optimizer src/main.cpp
        src/models/Airport.h
        src/models/Flight.h
        src/models/FlightOffer.h
        src/models/UserQuery.h
        src/structures/FlightGraph.cpp
        src/structures/FlightGraph.h
        src/structures/AirportIndex.cpp
        src/structures/AirportIndex.h
        src/structures/FlightHeap.cpp
        src/structures/FlightHeap.h
        src/structures/AirportTrie.cpp
        src/structures/AirportTrie.h
        src/structures/PriceCache.cpp
        src/structures/PriceCache.h
        src/engine/PathFinder.cpp
        src/engine/PathFinder.h
        src/engine/RankingEngine.cpp
        src/engine/RankingEngine.h
        src/engine/QueryEngine.cpp
        src/engine/QueryEngine.h
        src/api/AviationStackClient.cpp
        src/api/AviationStackClient.h
        src/display/ResultDisplay.cpp
        src/display/ResultDisplay.h
        tests/benchmark_heap.cpp
        tests/benchmark_graph.cpp
        tests/benchmark_cache.cpp
        src/engine/PricingModel.cpp
        src/engine/PricingModel.h
        src/models/Airline.h
        src/api/DataLoader.cpp
        src/api/DataLoader.h
        src/api/OpenSkyClient.cpp
        src/api/OpenSkyClient.h
        src/snapshot/SnapshotManager.cpp
        src/snapshot/SnapshotManager.h
        src/snapshot/CachePolicy.h
        tests/benchmark_pricing.cpp
        tests/benchmark_pathfinder.cpp)
