#ifndef _CACHE_H_
#define _CACHE_H_

#include <iostream>
#include <cassert>
#include <cstring>
#include "../common.hh"

struct CacheElement {
    bool valid;
    bool dirty;
    unsigned int tag;
    unsigned int addr;
    int updateTime;

    CacheElement() : valid (false), dirty(false), tag(0), addr(0), updateTime(0) {}
};

void printCacheLine(CacheElement c);

class Cache {
    protected:

    Address currentAddr;
    //Stats-----------------
    int cacheMiss;
    int cacheHit;
    int totalTransaction;
    //-----------------------
    int offsetBitCount;
    int indexBitCount;
    int indexMask;
    int cycleTime;

    public:
    Cache();

    void setAddr (unsigned int addr, TransactionType t);
    void setOffset();
    void setIndex();
    void setTag();
    void setAddrParam();
    void printAddr();
    void incrementCycleTime();
};

#endif