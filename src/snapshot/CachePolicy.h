//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_CACHEPOLICY_H
#define FLIGHT_OPTIMIZER_CACHEPOLICY_H

#include <chrono>
#include <climits>

/// Defines cache expiry policy for snapshots storage.
/// Used by SnapshotManager to determine whether a cached API response is still valid.
struct CachePolicy {
	/// Time-to-live in seconds. Snapshots older than this are considered stale.
	/// Default: 1 hour (3600 seconds).
	int ttlSeconds = 3600;

	/// If true, always fetch fresh data and overwrite snapshots regardless of age.
	bool forceRefresh = false;

	/// Returns a CachePolicy with ttlSeconds = INT_MAX (snapshots never expires).
	/// Useful for static data like airport CSVs.
	static CachePolicy neverExpire() {
		return CachePolicy{INT_MAX, false};
	}

	/// Returns a CachePolicy with forceRefresh = true.
	static CachePolicy alwaysRefresh() {
		return CachePolicy{0, true};
	}

	/// Returns a CachePolicy with ttlSeconds = 300 (5 minutes).
	/// Useful for live flight data.
	static CachePolicy shortLived() {
		return CachePolicy{300, false};
	}
};

#endif //FLIGHT_OPTIMIZER_CACHEPOLICY_H