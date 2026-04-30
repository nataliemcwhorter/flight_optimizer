//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_FLIGHTOFFER_H
#define FLIGHT_OPTIMIZER_FLIGHTOFFER_H

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>
#include "Flight.h"

struct FlightOffer {
    std::vector<Flight> legs;   // ordered by departure time
    SeatClass selectedClass;

    [[nodiscard]] double getTotalPrice() const {
        double total = 0.0;
        for (const Flight& leg : legs) {
            auto price = leg.getPrice(selectedClass);
            if (!price.has_value()) {
                // PricingModel hasn't run yet for this leg — treat as 0
                continue;
            }
            total += price.value();
        }
        return total;
    }

    // Returns total journey duration in minutes
    [[nodiscard]] long long getTotalDurationMinutes() const {
        if (legs.empty()) return 0;
        return (legs.back().arrivalTime - legs.front().departureTime) / 60;
    }

    [[nodiscard]] long long getLayoverDurationMinutes(int index) const {
        if (index < 0 || index >= static_cast<int>(legs.size()) - 1) {
            throw std::out_of_range("Layover index out of range");
        }
        return (legs[index + 1].departureTime - legs[index].arrivalTime) / 60;
    }

    // Returns total layover time across all legs in minutes
    [[nodiscard]] long long getTotalLayoverTimeMinutes() const {
        long long total = 0;
        for (int i = 0; i < static_cast<int>(legs.size()) - 1; i++) {
            total += getLayoverDurationMinutes(i);
        }
        return total;
    }

    [[nodiscard]] int getStopCount() const {
        if (legs.empty()) return 0;
        return static_cast<int>(legs.size()) - 1;
    }

    [[nodiscard]] bool isValid() const {
        if (legs.empty()) return false;

        for (int i = 0; i < static_cast<int>(legs.size()) - 1; i++) {
            // Each leg's destination must match the next leg's origin
            if (legs[i].destination.iataCode != legs[i + 1].origin.iataCode) {
                return false;
            }
            // No negative layovers (missed connections)
            if (getLayoverDurationMinutes(i) < 0) {
                return false;
            }
        }
        return true;
    }

    bool operator>(const FlightOffer& other) const {
        return getTotalPrice() > other.getTotalPrice();
    }

    [[nodiscard]] std::string toString() const {
        std::ostringstream oss;
        long long totalMins = getTotalDurationMinutes();
        oss << "=== Flight Offer ===\n";
        oss << "Class   : " << seatClassToString(selectedClass) << "\n";
        oss << "Stops   : " << getStopCount() << "\n";
        oss << "Price   : $" << getTotalPrice() << "\n";
        oss << "Duration: " << totalMins / 60 << "h " << totalMins % 60 << "m\n";
        oss << "--- Legs ---\n";
        for (const Flight& leg : legs) {
            oss << leg.toString() << "\n";
        }
        return oss.str();
    }

};

#endif //FLIGHT_OPTIMIZER_FLIGHTOFFER_H