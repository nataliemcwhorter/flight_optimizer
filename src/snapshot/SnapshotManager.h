//
// Created by Natalie McWhorter on 4/14/26.
//

#ifndef FLIGHT_OPTIMIZER_SNAPSHOTMANAGER_H
#define FLIGHT_OPTIMIZER_SNAPSHOTMANAGER_H

#include <string>
#include <filesystem>
#include "../snapshot/CachePolicy.h"

/// Manages saving and loading API JSON responses as local snapshots files.
/// Snapshots are stored in a configurable directory (default: data/snapshots/).
/// File names are derived from cache keys. Uses CachePolicy to determine whether
/// a snapshots is still fresh.
class SnapshotManager {
private:
    /// Directory where snapshots files are stored.
    std::filesystem::path snapshotDir;

    /// Cache expiry policy.
    CachePolicy policy;

    /// Converts a cache key to a full file path.
    /// Replaces any '/' in key with '_' to ensure valid filename.
    /// Returns snapshotDir / (key + '.json')
    std::filesystem::path keyToPath(const std::string& key) const;

public:
    /// Constructor. Creates the snapshots directory if it does not exist.
    /// @param dir Directory path for storing snapshots (default: "data/snapshots")
    /// @param policy CachePolicy determining snapshots freshness (default: 1 hour TTL)
    explicit SnapshotManager(const std::string& dir = "data/snapshots",
                             const CachePolicy& policy = {});

    /// Returns true if a snapshots file exists for this key AND it is not stale
    /// according to the CachePolicy.
    /// @param key Cache key (e.g., "flights_JFK_LAX")
    /// @return true if snapshots is fresh and available, false otherwise
    bool hasSnapshot(const std::string& key) const;

    /// Loads and returns the raw JSON string from the snapshots file.
    /// Throws std::runtime_error if file does not exist.
    /// @param key Cache key
    /// @return Raw JSON string contents
    std::string loadSnapshot(const std::string& key) const;

    /// Writes the raw JSON string to a snapshots file, overwriting any existing file.
    /// @param key Cache key
    /// @param json Raw JSON string to save
    void saveSnapshot(const std::string& key, const std::string& json);

    /// Deletes all snapshots files in the snapshots directory.
    void clearAll();

    /// Returns the number of snapshots files currently in the snapshots directory.
    int snapshotCount() const;
};

#endif //FLIGHT_OPTIMIZER_SNAPSHOTMANAGER_H