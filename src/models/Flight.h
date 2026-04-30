//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_FLIGHT_H
#define FLIGHT_OPTIMIZER_FLIGHT_H
#include <algorithm>

#include "Airport.h"
#include "UserQuery.h"

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <chrono>
#include <ranges>


struct Flight {
	//Identification
	std::string flightNumber;
	std::string airlineCode;

	//Timing
	long long departureTime;
	long long arrivalTime;

	//Route
	Airport origin;
	Airport destination;

	//Availability
	std::vector<SeatClass> availableClasses;
	std::map<SeatClass, double> pricePerClass;
	std::string liveStatus;

	double popularityScore;


	[[nodiscard]] long long getDuration() const {
		return arrivalTime-departureTime;
	}

	[[nodiscard]] std::optional<double> getPrice(SeatClass seat) const{
		auto it = pricePerClass.find(seat);
		if (it != pricePerClass.end()) return it->second;
		return std::nullopt;
	}

	[[nodiscard]] bool isAvailable(SeatClass seat) const {
		return std::ranges::contains(availableClasses, seat);
	}

	[[nodiscard]] std::string toString() const {
		return "[" + flightNumber + "] " + airlineCode +
			   "\n  From: " + origin.toString() +
			   "\n  To: " + destination.toString() +
			   "\n  Departure Time: " + timeToString(departureTime) +
			   "\n  Arrival Time: " + timeToString(arrivalTime) +
			   "\n  Status: " + liveStatus;
	}

	static std::string timeToString(long long unixTime) {
		auto t = static_cast<std::time_t>(unixTime);
		std::string s(std::ctime(&t));
		s.erase(s.find('\n'));
		return s;
	}


};

#endif //FLIGHT_OPTIMIZER_FLIGHT_H