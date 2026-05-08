#include "FlightGraph.h"

void FlightGraph::addAirport(const Airport& airport) {
	airports.insert({airport.iataCode, airport});
	if (!airport.icaoCode.empty() && airport.icaoCode != "\\N")
		icaoToIata[airport.icaoCode] = airport.iataCode;
	auto it = adjacency.find(airport.iataCode);
	if (it == adjacency.end())
		adjacency.insert({airport.iataCode, {}});
}

void FlightGraph::addFlight(const Flight& flight) {
	adjacency[flight.origin.iataCode].push_back(flight);
}

std::vector<Flight> FlightGraph::getFlightsFrom(const std::string& iataCode) const {
	auto it = adjacency.find(iataCode);
	if (it == adjacency.end()) return {};
	return it->second;
}

std::optional<Airport> FlightGraph::getAirport(const std::string& iataCode) const {
	auto it = airports.find(iataCode);
	if (it != airports.end()) return it->second;
	return std::nullopt;
}

std::optional<Airport> FlightGraph::getAirportByIcao(const std::string& icaoCode) const {
	auto it = icaoToIata.find(icaoCode);
	if (it == icaoToIata.end()) return std::nullopt;
	return getAirport(it->second);
}

bool FlightGraph::hasAirport(const std::string& iataCode) const {
	return airports.contains(iataCode);
}

int FlightGraph::airportCount() const {
	return static_cast<int>(airports.size());
}

int FlightGraph::flightCount() const {
	int count = 0;
	for (const auto& pair : adjacency)
		count += static_cast<int>(pair.second.size());
	return count;
}

void FlightGraph::clear() {
	airports.clear();
	icaoToIata.clear();
	adjacency.clear();
}