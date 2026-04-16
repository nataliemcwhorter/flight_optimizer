//
// Created by Natalie McWhorter on 4/14/26.
//
#ifndef FLIGHT_OPTIMIZER_USERQUERY_H
#define FLIGHT_OPTIMIZER_USERQUERY_H
#endif //FLIGHT_OPTIMIZER_USERQUERY_H

#pragma once
#include <string>

enum class SeatClass {
	ECONOMY,
	PREMIUM_ECONOMY,
	BUSINESS,
	FIRST
};

enum class Priority {
	CHEAPEST,
	FASTEST,
	FEWEST_STOPS,
	BALANCED   // weighted blend
};

struct UserQuery {
	std::string origin;          // IATA code e.g. "RDU"
	std::string destination;     // IATA code e.g. "LAX"
	std::string departDate;      // e.g. "2025-06-15"
	std::string returnDate;      // empty string if one-way
	SeatClass seatClass;
	Priority priority;
	int maxStops;
	double budgetCap;            // max total price, 0.0 = no limit
	int maxDurationMinutes;      // max total travel time, 0 = no limit
};
