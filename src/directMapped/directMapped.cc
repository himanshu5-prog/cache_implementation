#include "directMapped.hh"

DirectMapped :: DirectMapped() {
    indexMask = 0x3FF;
    indexBitCount = 10;
    victimCache = nullptr;
    std::cout << "DirectMapped::Constructor : OffsetBitCount: " << offsetBitCount << ", indexMask: " << std::hex << indexMask 
        << ", indexBitCount:" << std::dec << indexBitCount << "\n";}

void DirectMapped :: setVictimCacheEnabled(){
    victimCache = std::make_unique<VictimCache>();
    std::cout << "DirectMapped: Victim cache enabled.\n";
}

bool DirectMapped :: isCacheHit(){
    uint64_t index = currentAddr.index;
    uint64_t tag = currentAddr.tag;

    if (table[index].valid){
        if (tag == table[index].tag){
            return true;
        }
    }

    return false;
}

void DirectMapped :: placeCacheLine(){

    int index = currentAddr.index;

    CacheElement c;
    c.addr = currentAddr.value;
    c.dirty = false;
    c.tag = currentAddr.tag;
    c.valid = true;
    c.updateTime = cycleTime;

    if (currentAddr.type == WRITE)
        c.dirty = true;
    
    if (table[index].valid){
        if (table[index].dirty){
            // Dirty line was modified in the main cache, count the write-back.
            writeBacks += 1;
        }

        std:: cout << "Evicting index: " << index << ", ";

        if (victimCache){
            std::cout << "DirectMapped: placeCacheLine :: Placing evicted line into victim cache.\n";
            victimCache->placeCacheLine(table[index], index); // Place the evicted line into the victim cache
        } else {
            std::cout << "DirectMapped: placeCacheLine :: Victim cache not enabled. Evicted line is just sent to lower level of cache.\n";
        }
        std::cout << "DirectMapped: placeCacheLine :: Cache line evicted:  ";
        printCacheLine(table[index]);
    }
    // Updating the cache
    table[index] = c;
}

void DirectMapped :: run(){

    if (currentAddr.valid){

        if (!isCacheHit()){
            // Cache miss
            // Check if the line is present in the victim cache
            if (victimCache) {
                if (victimCache->isCacheHit(currentAddr.tag, currentAddr.index)) {
                    // Found in victim cache, use it
                    CacheElement victimLine = victimCache->getCacheLine(currentAddr.tag, currentAddr.index);
                    std::cout << "DirectMapped: run :: Cache hit in victim cache for address: 0x" << std::hex << currentAddr.value << std::dec << ", cycle: " << getCycleTime() << "\n";
                    
                    if (currentAddr.type == WRITE) {
                        victimLine.dirty = true; // Mark as dirty if it's a write operation
                    }
                    victimLine.updateTime = cycleTime; // Update the time for LRU or other policies if needed
                    // Place the victim line back into the main cache
                    CacheElement evictedLine = table[currentAddr.index];

                    table[currentAddr.index] = victimLine;
                    // Remove the line from the victim cache
                    victimCache->removeCacheLine(currentAddr.tag, currentAddr.index);
                    cacheHit += 1; // Count this as a hit since we found it in the victim cache

                    if (evictedLine.valid) {
                        if (evictedLine.dirty) {
                            writeBacks += 1; // Count write-back if the evicted line is dirty
                        }
                        // Place the evicted line into the victim cache
                        victimCache->placeCacheLine(evictedLine, currentAddr.index);
                    }
                } else {
                    // Not found in victim cache, handle as a regular cache miss
                    std::cout << "DirectMapped: run :: Cache miss for address (miss in Victim cache and main cache): 0x" << std::hex << currentAddr.value << std::dec << ", cycle: " << getCycleTime() << "\n";
                    cacheMiss += 1;
                    placeCacheLine();
                }
            } else if (victimCache == nullptr) {
                // Victim cache not enabled, handle as a regular cache miss
                std::cout << "DirectMapped: run (No Victim Cache):: Cache miss for address: 0x" << std::hex << currentAddr.value << std::dec << ", cycle: " << getCycleTime() << "\n";
                cacheMiss += 1;
                placeCacheLine();
            }
        } else {
            // Cache hit
            cacheHit += 1;
            table[currentAddr.index].updateTime = cycleTime;

            if (currentAddr.type == WRITE)
                table[currentAddr.index].dirty = true;
            
            if (victimCache) {
                std::cout << "DirectMapped: run (With Victim Cache):: cache hit for address: 0x" << std::hex << currentAddr.value << std::dec << ", cycle: " << getCycleTime() << "\n";
            } else {
                std::cout << "DirectMapped: run (No Victim Cache):: cache hit for address: 0x" << std::hex << currentAddr.value << std::dec << ", cycle: " << getCycleTime() << "\n";
            }
        }
        totalTransaction += 1;
    }

    incrementCycleTime();
}

void DirectMapped :: printValidCache(){
    std :: cout << "Cache content\n";
    for (int i =0; i < DIRECT_MAPPED_CACHE_SIZE; ++i){
        if (table[i].valid){
            std:: cout << "index: " << std :: dec << i << ", ";
            printCacheLine(table[i]);
        }
    }
}

void DirectMapped :: printVictimCache(){
    if (victimCache){
        victimCache->printValidCache();
    } else {
        std::cout << "DirectMapped: printVictimCache :: Victim cache is not enabled.\n";
    }
}

void DirectMapped :: printVictimCacheStats(){
    if (victimCache){
        victimCache->printStats();
    } else {
        std::cout << "DirectMapped: printVictimCacheStats :: Victim cache is not enabled.\n";
    }
}
