#ifndef _DIRECTMAPPED_
#define _DIRECTMAPPED_

#include "../cache/cache.hh"
#include "../common.hh"

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