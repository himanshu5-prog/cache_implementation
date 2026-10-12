#ifndef SKTWO_WAY_HH
#define SKTWO_WAY_HH
#include <iostream>
#include <vector>
#include <algorithm>
#include "../cache/cache.hh"

#define SK_TWO_WAY_INDEX_BIT_COUNT 9
#define SK_TWO_WAY_SIZE 512
#define SK_TWO_WAY_ASSOCIATIVITY 2

#define SK_TWO_WAY_INDEX_MASK ((1 << SK_TWO_WAY_INDEX_BIT_COUNT) - 1)
class SkTwoWay : public Cache {
private:
    CacheElement table[SK_TWO_WAY_SIZE][SK_TWO_WAY_ASSOCIATIVITY];
    bool placeNext[SK_TWO_WAY_SIZE];
    
    
public:
    SkTwoWay();
    ~SkTwoWay();
    uint64_t getindex ();
    uint64_t getindex2 ();
    uint64_t getOffset();
    uint64_t gettag ();

    uint64_t accessIndex();
    uint64_t isCacheHit();
    void run();
    void placeCacheElement(uint64_t index, uint64_t way);
    

};
#endif // SKTWO_WAY_HH