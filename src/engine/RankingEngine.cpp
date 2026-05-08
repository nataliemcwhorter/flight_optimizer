//
// Created by Natalie McWhorter on 4/14/26.
//

#include "RankingEngine.h"
#include <stdexcept>

RankingEngine::RankingEngine() {
    strategies["cheapest"] = [](const FlightOffer& a, const FlightOffer& b) {
        return a.getTotalPrice() < b.getTotalPrice();
    };
    strategies["fastest"] = [](const FlightOffer& a, const FlightOffer& b) {
        return a.getTotalDurationMinutes() < b.getTotalDurationMinutes();
    };
    strategies["fewest_stops"] = [](const FlightOffer& a, const FlightOffer& b) {
        return a.legs.size() < b.legs.size();
    };
    strategies["best_value"] = [](const FlightOffer& a, const FlightOffer& b) {
        double ratio_a = a.getTotalDurationMinutes() > 0
                         ? a.getTotalPrice() / a.getTotalDurationMinutes() : 0;
        double ratio_b = b.getTotalDurationMinutes() > 0
                         ? b.getTotalPrice() / b.getTotalDurationMinutes() : 0;
        return ratio_a < ratio_b;
    };
}

void RankingEngine::registerStrategy(const std::string& name,
    std::function<bool(const FlightOffer&, const FlightOffer&)> comparator) {
    strategies[name] = comparator;
}

std::vector<FlightOffer> RankingEngine::rank(std::vector<FlightOffer> offers,
                                              const std::string& strategyName) const {
    auto it = strategies.find(strategyName);
    if (it == strategies.end())
        throw std::invalid_argument("Unknown strategy: " + strategyName);
    std::sort(offers.begin(), offers.end(), it->second);
    return offers;
}

std::vector<std::string> RankingEngine::availableStrategies() const {
    std::vector<std::string> names;
    for (const auto& entry : strategies)
        names.push_back(entry.first);
    return names;
}