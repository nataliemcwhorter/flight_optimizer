//
// Created by Natalie McWhorter on 4/14/26.
//

#include "ResultDisplay.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
using namespace std;

static std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::CHEAPEST:     return "Cheapest";
        case Priority::FASTEST:      return "Fastest";
        case Priority::FEWEST_STOPS: return "Fewest Stops";
        case Priority::BEST_VALUE:   return "Best Value";
        default:                     return "Unknown";
    }
}

void ResultDisplay::printResults(const std::vector<FlightOffer>& offers, const UserQuery& query) {
    printSeparator();
    printQuerySummary(query);
    printSeparator();
    if (offers.empty()) {
        printNoResults();
        return;
    }
    cout << "Found " << offers.size() << " flight offer(s):\n";
    for (int i = 0; i < static_cast<int>(offers.size()); i++) {
        printOffer(offers[i], i + 1);
    }
    printSeparator();
}

int ResultDisplay::printResultsPaginated(const std::vector<FlightOffer>& offers, const UserQuery& query, int startIndex, int count) {
    if (startIndex == 0) {
        printSeparator();
        printQuerySummary(query);
        printSeparator();
    }

    if (offers.empty()) {
        if (startIndex == 0) printNoResults();
        return 0;
    }

    if (startIndex == 0) {
        cout << "Found " << offers.size() << " flight offer(s):\n";
    }

    int endIndex = std::min(startIndex + count, static_cast<int>(offers.size()));
    int printed = 0;

    for (int i = startIndex; i < endIndex; i++) {
        printOffer(offers[i], i + 1);
        printed++;
    }

    if (endIndex < static_cast<int>(offers.size())) {
        cout << "\n" << (offers.size() - endIndex) << " more offer(s) available.\n";
    }

    if (endIndex >= static_cast<int>(offers.size())) {
        printSeparator();
    }

    return printed;
}

void ResultDisplay::printOffer(const FlightOffer& offer, int index) {
    long long totalMins = offer.getTotalDurationMinutes();
    cout << "[" << index << "] --- FlightOffer ---\n";
    cout << fixed << setprecision(2);
    cout << "  Price:    $" << offer.getTotalPrice() << "\n";
    cout << "  Duration: " << formatDuration(static_cast<int>(totalMins)) << "\n";
    cout << "  Stops:    " << offer.getStopCount() << "\n";
    cout << "  Class:    " << seatClassToString(offer.selectedClass) << "\n";
    for (int j = 0; j < static_cast<int>(offer.legs.size()); j++) {
        printLeg(offer.legs[j], j + 1);
    }
}

void ResultDisplay::printLeg(const Flight& flight, int legNumber) {
    cout << "  Leg " << legNumber << ":\n";
    cout << "    Flight:    [" << flight.flightNumber << "] (" << flight.airlineCode << ")\n";
    cout << "    Route:     " << flight.origin.iataCode << " \u2192 " << flight.destination.iataCode << "\n";
    cout << "    Departs:   " << formatTimestamp(flight.departureTime) << "\n";
    cout << "    Arrives:   " << formatTimestamp(flight.arrivalTime) << "\n";
    cout << "    Status:    " << flight.liveStatus << "\n";
}

void ResultDisplay::printQuerySummary(const UserQuery& query) {
    cout << "Query: " << query.origin << " \u2192 " << query.destination << " on " << query.departDate << "\n";
    if (!query.returnDate.empty())
        cout << "  Return:    " << query.returnDate << "\n";
    cout << fixed << setprecision(2);
    cout << "  Class:     " << seatClassToString(query.seatClass) << "\n";
    cout << "  Budget:    $" << query.budgetCap << "\n";
    cout << "  Max Stops: " << query.maxStops << "\n";
    cout << "  Priority:  " << priorityToString(query.priority) << "\n";
}

void ResultDisplay::printNoResults() {
    cout << "No flight offers found matching your criteria. Try relaxing your filters.\n";
}

void ResultDisplay::printSeparator() {
    cout << std::string(50, '-') << endl;
}

std::string ResultDisplay::formatDuration(int minutes) {
    int hours = minutes / 60;
    int mins  = minutes % 60;
    return std::to_string(hours) + "h " + std::to_string(mins) + "m";
}

std::string ResultDisplay::formatTimestamp(long long unixTime) {
    time_t t = static_cast<time_t>(unixTime);
    struct tm* tm_info = std::localtime(&t);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%H:%M on %Y-%m-%d", tm_info);
    return std::string(buffer);
}