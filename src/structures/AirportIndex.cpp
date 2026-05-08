//
// Created by Natalie McWhorter on 4/14/26.
//

#include "AirportIndex.h"

AirportIndex::AirportIndex(FlightGraph& graph)
	: graph(graph) {
}

void AirportIndex::indexAirport(const Airport& airport) {
	trie.insert(airport);
	graph.addAirport(airport);

}

std::optional<Airport> AirportIndex::findByCode(const std::string& iataCode) const {
	return trie.find(iataCode);
}

std::vector<Airport> AirportIndex::searchByPrefix(const std::string& prefix) const {
	return trie.search(prefix);
}

bool AirportIndex::isEmpty() const {
	return trie.isEmpty();
}