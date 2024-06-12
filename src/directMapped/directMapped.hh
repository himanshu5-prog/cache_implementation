#ifndef _DIRECTMAPPED_
#define _DIRECTMAPPED_

#include "../cache/cache.hh"
#include "../common.hh"

#define INDEX_MASK_DIRECT_MAPPED 0x3FF
#define BIT_COUNT_INDEX_DIRECT_MAPPED 10

class DirectMapped : public Cache {
    
    CacheElement table[1024];

    public:
    DirectMapped();
    
    bool isCacheHit(); // Determines whether cache access is hit or miss
    void placeCacheLine(); // Place the request in cache
    void run(); // top level function which needs to be call to place a request in cache
    void printValidCache(); // Print cache table containing valid content

};
#endif