#include "eightWayCache.hh"

EightWayCache :: EightWayCache() {
    indexBitCount = 7; // 128 lines, so 10 bits for index
    indexMask = 0x7F; // 7 bits mask for index
    debug = false;
    replacePolicy = LRU; // Default replacement policy
    srand(time(0)); // Seed for random number generator
   
    for (auto &row : RRPV) {
        row.fill(3); // Initialize all RRPV values to 3
    }

    std::cout << "EightWayCache::Constructor : OffsetBitCount: " << offsetBitCount << ", indexMask: " << std::hex << indexMask 
        << ", indexBitCount:" << std::dec << indexBitCount << "\n";
}

int EightWayCache :: isCacheHit() {
    int index = currentAddr.index;
    int tag = currentAddr.tag;

    assert(index < TOTAL_LINE_COUNT / NUMBER_OF_WAYS);
    for (int i = 0; i < NUMBER_OF_WAYS; ++i) {
        if (table[index][i].valid) {
            if (tag == table[index][i].tag) {
                if (debug) {
                    std::cout  << std::dec << getCycleTime()<< ":: isCacheHit: Cache hit for addr: 0x" << std::hex << currentAddr.value << " in way: " << std::dec << i << "\n";
                }
                return i;
            }
        }
    }
    return -1;
}

int EightWayCache :: getEmptySlot() {
    int index = currentAddr.index;
    assert(index < TOTAL_LINE_COUNT / NUMBER_OF_WAYS);
    for (int i = 0; i < NUMBER_OF_WAYS; ++i) {
        if (!table[index][i].valid) { // Check if the cache line is not valid
            if (debug) {
                std::cout << std::dec << getCycleTime() << ":: getEmptySlot: Found empty slot in way: " << std::dec << i << " for addr: 0x" << std::hex << currentAddr.value << ", index: " << std :: dec << index <<"\n";
            }
            return i;
        }
    }
    return -1; // No empty slot found
}

int EightWayCache :: getEvictCacheLineId() {
    
    switch(replacePolicy) {
        case LRU:
            return getEvictLRU(currentAddr.index);
        case RANDOM:
            return getEvictRandom(currentAddr.index);
        case SRRIP:
            return getEvictSRRIP(currentAddr.index);
        default:
            std::cerr << "getEvictCacheLineId: Unsupported replacement policy. Defaulting to LRU.\n";
            return getEvictLRU(currentAddr.index);
    }
}

int EightWayCache :: getEvictLRU(int index) {
    // This function can be used if we want to explicitly call LRU eviction
    //int index = currentAddr.index;
    bool flag = false;
    int t;
    int id;

    // Least recently used Eviction policy is getting used
    for (int i = 0; i < NUMBER_OF_WAYS; ++i) {
        assert(table[index][i].valid);

        if (!flag) {
            t = table[index][i].updateTime;
            flag = true;
            id = i;
        } else {
            if (t > table[index][i].updateTime) {
                t = table[index][i].updateTime;
                id = i;
            }
        }
    }

    if (debug) {
        std::cout << std::dec << getCycleTime() << ":: getEvictLRU: Evicting way: " << std::dec << id << " for addr: 0x" << std::hex << currentAddr.value << "\n";
    }
    return id;
}

int EightWayCache :: getEvictRandom(int index) {
    int wayId = rand() % NUMBER_OF_WAYS; // Randomly select a way to evict
    if (debug) {
        std::cout << std :: dec << getCycleTime() << ":: getEvictRandom: Evicting way: " << std::dec << wayId << " for addr: 0x" << std::hex << currentAddr.value << "\n";
    }
    return wayId;
}

int EightWayCache :: getEvictSRRIP(int index) {
    // Placeholder for SRRIP eviction policy
    assert(index < TOTAL_LINE_COUNT / NUMBER_OF_WAYS);

    int wayId;

    while(true){
        wayId = getEvictHelperSRRIP(index);
        if (wayId != -1){
            if (debug) {
                std::cout << std::dec << getCycleTime()<< ":: getEvictSRRIP: Evicting way: " << std::dec << wayId << " for addr: 0x" << std::hex << currentAddr.value << "\n";
            }
            return wayId;
        }
        incrementRRPV(index);
    }
    // Increment RRPV values if no candidate found
    
    return -1; // This line should never be reached
}

