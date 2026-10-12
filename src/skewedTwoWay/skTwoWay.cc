#include "skTwoWay.hh"

SkTwoWay::SkTwoWay() {
    for (int i = 0; i < SK_TWO_WAY_SIZE; ++i) {
        for (int j = 0; j < SK_TWO_WAY_ASSOCIATIVITY; ++j) {
            table[i][j] = CacheElement();
        }
        placeNext[i] = 0;
    }
    
}

SkTwoWay::~SkTwoWay() {
    // Destructor logic if needed
}

uint64_t SkTwoWay::getindex() {
    return (currentAddr.value >> OFFSET_BIT_COUNT) & (SK_TWO_WAY_INDEX_MASK);
}

uint64_t SkTwoWay::getindex2() {
    return (currentAddr.value >> (OFFSET_BIT_COUNT + SK_TWO_WAY_INDEX_BIT_COUNT)) & (SK_TWO_WAY_INDEX_MASK);
}

uint64_t SkTwoWay::getOffset() {
    return currentAddr.value & ((1 << OFFSET_BIT_COUNT) - 1);
}

uint64_t SkTwoWay::gettag() {
    return currentAddr.value >> (OFFSET_BIT_COUNT + (2 * SK_TWO_WAY_INDEX_BIT_COUNT));
}

uint64_t SkTwoWay::accessIndex() {
    return getindex() ^ getindex2();
}

uint64_t SkTwoWay::isCacheHit() {
    uint64_t index = accessIndex();
    uint64_t tag = gettag();
    for (int i = 0; i < SK_TWO_WAY_ASSOCIATIVITY; ++i) {
        if (table[index][i].valid && table[index][i].tag == tag) {
            if (i == 0) {
                placeNext[index] = 1;
            } else {
                placeNext[index] = 0;
            }
            return i; // Cache hit
        }
    }
    return SK_TWO_WAY_ASSOCIATIVITY; // Cache miss
}


void SkTwoWay::run() {
    // Implement the cache run logic here
    if (currentAddr.valid) {

        uint64_t bank0 = getindex();
        uint64_t bank1 = getindex2();
        totalTransaction += 1;
        std :: cout << "Accessing cache with bank0: " << bank0 << ", bank1: " << bank1 << std::endl;
        if (isCacheHit() < SK_TWO_WAY_ASSOCIATIVITY) {
            // Cache hit logic
            if (currentAddr.type == WRITE) {
                // Write hit logic
                table[accessIndex()][isCacheHit()].dirty = true;
            }
            table[accessIndex()][isCacheHit()].updateTime = cycleTime;
            std::cout << "Cache hit at index " << accessIndex() << ", way: " << isCacheHit() << std::endl;
            printCacheLine(table[accessIndex()][isCacheHit()]);
            cacheHit += 1;
        } else {
            // Cache miss logic
            cacheMiss += 1;
            std::cout << "Cache miss at index " << accessIndex() << std::endl;

            if (placeNext[bank0] == 0){
                placeCacheElement(accessIndex(), (uint64_t)0);
                placeNext[bank0] = 1;
            } else {
                placeCacheElement(accessIndex(), (uint64_t)1);
                placeNext[bank0] = 0;
            }
        }
    }
    cycleTime += 1;
}

void SkTwoWay::placeCacheElement(uint64_t index, uint64_t way) {
    assert (index < SK_TWO_WAY_SIZE && way < SK_TWO_WAY_ASSOCIATIVITY);

    CacheElement newElement;
    newElement.addr = currentAddr.value;
    newElement.tag = gettag();
    newElement.valid = true;
    newElement.dirty = (currentAddr.type == WRITE);
    newElement.updateTime = cycleTime;

    if (table[index][way].valid) {
        std::cout << "Cache element already exists at index " << index << ", way " << way <<", Need to evict" << std::endl;
        printCacheLine(table[index][way]);
    }

    table[index][way] = newElement;
    std::cout << "Placed cache element at index " << index << ", way " << way << std::endl;
    printCacheLine(table[index][way]);

    if (way == 0) {
        placeNext[index] = 1;
    } else {
        placeNext[index] = 0;
    }
}
