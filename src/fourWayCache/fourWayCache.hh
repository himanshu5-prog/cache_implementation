#ifndef _FOURWAYCACHE_
#define _FOURWAYCACHE_

#include "../cache/cache.hh"
#include "../common.hh"

#define NUMBER_OF_WAYS 4

class FourWayCache : public Cache {
    CacheElement table[256][4];

    public:
    FourWayCache();
    int isCacheHit();
    int getEmptySlot();
    int getEvictCacheLineId();
    void placeCacheLine(int wayID);
    void run();
    void printValidCache();

};

#endif
