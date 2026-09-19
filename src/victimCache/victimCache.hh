#ifndef __VICTIMCACHE__
#define __VICTIMCACHE__

#include <iostream>
#include <cassert>
#include "../common.hh"
#include "../cache/cache.hh"

#define VICTIM_CACHE_SIZE 8 // Number of lines in the victim cache

struct VictimCacheElement{
    CacheElement cacheLine;
    uint64_t index; // Index in direct mapped cache where this line was evicted from
    VictimCacheElement() : index(0) {}
};
class VictimCache {
    VictimCacheElement table[VICTIM_CACHE_SIZE]; // 8 lines in the victim cache
    int nextEvictIndex; // Index to track the next line to evict (for FIFO replacement)
    int hitCount;
    int missCount;
    int evictCount;
    int totalTransactions;

    public:
    VictimCache();
    bool isCacheHit(uint64_t tag, uint64_t index); // Check if the tag is present in the victim cache
    void placeCacheLine(CacheElement c, uint64_t index); // Place a cache line into the victim cache
    CacheElement getEvictCacheLine(); // Get the cache line to be evicted from the victim cache
    void evictCacheLine(); // Evict a cache line from the victim cache (FIFO)
    void printValidCache(); // Print valid entries in the victim cache
    void printStats() const; // Print statistics of the victim cache
    CacheElement getCacheLine(uint64_t tag, uint64_t index); // Retrieve a cache line by its tag and index from the victim cache
    void removeCacheLine(uint64_t tag, uint64_t index); // Remove a cache line by its tag and index from the victim cache
    int getCacheSize() const { return VICTIM_CACHE_SIZE; } // Get the size of the victim cache
    int getValidCacheLineCount() const; // Get the count of valid cache lines in the victim cache
};
#endif