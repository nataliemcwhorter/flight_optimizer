//
// Created by Natalie McWhorter on 4/14/26.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "models/UserQuery.h"
#include "structures/FlightGraph.h"
#include "structures/AirportIndex.h"
#include "engine/PathFinder.h"
#include "engine/RankingEngine.h"
#include "engine/QueryEngine.h"
#include "engine/PricingModel.h"
#include "api/DataLoader.h"
#include "api/OpenSkyClient.h"
#include "api/AviationStackClient.h"
#include "snapshot/SnapshotManager.h"
#include "snapshot/CachePolicy.h"
#include "display/ResultDisplay.h"
using namespace std;

SeatClass parseSeatClass(const std::string& input) {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "premium economy" || lower == "premium") return SeatClass::PREMIUM_ECONOMY;
    if (lower == "business")                               return SeatClass::BUSINESS;
    if (lower == "first" || lower == "first class")        return SeatClass::FIRST;
    if (lower != "economy")
        cout << "Unrecognized seat class \"" << input << "\", defaulting to Economy.\n";
    return SeatClass::ECONOMY;
}

Priority parsePriority(const std::string& input) {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "duration" || lower == "fastest")  return Priority::FASTEST;
    if (lower == "stops"    || lower == "fewest")   return Priority::FEWEST_STOPS;
    if (lower == "value"    || lower == "best")     return Priority::BEST_VALUE;
    return Priority::CHEAPEST;
}

long long parseDateToTimestamp(const std::string& dateStr) {
    // Parse "YYYY-MM-DD" format to Unix timestamp
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, "%Y-%m-%d");

    if (ss.fail()) {
        // Default to current time if parse fails
        return std::time(nullptr);
    }

    // Set time to noon (12:00:00) to avoid edge cases
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    return static_cast<long long>(std::mktime(&tm));
}

