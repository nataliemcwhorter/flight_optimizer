//
// Created by Natalie McWhorter on 4/14/26.
//

#include "SnapshotManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <algorithm>

SnapshotManager::SnapshotManager(const std::string& dir, const CachePolicy& policy)
    : snapshotDir(dir), policy(policy)
{
    if (!std::filesystem::exists(snapshotDir))
        std::filesystem::create_directories(snapshotDir);
}

std::filesystem::path SnapshotManager::keyToPath(const std::string& key) const
{
    std::string sanitized = key;
    std::replace(sanitized.begin(), sanitized.end(), '/', '_');
    return snapshotDir / (sanitized + ".json");
}

bool SnapshotManager::hasSnapshot(const std::string& key) const
{
    auto path = keyToPath(key);
    if (!std::filesystem::exists(path)) return false;
    if (policy.forceRefresh) return false;

    auto lastWrite = std::filesystem::last_write_time(path);
    auto age = std::filesystem::file_time_type::clock::now() - lastWrite;
    auto ageSeconds = std::chrono::duration_cast<std::chrono::seconds>(age).count();

    return ageSeconds < policy.ttlSeconds;
}

std::string SnapshotManager::loadSnapshot(const std::string& key) const
{
    auto path = keyToPath(key);
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Snapshot not found: " + key);

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void SnapshotManager::saveSnapshot(const std::string& key, const std::string& json)
{
    auto path = keyToPath(key);
    std::ofstream file(path, std::ios::trunc);
    if (!file.is_open())
        throw std::runtime_error("Could not write snapshots: " + key);

    file << json;
}

void SnapshotManager::clearAll()
{
    for (const auto& entry : std::filesystem::directory_iterator(snapshotDir)) {
        if (entry.path().extension() == ".json")
            std::filesystem::remove(entry.path());
    }
}

int SnapshotManager::snapshotCount() const
{
    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(snapshotDir)) {
        if (entry.path().extension() == ".json")
            count++;
    }
    return count;
}