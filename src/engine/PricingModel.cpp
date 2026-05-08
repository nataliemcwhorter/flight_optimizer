//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PricingModel.h"
#include <ctime>
#include <algorithm>
using namespace std;

PricingModel::PricingModel(PriceCache& cache) : cache(cache) {
	// Constructor implementation
}

std::string PricingModel::makeCacheKey(const Flight& flight, SeatClass sc) const {
	return flight.flightNumber + "_" + std::to_string(static_cast<int>(sc));
}

double PricingModel::computePrice(const Flight& flight, const Airline& airline,
								   SeatClass sc, const PricingConfig& config) {
	string c_key = makeCacheKey(flight, sc);
	auto cached = cache.get(c_key);
	if (cached.has_value()) {return cached.value();}

	double distance = flight.origin.distanceTo(flight.destination);
	double seatMultiplier = airline.getMultiplier(sc);

	// Base price calculation
	double basePrice = airline.baseRatePerKm * distance;

	// Distance-based pricing tiers (longer flights have better per-km rates)
	double distanceFactor = 1.0;
	if (distance < 500) {
		distanceFactor = 1.3;  // Short-haul premium
	} else if (distance < 2000) {
		distanceFactor = 1.0;  // Medium-haul baseline
	} else {
		distanceFactor = 0.85; // Long-haul discount per km
	}

	// Seat class premium adjustments (beyond base multiplier)
	double classPremium = 1.0;
	switch (sc) {
		case SeatClass::ECONOMY:
			classPremium = 1.0;
			break;
		case SeatClass::PREMIUM_ECONOMY:
			classPremium = 1.4;  // 40% premium over economy
			break;
		case SeatClass::BUSINESS:
			classPremium = 2.5;  // 150% premium over economy
			break;
		case SeatClass::FIRST:
			classPremium = 4.0;  // 300% premium over economy
			break;
	}

	// Time-based pricing (flights departing in peak hours cost more)
	double timeFactor = 1.0;
	if (flight.departureTime > 0) {
		time_t t = static_cast<time_t>(flight.departureTime);
		struct tm* tm_info = std::localtime(&t);
		int hour = tm_info->tm_hour;

		// Peak hours: 6-9 AM, 5-8 PM
		if ((hour >= 6 && hour <= 9) || (hour >= 17 && hour <= 20)) {
			timeFactor = 1.25;  // 25% peak hour premium
		}
	}

	// Compute final price with all factors
	double price = basePrice
		* distanceFactor
		* seatMultiplier
		* classPremium
		* timeFactor
		* flight.popularityScore
		* config.demandFactor
		* config.popularityBoost;

	// Add minimum base fare to prevent unrealistically low prices
	double minFare = 50.0;
	if (sc == SeatClass::BUSINESS) minFare = 150.0;
	if (sc == SeatClass::FIRST) minFare = 300.0;
	price = std::max(price, minFare);

	cache.put(c_key, price);
	return price;
};

void PricingModel::populatePrices(Flight& flight, const Airline& airline,
								   const PricingConfig& config) {
	for (SeatClass sc : flight.availableClasses) {
		flight.pricePerClass[sc] = computePrice(flight, airline, sc, config);
	}
}
