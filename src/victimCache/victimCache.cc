#include "victimCache.hh"

VictimCache::VictimCache() : nextEvictIndex(0), hitCount(0), missCount(0), totalTransactions(0), evictCount(0) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        table[i].valid = false; // Initialize all lines as invalid
    }
}

bool VictimCache::isCacheHit(unsigned int tag) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].valid && table[i].tag == tag) {
            hitCount++;
            totalTransactions++;
            return true; // Cache hit
        }
    }
    missCount++;
    totalTransactions++;
    return false; // Cache miss
}

void VictimCache::placeCacheLine(CacheElement c) {
    // Place the cache line in the next available slot or evict if full
    evictCacheLine(); // Evict the next line if needed
    table[nextEvictIndex] = c; // Place the new cache line
    std::cout << "VictimCache: Placed line with tag: 0x" << std::hex << c.tag << " at index " << nextEvictIndex << "\n";
    nextEvictIndex = (nextEvictIndex + 1) % VICTIM_CACHE_SIZE; // Update the eviction index (FIFO)
}

CacheElement VictimCache::getEvictCacheLine() {
    CacheElement evictedLine = table[nextEvictIndex];
    table[nextEvictIndex].valid = false; // Mark the line as invalid
    nextEvictIndex = (nextEvictIndex + 1) % VICTIM_CACHE_SIZE; // Update the eviction index (FIFO)
    return evictedLine;
}

void VictimCache::evictCacheLine() {
    if (table[nextEvictIndex].valid) {
        std::cout << "VictimCache: Evicting line at index " << nextEvictIndex << " with tag: 0x" << std::hex << table[nextEvictIndex].tag << "\n";
        table[nextEvictIndex].valid = false; // Mark the line as invalid
        evictCount += 1;
    }
}

void VictimCache::printValidCache() {
    std::cout << "Victim Cache Contents:\n";
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].valid) {
            std::cout << "Index: " << i << ", Tag: 0x" << std::hex << table[i].tag << ", Addr: 0x" << std::hex << table[i].addr << ", Dirty: " << std::boolalpha << table[i].dirty << "\n";
        }
    }
}

CacheElement VictimCache::getCacheLineByTag(unsigned int tag) {
        for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
            if (table[i].valid && table[i].tag == tag) {
                return table[i]; // Return the cache line if found
            }
        }
        return CacheElement(); // Return an invalid cache line if not found
}

void VictimCache::removeCacheLineByTag(unsigned int tag) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].valid && table[i].tag == tag) {
            table[i].valid = false; // Mark the line as invalid
            std::cout << "VictimCache: Removed line with tag: 0x" << std::hex << tag << " from index " << i << "\n";
            return;
        }
    }
    std::cout << "VictimCache: No line found with tag: 0x" << std::hex << tag << " to remove.\n";
}

void VictimCache::printStats() const {
    std::cout << "Victim Cache Statistics:\n";
    std::cout << "Total Victim Cache Transactions: " << totalTransactions << "\n";
    std::cout << "Cache Hits: " << hitCount << "\n";
    std::cout << "Cache Misses: " << missCount << "\n";
    std::cout << "Evictions: " << evictCount << "\n";
}

int VictimCache::getValidCacheLineCount() const {
    int count = 0;
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].valid) {
            count++;
        }
    }
    return count;
}