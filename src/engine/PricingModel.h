//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_PRICINGMODEL_H
#define FLIGHT_OPTIMIZER_PRICINGMODEL_H

#include <string>
#include "../models/Flight.h"
#include "../models/Airline.h"
#include "../models/UserQuery.h"
#include "../structures/PriceCache.h"

/**
 * Configuration parameters for pricing calculations.
 */
struct PricingConfig {
    double demandFactor = 1.0;      ///< Multiplier for demand-based pricing adjustments
    double popularityBoost = 1.0;   ///< Multiplier for popularity-based price increases
};

/**
 * Computes realistic flight prices using a multi-factor formula.
 * Formula: P = B × D × S × R × T
 *   - B = airline.baseRatePerKm
 *   - D = distance in km (flight.origin.distanceTo(flight.destination))
 *   - S = airline.getMultiplier(seatClass)
 *   - R = flight.popularityScore
 *   - T = config.demandFactor × config.popularityBoost
 * Results are cached in PriceCache to avoid recomputation for repeated queries.
 */
class PricingModel {
private:
    PriceCache& cache;

    /**
     * Builds a unique cache key for a flight-seatClass combination.
     * Key format: "flightNumber_seatClassInt" (e.g., "AA101_2" for BUSINESS).
     * @param flight The flight to key.
     * @param sc The seat class to key.
     * @return A unique cache key string.
     */
    std::string makeCacheKey(const Flight& flight, SeatClass sc) const;

public:
    /**
     * Constructor that initializes the PricingModel with a reference to the price cache.
     * @param cache Reference to the PriceCache for storing and retrieving computed prices.
     */
    explicit PricingModel(PriceCache& cache);

    /**
     * Computes the price for a flight in a given seat class.
     * Checks cache first; if not found, computes using the pricing formula and caches the result.
     * @param flight The flight to price.
     * @param airline The airline operating the flight (provides base rate and class multipliers).
     * @param sc The seat class for which to compute price.
     * @param config Optional pricing configuration (default PricingConfig used if not provided).
     * @return The computed price as a double.
     */
    double computePrice(const Flight& flight, const Airline& airline, SeatClass sc, const PricingConfig& config = {});

    /**
     * Populates all available seat classes in a flight's pricePerClass map.
     * Calls computePrice for each SeatClass in flight.availableClasses and stores results.
     * @param flight Reference to the flight to populate (modified in-place).
     * @param airline The airline operating the flight.
     * @param config Optional pricing configuration (default used if not provided).
     */
    void populatePrices(Flight& flight, const Airline& airline, const PricingConfig& config = {});
};

#endif //FLIGHT_OPTIMIZER_PRICINGMODEL_H