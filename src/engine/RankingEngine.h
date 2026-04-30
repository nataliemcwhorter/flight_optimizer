//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_RANKINGENGINE_H
#define FLIGHT_OPTIMIZER_RANKINGENGINE_H

#include <map>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include "../models/FlightOffer.h"

/**
 * @class RankingEngine
 * @brief Implements strategy pattern for ranking FlightOffers.
 *
 * Provides built-in ranking strategies (cheapest, fastest, fewest_stops, best_value)
 * and allows registration of custom ranking strategies at runtime.
 */
class RankingEngine {
private:
    /**
     * @brief Map of strategy names to comparator functions.
     *
     * Each strategy is a comparator function that takes two FlightOffers
     * and returns true if the first offer should come before the second
     * in the sorted order.
     */
    std::map<std::string, std::function<bool(const FlightOffer&, const FlightOffer&)>> strategies;

public:
    /**
     * @brief Constructor for RankingEngine.
     *
     * Initializes the engine with four built-in ranking strategies.
     */
    RankingEngine();

    /**
     * @brief Registers a custom ranking strategy.
     *
     * @param name The unique name identifier for this strategy
     * @param comparator A function that compares two FlightOffers,
     *                   returning true if first should precede second
     */
    void registerStrategy(const std::string& name,
                         std::function<bool(const FlightOffer&, const FlightOffer&)> comparator);

    /**
     * @brief Ranks a list of FlightOffers using the specified strategy.
     *
     * @param offers A vector of FlightOffers to rank
     * @param strategyName The name of the ranking strategy to apply
     * @return A sorted copy of the offers according to the strategy
     * @throws std::invalid_argument if strategyName is not registered
     */
    std::vector<FlightOffer> rank(std::vector<FlightOffer> offers,
                                   const std::string& strategyName) const;

    /**
     * @brief Returns all available strategy names.
     *
     * @return Vector of all registered strategy names
     */
    std::vector<std::string> availableStrategies() const;
};

#endif //FLIGHT_OPTIMIZER_RANKINGENGINE_H