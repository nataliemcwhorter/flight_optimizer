//
// Created by Natalie McWhorter on 4/14/26.
//

#include "AirportIndex.h"

AirportIndex::AirportIndex(FlightGraph& graph)
	: graph(graph) {
	// TODO: Initialize graph reference via member initializer list
	// TODO: trie is default-constructed
}

void AirportIndex::indexAirport(const Airport& airport) {
	// TODO: Call trie.insert(airport)
	// TODO: Call graph.addAirport(airport)
}

std::optional<Airport> AirportIndex::findByCode(const std::string& iataCode) const {
	// TODO: Delegate to trie.find(iataCode)
}

std::vector<Airport> AirportIndex::searchByPrefix(const std::string& prefix) const {
	// TODO: Delegate to trie.search(prefix)
}

bool AirportIndex::isEmpty() const {
	// TODO: Delegate to trie.isEmpty()
}