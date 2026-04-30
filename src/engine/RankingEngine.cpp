//
// Created by Natalie McWhorter on 4/14/26.
//

#include "RankingEngine.h"

// Constructor: Register all 4 built-in ranking strategies.
// Each strategy is a lambda comparator that returns true if offer 'a' should come before offer 'b'.
// The comparators are stored in the strategies map keyed by strategy name.
RankingEngine::RankingEngine() {
    // TODO "cheapest": Register a lambda that compares offers by total price (ascending).
    // Comparator should return true if a.getTotalPrice() < b.getTotalPrice()


    // TODO "fastest": Register a lambda that compares offers by total flight duration (ascending).
    // Total duration = sum of getDuration() across all legs in the offer.
    // Comparator should return true if duration_a < duration_b


    // TODO "fewest_stops": Register a lambda that compares offers by number of stops (ascending).
    // Number of stops = number of legs - 1. Compare using a.legs.size() < b.legs.size()


    // TODO "best_value": Register a lambda that compares offers by price-to-duration ratio (ascending).
    // Ratio = total price / total duration. Guard against zero duration to avoid division by zero.
    // Comparator should return true if ratio_a < ratio_b

}

// Register a custom ranking strategy.
//
// Parameters:
//   name - unique identifier for this strategy
//   comparator - lambda or function object that defines the sort order (returns true if a < b)
void RankingEngine::registerStrategy(const std::string& name, std::function<bool(const FlightOffer&, const FlightOffer&)> comparator) {
    // TODO: Insert the comparator into the strategies map with key 'name'.
    // strategies[name] = comparator;
}

// Rank a list of flight offers using the specified strategy.
//
// Parameters:
//   offers - vector of FlightOffer objects to rank
//   strategyName - name of the ranking strategy to apply
//
// Returns:
//   Sorted vector of FlightOffer objects in the order specified by the strategy
//
// Throws:
//   std::invalid_argument if strategyName is not found in the strategies map
std::vector<FlightOffer> RankingEngine::rank(std::vector<FlightOffer> offers, const std::string& strategyName) const {
    // TODO Step 1: Look up strategyName in the strategies map.

    // TODO Step 2: If strategyName is not found, throw std::invalid_argument with message:
    // "Unknown strategy: " + strategyName

    // TODO Step 3: Apply std::sort to the offers vector using the retrieved comparator.
    // std::sort(offers.begin(), offers.end(), comparator);

    // TODO Step 4: Return the sorted offers vector.
}

// Get a list of all available ranking strategy names.
//
// Returns:
//   Vector of strategy names currently registered in this engine
std::vector<std::string> RankingEngine::availableStrategies() const {
    // TODO: Create an empty vector<string>.
    // Iterate through the strategies map (strategies.begin() to strategies.end()).
    // For each entry, push the key (first) into the vector.
    // Return the populated vector.
}