#ifndef _FOURWAYCACHE_
#define _FOURWAYCACHE_

#include "../cache/cache.hh"
#include "../common.hh"

#define NUMBER_OF_WAYS 4
#define TOTAL_LINE_COUNT 1024
class FourWayCache : public Cache {
    CacheElement table[TOTAL_LINE_COUNT/NUMBER_OF_WAYS][NUMBER_OF_WAYS];

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
