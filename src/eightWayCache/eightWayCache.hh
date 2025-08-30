#ifndef __EIGHTWAYCACHE__
#define __EIGHTWAYCACHE__

#include "../cache/cache.hh"
#include "../common.hh"
#include <array>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()
#define NUMBER_OF_WAYS 8
#define TOTAL_LINE_COUNT 1024
#define MAX_RRPV 3 // Maximum RRPV value for SRRIP
class EightWayCache : public Cache {
    CacheElement table[TOTAL_LINE_COUNT/NUMBER_OF_WAYS][NUMBER_OF_WAYS];
    std :: array < std :: array <int, NUMBER_OF_WAYS>, TOTAL_LINE_COUNT/NUMBER_OF_WAYS> RRPV; // For SRRIP policy
    bool debug;
    CacheReplacementPolicy replacePolicy;
    public:
    //Constructor-----------
    EightWayCache();
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
    // Least Recently Used Eviction policy
    int getEvictLRU(int index);
    //---------------------------------------------------------
    // Random Eviction policy
    int getEvictRandom(int index);
    // ---------------------------------------------------------
    // SRRIP Eviction policy
    int getEvictSRRIP(int index);
    // ---------------------------------------------------------
    int getEvictHelperSRRIP(int index);

    void incrementRRPV(int index);
    //---------------------------------------------------------
    void handleCacheHitLRU(int index, int wayId);
    //---------------------------------------------------------
    void handleCacheHitSRRIP(int index, int wayId);

    void placeCacheLine(int index, int wayId);
    //---------------------------------------------------------
    // Setters and Getters for replacement policy
    void setReplacementPolicy(CacheReplacementPolicy policy) { replacePolicy = policy; }
    CacheReplacementPolicy getReplacementPolicy() const { return replacePolicy; }
    std :: string getReplacementPolicyString() const {
        switch(replacePolicy) {
            case LRU:
                return "LRU";
            case RANDOM:
                return "RANDOM";
            case SRRIP:
                return "SRRIP";
            default:
                return "UNKNOWN";
        }
    }
    //---------------------------------------------------------
    // Enable or disable debug mode
    void setDebugMode(bool mode) { debug = mode; }
    bool isDebugMode() const { return debug; }
}; 
#endif // __EIGHTWAYCACHE__