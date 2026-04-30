#include "FlightGraph.h"

void FlightGraph::addAirport(const Airport& airport) {
	// TODO: Insert airport into airports map keyed by airport.iataCode;
	// if iataCode not already in adjacency, insert empty vector to ensure key exists
}

void FlightGraph::addFlight(const Flight& flight) {
	// TODO: Push_back flight into adjacency[flight.origin.iataCode];
	// note: addAirport should be called first for both origin and destination before addFlight
}

std::vector<Flight> FlightGraph::getFlightsFrom(const std::string& iataCode) const {
	// TODO: Look up adjacency map by iataCode;
	// if not found return empty vector {}
	return {};
}

std::optional<Airport> FlightGraph::getAirport(const std::string& iataCode) const {
	// TODO: Look up airports map by iataCode;
	// if not found return std::nullopt
	return std::nullopt;
}

bool FlightGraph::hasAirport(const std::string& iataCode) const {
	// TODO: Return airports.count(iataCode) > 0
	return false;
}

int FlightGraph::airportCount() const {
	// TODO: Return static_cast<int>(airports.size())
	return 0;
}

int FlightGraph::flightCount() const {
	// TODO: Iterate over adjacency map, accumulate sum of each vector's size(), return total as int
	return 0;
}

void FlightGraph::clear() {
	// TODO: Call airports.clear() and adjacency.clear()
}