int EightWayCache :: getEvictHelperSRRIP(int index){
     for (int i = 0; i < NUMBER_OF_WAYS; ++i) {
        assert(table[index][i].valid);

        if (RRPV[index][i] == MAX_RRPV){
            if (debug) {
                std::cout << std::dec << getCycleTime() << ":: getEvictSRRIP: Evicting way: " << std::dec << i << " for addr: 0x" << std::hex << currentAddr.value << "\n";
            }
            return i;
        }
    }

    return -1;
}

void EightWayCache :: incrementRRPV(int index){
    for (int i = 0; i < NUMBER_OF_WAYS; ++i) {
        if (RRPV[index][i] < MAX_RRPV) {
            RRPV[index][i]++;
        }
    }
    if (debug) {
        std::cout << std :: dec << getCycleTime() << ":: incrementRRPV: Incremented RRPV values for index: " << std::dec << index << "\n";
    }
}

void EightWayCache::handleCacheHitLRU(int index, int wayId){
    table[index][wayId].updateTime = cycleTime;
}

void EightWayCache::handleCacheHitSRRIP(int index, int wayId){
    RRPV[index][wayId] = 0; // Set RRPV to 0 on cache hit
}

void EightWayCache :: placeCacheLine(int index, int wayId) {
    CacheElement c;
    c.addr = currentAddr.value;
    c.dirty = false;
    c.tag = currentAddr.tag;
    c.valid = true;
    c.updateTime = cycleTime;

    if (currentAddr.type == WRITE)
        c.dirty = true;

    if (table[index][wayId].valid) {
        std::cout << std :: dec << getCycleTime() << ":: placeCacheLine: Evicting index: " << std::dec << index << ", way: " << wayId << " : ";
        printCacheLine(table[index][wayId]);
    }

    // Updating the cache
    table[index][wayId] = c;

    // Update RRPV for SRRIP policy
    if (replacePolicy == SRRIP) {
        RRPV[index][wayId] = MAX_RRPV - 1; // Set RRPV to 0 on insertion
    }
}

// Review this function for correctness
void EightWayCache :: run() {
    if (currentAddr.valid) {

        totalTransaction += 1;
        int index = currentAddr.index;
        int wayId;
        int freeSlot;
        int evictId;
        if (debug){
            std::cout << std::dec << getCycleTime() <<":: run function for addr: 0x" << std::hex << currentAddr.value << ", indexMask: 0x" << std::hex << indexMask << ", index: " << std ::dec << index << "\n";
        }
        wayId = isCacheHit();

        if (wayId != -1) {
            // Cache hit
            cacheHit += 1;
            if (replacePolicy == LRU) {
                handleCacheHitLRU(index, wayId);
            } else if (replacePolicy == SRRIP) {
                handleCacheHitSRRIP(index, wayId);
            }

            if (currentAddr.type == WRITE)
                table[index][wayId].dirty = true;

            if (debug) {
                std::cout << std::dec << getCycleTime() << ":: run: Cache hit for addr: 0x" << std::hex << currentAddr.value << "\n";
            }
        } else {
            // Cache miss
            cacheMiss += 1;
            if (debug) {
                std::cout<< std::dec << getCycleTime() << ":: run: Cache miss for addr: 0x" << std::hex << currentAddr.value << "\n";
            }
            freeSlot = getEmptySlot();
            if (freeSlot != -1) {
                // Found an empty slot
                placeCacheLine(index, freeSlot);
            } else {
                // Need to evict a line
                evictId = getEvictCacheLineId();
                if (table[index][evictId].dirty) {
                    writeBacks += 1; // Increment writeBacks if the evicted line is dirty
                }
                placeCacheLine(index, evictId);
            }
        }
    }
    incrementCycleTime();
}

void EightWayCache :: printValidCache() {
    std::cout << std::dec << getCycleTime() << ":: EightWayCache :: printValidCache: Valid cache lines:\n";
    for (int i = 0; i < TOTAL_LINE_COUNT / NUMBER_OF_WAYS; ++i) {
        for (int j = 0; j < NUMBER_OF_WAYS; ++j) {
            if (table[i][j].valid) {
                std::cout << std::dec << getCycleTime()<< ":: Index: " << std::dec << i << ", Way: " << j << " : ";
                printCacheLine(table[i][j]);
            }
        }
    }
}