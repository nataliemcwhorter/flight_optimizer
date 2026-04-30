//
// Created by Natalie McWhorter on 4/14/26.
//

#include "AirportTrie.h"

#include <iostream>

/**
 * Constructor: Initialize the trie with a new root node.
 */
AirportTrie::AirportTrie() {
    root = std::make_unique<TrieNode>();
}


/**
 * insert: Add an airport to the trie indexed by IATA code.
 */
void AirportTrie::insert(const Airport& airport) {
    if (airport.iataCode.length() != 3) throw std::invalid_argument("Invalid airport code");
    for (char c : airport.iataCode) {
        if (!std::isupper(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("IATA code must be all uppercase letters");
        }
    }
    TrieNode* current = root.get();
    for (char c : airport.iataCode) {
        if (!current->children.contains(c)) {
            current->children[c] = std::make_unique<TrieNode>();
        }
        current = current->children[c].get();
    }
    current->isEnd = true;
    current->airport = airport;
}

/**
 * find: Search for an exact airport by IATA code.
  */
std::optional<Airport> AirportTrie::find(const std::string& iataCode) const {
    TrieNode* current = root.get();
    for (char c : iataCode) {
        if (!current->children.contains(c)) {
            return std::nullopt;
        }
        current = current->children[c].get();
    }
    if (current->isEnd) {
        return current->airport;
    }
    return std::nullopt;
}

/**
 * search: Perform prefix-based search on IATA codes.
 */
std::vector<Airport> AirportTrie::search(const std::string& prefix) const {
    TrieNode* current = root.get();
    std::vector<Airport> results;
    for (char c : prefix) {
        if (!current->children.contains(c)) {
            return std::vector<Airport>();
        }
        current = current->children[c].get();
    }
    collectAll(current, results);
    return results;

}

/**
 * collectAll: Recursively collect all airports in the subtrie rooted at node.
 * Helper method for search().
  */
void AirportTrie::collectAll(const TrieNode* node, std::vector<Airport>& results) const {
    if (node == nullptr) {
        return;
    }
    if (node->isEnd) {
        results.push_back(node->airport.value());
    }
    for (auto& c : node->children) {
        collectAll(c.second.get(), results);
    }

}

/**
 * isEmpty: Check whether the trie is empty.
 */
bool AirportTrie::isEmpty() const {
    return root->children.empty();
}