int main() {
    // === SECTION 1: Initialization ===
    SnapshotManager snapshots("data/snapshots", CachePolicy::shortLived());
    cout << "=== Flight Optimizer ===\n";
    cout << "Initializing...\n";

    // === SECTION 2: Load Static Data ===
    // All three files are from OpenFlights (no header rows, .dat format)
    // Download from:
    //   airports: https://raw.githubusercontent.com/jpatokal/openflights/master/data/airports.dat
    //   routes:   https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat
    //   airlines: https://raw.githubusercontent.com/jpatokal/openflights/master/data/airlines.dat
    // Place them in your project's data/ folder.
    FlightGraph graph;
    AirportIndex airportIndex(graph);
    unordered_map<string, Airline> airlines;

    try {
        DataLoader::loadAirports("data/airports.dat", graph);
        DataLoader::loadRoutes("data/routes.dat", graph);
        DataLoader::loadAirlines("data/airlines.dat", airlines);
    } catch (const std::exception& e) {
        cerr << "Error loading data: " << e.what() << "\n";
        cerr << "Make sure airports.dat, routes.dat, and airlines.dat are in your data/ folder.\n";
        cerr << "Download from: https://raw.githubusercontent.com/jpatokal/openflights/master/data/\n";
        return 1;
    }

    cout << "Loaded " << graph.airportCount() << " airports, "
         << graph.flightCount()   << " routes, "
         << airlines.size()       << " airlines.\n";

    // === SECTION 3: Build Engine & API Clients ===
    PriceCache priceCache(50000);
    PricingModel pricingModel(priceCache);
    QueryEngine queryEngine(graph);

    // Initialize API clients (set your API key here or leave empty to skip API calls)
    string aviationStackApiKey = "e56eb28db8a66e3acc97fcb1c22f499a";  // Get free key from https://aviationstack.com
    AviationStackClient aviationClient(aviationStackApiKey, snapshots);
    OpenSkyClient openSkyClient(graph);

    cout << "Engine ready.\n";
    if (!aviationStackApiKey.empty()) {
        cout << "AviationStack API enabled for live flight data.\n";
    }
    cout << "\n";

    // === SECTION 4: Query Loop ===
    while (true) {
        UserQuery query;

        cout << "Enter origin airport IATA code (or 'quit' to exit): ";
        getline(cin, query.origin);
        std::transform(query.origin.begin(), query.origin.end(), query.origin.begin(), ::toupper);
        if (query.origin == "QUIT") break;

        cout << "Enter destination airport IATA code: ";
        getline(cin, query.destination);
        std::transform(query.destination.begin(), query.destination.end(), query.destination.begin(), ::toupper);

        cout << "Enter departure date (YYYY-MM-DD): ";
        getline(cin, query.departDate);
        query.departureDate = parseDateToTimestamp(query.departDate);

        cout << "Enter seat class (Economy / Premium Economy / Business / First): ";
        string seatInput;
        getline(cin, seatInput);
        query.seatClass = parseSeatClass(seatInput);

        cout << "Enter max stops (0 = direct only): ";
        cin >> query.maxStops;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter max budget in USD (0 = no limit): ";
        cin >> query.budgetCap;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter max duration in minutes (0 = no limit): ";
        cin >> query.maxDurationMinutes;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter priority (price / duration / stops / value): ";
        string priorityInput;
        getline(cin, priorityInput);
        query.priority = parsePriority(priorityInput);

        // Fetch live flights from API if enabled
        vector<Flight> liveFlights;
        if (!aviationStackApiKey.empty()) {
            try {
                cout << "Fetching live flights from " << query.origin << " to " << query.destination << "...\n";
                liveFlights = aviationClient.getLiveFlights(query.origin, query.destination, query.departDate);
                cout << "Found " << liveFlights.size() << " live flights from API.\n";

                // Create temporary graph with ONLY live flights
                FlightGraph liveGraph;

                // Add relevant airports
                auto originAirport = graph.getAirport(query.origin);
                auto destAirport = graph.getAirport(query.destination);
                if (originAirport) liveGraph.addAirport(*originAirport);
                if (destAirport) liveGraph.addAirport(*destAirport);

                // Add only live flights
                for (const Flight& flight : liveFlights) {
                    liveGraph.addFlight(flight);
                }

                // Use temporary live graph for this query
                QueryEngine liveQueryEngine(liveGraph);
                vector<FlightOffer> offers = liveQueryEngine.query(query);

                // Resolve full Airport objects onto legs before pricing
                for (FlightOffer& offer : offers) {
                    for (Flight& leg : offer.legs) {
                        auto origin = graph.getAirport(leg.origin.iataCode);
                        auto dest   = graph.getAirport(leg.destination.iataCode);
                        if (origin) leg.origin      = *origin;
                        if (dest)   leg.destination = *dest;
                    }
                }

                // Populate prices for each leg using matched airline or default
                for (FlightOffer& offer : offers) {
                    for (Flight& leg : offer.legs) {
                        auto it = airlines.find(leg.airlineCode);
                        if (it != airlines.end())
                            pricingModel.populatePrices(leg, it->second);
                        else {
                            Airline defaultAirline;
                            defaultAirline.iataCode = leg.airlineCode;
                            pricingModel.populatePrices(leg, defaultAirline);
                        }
                    }
                }

                // Display results with pagination
                int displayIndex = 0;
                const int pageSize = 5;
                while (true) {
                    ResultDisplay::printResultsPaginated(offers, query, displayIndex, pageSize);
                    displayIndex += pageSize;

                    cout << "\n";
                    if (displayIndex < static_cast<int>(offers.size())) {
                        cout << "Options: (m)ore results / (n)ew search / (q)uit: ";
                    } else {
                        cout << "Options: (n)ew search / (q)uit: ";
                    }

                    string option;
                    getline(cin, option);
                    std::transform(option.begin(), option.end(), option.begin(), ::tolower);

                    if (option == "m" || option == "more") {
                        if (displayIndex >= static_cast<int>(offers.size())) {
                            cout << "No more results to display.\n";
                            continue;
                        }
                        continue;
                    } else if (option == "n" || option == "new" || option == "new search") {
                        cout << "\n";
                        break;
                    } else if (option == "q" || option == "quit") {
                        cout << "Thank you for using Flight Optimizer. Goodbye!\n";
                        return 0;
                    } else {
                        cout << "Invalid option. Try again.\n";
                    }
                }
                continue;
            } catch (const exception& e) {
                cerr << "Warning: Could not fetch live flights: " << e.what() << "\n";
                cerr << "Falling back to static route data.\n";
            }
        }

        // Find and rank paths (fallback to static data)
        vector<FlightOffer> offers = queryEngine.query(query);
        // Resolve full Airport objects onto legs before pricing
        for (FlightOffer& offer : offers) {
            for (Flight& leg : offer.legs) {
                auto origin = graph.getAirport(leg.origin.iataCode);
                auto dest   = graph.getAirport(leg.destination.iataCode);
                if (origin) leg.origin      = *origin;
                if (dest)   leg.destination = *dest;
            }
        }

        // Populate prices for each leg using matched airline or default
        for (FlightOffer& offer : offers) {
            for (Flight& leg : offer.legs) {
                auto it = airlines.find(leg.airlineCode);
                if (it != airlines.end())
                    pricingModel.populatePrices(leg, it->second);
                else {
                    Airline defaultAirline;
                    defaultAirline.iataCode = leg.airlineCode;
                    pricingModel.populatePrices(leg, defaultAirline);
                }
            }
        }

        // Display results with pagination
        int displayIndex = 0;
        const int pageSize = 5;
        while (true) {
            ResultDisplay::printResultsPaginated(offers, query, displayIndex, pageSize);
            displayIndex += pageSize;

            cout << "\n";
            if (displayIndex < static_cast<int>(offers.size())) {
                cout << "Options: (m)ore results / (n)ew search / (q)uit: ";
            } else {
                cout << "Options: (n)ew search / (q)uit: ";
            }

            string option;
            getline(cin, option);
            std::transform(option.begin(), option.end(), option.begin(), ::tolower);

            if (option == "m" || option == "more") {
                if (displayIndex >= static_cast<int>(offers.size())) {
                    cout << "No more results to display.\n";
                    continue;
                }
                continue;
            } else if (option == "n" || option == "new" || option == "new search") {
                cout << "\n";
                break;
            } else if (option == "q" || option == "quit") {
                break;
            } else {
                cout << "Invalid option. Try again.\n";
            }
        }
    }

    cout << "Thank you for using Flight Optimizer. Goodbye!\n";
    return 0;
}