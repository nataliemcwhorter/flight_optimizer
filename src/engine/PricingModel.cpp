//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PricingModel.h"

PricingModel::PricingModel(PriceCache& cache) : cache(cache) {
	// Constructor implementation
}

std::string PricingModel::makeCacheKey(const Flight& flight, SeatClass sc) const {
	// TODO: Return flight.flightNumber + "_" + std::to_string(static_cast<int>(sc))
	// This creates a unique cache key combining flight identifier and seat class
}

double PricingModel::computePrice(const Flight& flight, const Airline& airline,
								   SeatClass sc, const PricingConfig& config) {
	// TODO Step 1: Build cache key using makeCacheKey(flight, sc)
	//             Call cache.get(key) and check if value is present
	//             If present (optional has value), return it immediately

	// TODO Step 2: Calculate distance using flight.origin.distanceTo(flight.destination)

	// TODO Step 3: Retrieve seat class multiplier using airline.getMultiplier(sc)

	// TODO Step 4: Apply pricing formula:
	//             price = airline.baseRatePerKm * distance * seatMultiplier
	//                     * flight.popularityScore * config.demandFactor * config.popularityBoost

	// TODO Step 5: Store the computed price in cache using cache.put(key, price)

	// TODO Step 6: Return the computed price
}

void PricingModel::populatePrices(Flight& flight, const Airline& airline,
								   const PricingConfig& config) {
	// TODO: Use a range-based for loop to iterate over flight.availableClasses
	//       For each SeatClass sc in that vector:
	//       - Call computePrice(flight, airline, sc, config)
	//       - Store the returned price in flight.pricePerClass[sc]
	//       This populates the price map for all available seat classes on this flight
}
