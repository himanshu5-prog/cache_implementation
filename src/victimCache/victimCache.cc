#include "victimCache.hh"

VictimCache::VictimCache() : nextEvictIndex(0), hitCount(0), missCount(0), totalTransactions(0), evictCount(0) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
       table[i].cacheLine.valid = false; // Initialize all cache lines as invalid
       table[i].index = 0; // Initialize index to 0 (no associated index
    }
}

bool VictimCache::isCacheHit(uint64_t tag, uint64_t index) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].cacheLine.valid && table[i].cacheLine.tag == tag && table[i].index == index) {
            hitCount++;
            totalTransactions++;
            return true; // Cache hit
        }
    }
    missCount++;
    totalTransactions++;
    return false; // Cache miss
}

void VictimCache::placeCacheLine(CacheElement c, uint64_t index) {
    // Place the cache line in the next available slot or evict if full
    evictCacheLine(); // Evict the next line if needed
    table[nextEvictIndex].cacheLine = c; // Place the new cache line
    table[nextEvictIndex].index = index; // Set the associated index
    table[nextEvictIndex].cacheLine.dirty = false; // Reset dirty bit for the new line
    std::cout << "VictimCache: Placed line with direct-mapped cache index: " << index << ", tag: 0x" << std::hex << c.tag << " at index " << nextEvictIndex << "\n";
    nextEvictIndex = (nextEvictIndex + 1) % VICTIM_CACHE_SIZE; // Update the eviction index (FIFO)
}

CacheElement VictimCache::getEvictCacheLine() {
    CacheElement evictedLine = table[nextEvictIndex].cacheLine;
    table[nextEvictIndex].cacheLine.valid = false; // Mark the line as invalid
    table[nextEvictIndex].index = -1; // Clear the associated index
    return evictedLine;
}

void VictimCache::evictCacheLine() {
    if (table[nextEvictIndex].cacheLine.valid) {
        std::cout << "VictimCache: Evicting line at index " << nextEvictIndex << " with address: 0x" << std::hex << table[nextEvictIndex].cacheLine.addr << std::dec << ", Direct-Mapped Index: " << table[nextEvictIndex].index << "\n";
        table[nextEvictIndex].cacheLine.valid = false; // Mark the line as invalid
        evictCount += 1;
    }
}

void VictimCache::printValidCache() {
    std::cout << "Victim Cache Contents:\n";
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].cacheLine.valid) {
            std::cout << "Index: " << i << ", Tag: 0x" << std::hex << table[i].cacheLine.tag  << ", Direct-Mapped Index: " << table[i].index << ", Dirty: " << std::boolalpha << table[i].cacheLine.dirty << "\n";
        }
    }
}

CacheElement VictimCache::getCacheLine(uint64_t tag, uint64_t index) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].cacheLine.valid && table[i].cacheLine.tag == tag && table[i].index == index) {
            return table[i].cacheLine; // Return the cache line if found
        }
    }
    return CacheElement(); // Return an invalid cache line if not found
}

void VictimCache::removeCacheLine(uint64_t tag, uint64_t index) {
    for (int i = 0; i < VICTIM_CACHE_SIZE; ++i) {
        if (table[i].cacheLine.valid && table[i].cacheLine.tag == tag && table[i].index == index) {
            table[i].cacheLine.valid = false; // Mark the line as invalid
        std::cout << "VictimCache: Removed line with tag: 0x" << std::hex << tag << std::dec << " and direct-mapped index: " << index << " from index " << i << "\n";
            return;
        }
    }
    std::cout << "VictimCache: No line found with tag: 0x" << std::hex << tag << std::dec << " and direct-mapped index: " << index << " to remove.\n";
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
        if (table[i].cacheLine.valid) {
            count++;
        }
    }
    return count;
}