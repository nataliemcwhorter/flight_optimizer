//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PriceCache.h"

PriceCache::PriceCache(int capacity) : capacity(capacity) {
	// TODO: Initialize member variables if needed
}

void PriceCache::put(std::string key, double value) {
	// TODO: Implement LRU put logic:
	//   1. If key already exists:
	//      - Update its value
	//      - Remove from current position in lruList
	//      - Move to front of lruList
	//   2. If key is new and cache is not full:
	//      - Insert new CacheEntry at front of lruList
	//      - Add iterator to cacheMap
	//   3. If key is new and cache IS full:
	//      - Evict back() entry from lruList
	//      - Remove evicted key from cacheMap
	//      - Insert new entry at front
}

std::optional<double> PriceCache::get(std::string key) {
	// TODO: Implement LRU get logic:
	//   1. Check if key exists in cacheMap
	//   2. If found:
	//      - Move the entry to front of lruList (mark as recently used)
	//      - Return the value wrapped in std::optional
	//   3. If not found:
	//      - Return std::nullopt
}

bool PriceCache::contains(std::string key) {
	// TODO: Check if key exists in cacheMap
	// Return true if found, false otherwise
	return false;
}

void PriceCache::clear() {
	// TODO: Clear both lruList and cacheMap
}

int PriceCache::size() const {
	// TODO: Return the current number of entries in the cache
	return 0;
}
