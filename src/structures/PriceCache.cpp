//
// Created by Natalie McWhorter on 4/14/26.
//

#include "PriceCache.h"
using namespace std;

PriceCache::PriceCache(int capacity) : capacity(capacity) {
}

void PriceCache::put(std::string key, double value) {
	auto it = cacheMap.find(key);
	if (it != cacheMap.end()) {
		lruList.splice(lruList.begin(), lruList, it->second);
		it->second->value = value;
		return;
	}
	else {
		if (cacheMap.size() >= capacity) {
			cacheMap.erase(lruList.back().key);
			lruList.pop_back();
		}
	}
	lruList.push_front({key, value});
	cacheMap[key] = lruList.begin();
}

std::optional<double> PriceCache::get(std::string key) {
	auto it = cacheMap.find(key);
	if (it != cacheMap.end()) {
		lruList.splice(lruList.begin(), lruList, it->second);
		return it->second->value;
	}
	else {
		return std::nullopt;
	}
}

bool PriceCache::contains(std::string key) {
	return cacheMap.contains(key);
}

void PriceCache::clear() {
	lruList.clear();
	cacheMap.clear();
}

int PriceCache::size() const {
	return cacheMap.size();
}
