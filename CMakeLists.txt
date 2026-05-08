cmake_minimum_required(VERSION 4.1)
project(flight_optimizer)
include_directories(include)

set(CMAKE_CXX_STANDARD 23)

add_executable(benchmark_cache tests/benchmark_cache.cpp
        src/structures/PriceCache.cpp
        src/structures/PriceCache.h)

add_executable(benchmark_graph tests/benchmark_graph.cpp
        src/structures/PriceCache.cpp
        src/structures/PriceCache.h
        src/structures/FlightGraph.cpp
        src/structures/FlightGraph.h)

add_executable(benchmark_heap tests/benchmark_heap.cpp
        src/structures/FlightHeap.h
        src/models/FlightOffer.h
        src/structures/FlightHeap.cpp)


add_executable(benchmark_pathfinder tests/benchmark_pathfinder.cpp
        src/structures/FlightGraph.h
        src/structures/FlightGraph.cpp
        src/engine/PathFinder.cpp
        src/engine/PathFinder.h
        src/models/Flight.h
        src/models/Airport.h
        src/models/UserQuery.h
        src/structures/FlightHeap.cpp
        src/structures/FlightHeap.h
        )

#include "../src/engine/PricingModel.h"
#include "../src/structures/PriceCache.h"
#include "../src/models/FlightOffer.h"
#include "../src/models/Flight.h"
#include "../src/models/Airport.h"
#include "../src/models/Airline.h"
add_executable(benchmark_pricing tests/benchmark_pricing.cpp
        src/engine/PricingModel.h
        src/engine/PricingModel.cpp
        src/structures/PriceCache.cpp
        src/structures/PriceCache.h
        src/models/FlightOffer.h
        src/models/Flight.h
        src/models/Airport.h
        src/models/Airline.h
        )


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

find_package(CURL REQUIRED)
target_link_libraries(flight_optimizer CURL::libcurl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
