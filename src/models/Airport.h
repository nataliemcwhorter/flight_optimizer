//
// Created by Natalie McWhorter on 4/14/26.
//
#ifndef FLIGHT_OPTIMIZER_AIRPORT_H
#define FLIGHT_OPTIMIZER_AIRPORT_H

#pragma once
#include <string>
#include <cmath>

struct Airport {
	// Identification
	std::string iataCode;       // ex. "RDU"
	std::string icaoCode;       // ex. "KRDU"
	int numericId;              // OurAirports numeric ID

	// Coordinates (degrees)
	double latitude;
	double longitude;

	//
	std::string name;           // ex. "Raleigh-Durham International"
	std::string city;           // ex. "Raleigh"
	std::string country;        // ex. "United States"

	// Methods
	bool operator==(const Airport& other) const {
		return iataCode == other.iataCode;
	}

	bool operator<(const Airport& other) const {
		return iataCode < other.iataCode;
	}

	[[nodiscard]] std::string toString() const {
		return iataCode + " - " + name + " (" + city + ", " + country + ")";
	}

	[[nodiscard]] double distanceTo(const Airport& other) const {
		// Haversine formula — returns distance in km
		const double R = 6371.0; // Earth's radius in km
		double lat1 = latitude * M_PI / 180.0;
		double lat2 = other.latitude * M_PI / 180.0;
		double dLat = (other.latitude - latitude) * M_PI / 180.0;
		double dLon = (other.longitude - longitude) * M_PI / 180.0;

		double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
				   std::cos(lat1) * std::cos(lat2) *
				   std::sin(dLon / 2) * std::sin(dLon / 2);

		double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
		return R * c;
	}
};

#endif //FLIGHT_OPTIMIZER_AIRPORT_H