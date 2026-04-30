//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_PRICECACHE_H
#define FLIGHT_OPTIMIZER_PRICECACHE_H

#include <list>
#include <unordered_map>
#include <optional>
#include <string>

/**
 * @brief LRU (Least Recently Used) cache for computed flight prices.
 *
 * Key: "flightNumber_seatClassAsInt" (e.g., "AA123_2" for BUSINESS seat)
 * Value: double (computed price for that flight/seat combination)
 *
 * When cache is full, the least recently used entry is evicted.
 * On get() hit, the accessed entry moves to the front (most recently used).
 */

struct CacheEntry {
    std::string key;
    double value;
};

class PriceCache {
private:
    int capacity;
    std::list<CacheEntry> lruList;  // Front = most recently used, back = least recently used
    std::unordered_map<std::string, std::list<CacheEntry>::iterator> cacheMap;

public:
    /**
     * @brief Construct a PriceCache with specified capacity.
     * @param capacity Maximum number of entries to hold before evicting LRU entry.
     */
    explicit PriceCache(int capacity);

    /**
     * @brief Insert or update a cache entry.
     * If key already exists, update its value and move to front.
     * If cache is full and key is new, evict the back (LRU) entry first.
     * @param key Cache key (flightNumber_seatClass).
     * @param value Computed price.
     */
    void put(std::string key, double value);

    /**
     * @brief Retrieve a cached value by key.
     * On cache hit, move the entry to front (mark as recently used).
     * @param key Cache key to look up.
     * @return std::optional<double> containing the price if found, std::nullopt otherwise.
     */
    std::optional<double> get(std::string key);

    /**
     * @brief Check if a key exists in the cache without updating LRU state.
     * @param key Cache key to check.
     * @return true if key is in cache, false otherwise.
     */
    bool contains(std::string key);

    /**
     * @brief Clear all entries from the cache.
     */
    void clear();

    /**
     * @brief Get the current number of entries in cache.
     * @return Number of entries currently stored.
     */
    int size() const;
};



#endif //FLIGHT_OPTIMIZER_PRICECACHE_H