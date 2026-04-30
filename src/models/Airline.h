//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_AIRLINE_H
#define FLIGHT_OPTIMIZER_AIRLINE_H

#include <string>
#include <map>
#include "UserQuery.h"

struct Airline {
	// Identification
	std::string iataCode;
	std::string icaoCode;
	std::string name;
	std::string country;
	bool isActive;

	//stuff for pricing model
	double baseRatePerKm = 0.10;
	std::map<SeatClass, double> classMultipliers;
	double demandSensitivity = 0.5;
	double popularityWeight = 0.5;

	[[nodiscard]] double getMultiplier(SeatClass seat) const {
		if (auto multiplier = classMultipliers.find(seat); multiplier != classMultipliers.end()) {
			return multiplier->second;
		}
		return 1.0;
	}

	[[nodiscard]] std::string toString() const {
		return iataCode + " - " + name + " (" + country + ")";
	}

	bool operator==(const Airline& other) const {
		return iataCode == other.iataCode;
	}

	bool operator<(const Airline& other) const {
		return iataCode < other.iataCode;
	}


};

#endif //FLIGHT_OPTIMIZER_AIRLINE_H