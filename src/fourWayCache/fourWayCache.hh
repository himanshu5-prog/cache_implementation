#ifndef _FOURWAYCACHE_
#define _FOURWAYCACHE_

#include "../cache/cache.hh"
#include "../common.hh"

#define NUMBER_OF_WAYS 4
#define TOTAL_LINE_COUNT 1024
class FourWayCache : public Cache {
    CacheElement table[TOTAL_LINE_COUNT/NUMBER_OF_WAYS][NUMBER_OF_WAYS];

    public:
    //Constructor-----------
    FourWayCache();
    //----------------------
    //Return the wayID if there is cache hit. Otherwise return -1
    int isCacheHit();
    //-----------------------------------------------------------
    // Check if there is empty slot in the line. Return the wayID if there is
    // Otherwise return -1
    int getEmptySlot();
    //--------------------------------------------------------------
    // Returns the line which is to be evicted.
    int getEvictCacheLineId();
    //-----------------------------------------------------------------
    // Place the line in a given wayID
    void placeCacheLine(int wayID);
    //-----------------------------------------------------------
    // Top level function
    void run();
    //------------------------------------------------------------
    // Function to print valid cache line------------------------
    void printValidCache();
    //---------------------------------------------------------
};

#endif
