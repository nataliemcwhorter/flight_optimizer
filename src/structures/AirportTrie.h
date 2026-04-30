//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_AIRPORTTRIE_H
#define FLIGHT_OPTIMIZER_AIRPORTTRIE_H

#pragma once

#include <map>
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include <stdexcept>
#include "../models/Airport.h"

/**
 * TrieNode: Internal node structure for the trie.
 * Each node can have up to 26 children (one per uppercase letter A-Z).
 * Stores an optional Airport if this node marks the end of a valid IATA code.
 */
struct TrieNode {
    std::map<char, std::unique_ptr<TrieNode>> children;
    bool isEnd = false;
    std::optional<Airport> airport;  // Only populated if isEnd == true
};

/**
 * AirportTrie: A trie data structure for efficient prefix-based airport lookup by IATA code.
 *
 * Design decisions:
 * - Uses std::unique_ptr exclusively (no raw pointers)
 * - IATA codes must be exactly 3 uppercase letters (A-Z)
 * - Memory cleanup is automatic via unique_ptr (no manual destruction needed)
 */
class AirportTrie {
private:
    std::unique_ptr<TrieNode> root;

    /**
     * collectAll: Recursively collects all airports in the subtrie rooted at node.
     * Helper method used by search() to gather results after prefix matching.
     *
     * @param node Pointer to the current TrieNode being processed
     * @param results Reference to vector where airports are accumulated
     */
    void collectAll(const TrieNode* node, std::vector<Airport>& results) const;

public:
    /**
     * Constructor: Initializes an empty trie with a new root node.
     */
    AirportTrie();

    /**
     * Destructor: Defaulted. unique_ptr automatically handles cleanup of root and all descendants.
     */
    ~AirportTrie() = default;

    /**
     * insert: Adds an airport to the trie, indexed by its IATA code.
     *
     * Validation: IATA code must be exactly 3 characters, all uppercase A-Z.
     * Throws std::invalid_argument if validation fails.
     *
     * @param airport The airport to insert
     * @throws std::invalid_argument if IATA code is invalid
     */
    void insert(const Airport& airport);

    /**
     * find: Searches for an exact airport match by IATA code.
     *
     * @param iataCode The 3-letter IATA code to search for
     * @return std::optional<Airport> containing the airport if found, nullopt otherwise
     */
    std::optional<Airport> find(const std::string& iataCode) const;

    /**
     * search: Performs prefix-based search on IATA codes.
     * Returns all airports whose IATA codes begin with the given prefix.
     *
     * @param prefix The prefix to search for (e.g., "SFO", "SF", "S")
     * @return std::vector<Airport> of all matching airports (empty if no matches)
     */
    std::vector<Airport> search(const std::string& prefix) const;

    /**
     * isEmpty: Checks whether the trie contains any airports.
     *
     * @return true if no airports are stored, false otherwise
     */
    bool isEmpty() const;
};
#endif //FLIGHT_OPTIMIZER_AIRPORTTRIE_H