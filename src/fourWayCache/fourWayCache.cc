#include "fourWayCache.hh"

FourWayCache :: FourWayCache(){
    indexBitCount = 8;
    indexMask = 0xFF;
}

int FourWayCache :: isCacheHit(){
    int index = currentAddr.index;
    int tag = currentAddr.tag;

    for (int i = 0; i < NUMBER_OF_WAYS; ++i){
        if (table[index][i].valid){

            if ( tag == table[index][i].tag){
                return i;
            }
        }
    }
    return -1;
}

int FourWayCache :: getEmptySlot(){
    int index = currentAddr.index;
    assert (index < 256);
    for (int i = 0; i < NUMBER_OF_WAYS; ++i){

        if ( table[index][i].valid == false){
            return i;
        }
    }
    return -1;
}

//This function will be called when there is no empty slot and need to evict a line
int FourWayCache :: getEvictCacheLineId(){
    int index = currentAddr.index;
    bool flag = false;
    int t;
    int id;
    for (int i = 0; i < NUMBER_OF_WAYS; ++i){
        assert (table[index][i].valid);

        if (flag== false){
            t = table[index][i].updateTime;
            flag = true;
            id = i;
        } else {

            if ( t > table[index][i].updateTime){
                t = table[index][i].updateTime;
                id = i;
            }
        }

    }
    return id;
}


void FourWayCache :: placeCacheLine(int wayId){
    
    CacheElement c;
    c.addr = currentAddr.value;
    c.tag = currentAddr.tag;
    c.updateTime = cycleTime;
    c.valid = true;
    c.dirty = false;

    if (currentAddr.type == WRITE){
        c.dirty = true;
    }

    if (table[currentAddr.index][wayId].valid){
        std :: cout << "placeCacheLine :: Need to evict the cache at wayID: " << wayId << "\nCache line evicted:\n";
        printCacheLine(table[currentAddr.index][wayId]);

        table[currentAddr.index][wayId] = c;
    } 

    table[currentAddr.index][wayId] = c;
}

void FourWayCache :: run(){

    if (!currentAddr.valid) {
        return;
    }
    std :: cout << "run function\n";
    totalTransaction += 1;
    int index = currentAddr.index;
    int wayId;
    int freeSlot;
    int evictId;
    wayId = isCacheHit();

    if (wayId != -1){
        //Cache hit
        cacheHit += 1;
        std :: cout << "run: Cache hit\n";

        table[index][wayId].updateTime = cycleTime;

        if (currentAddr.type == WRITE){
            table[index][wayId].dirty = true;
        }

    } else {
        //Cache miss
        cacheMiss += 1;
        std :: cout << "run :: Cache miss\n";
        freeSlot = getEmptySlot();
        std :: cout << "freeSlot: " << std :: dec <<freeSlot << "\n";
        if (freeSlot != -1){
            //There is available slot
            placeCacheLine(freeSlot);
        } else {
            //No available slot. Get which cache line to evict and then place the line
            evictId = getEvictCacheLineId();
            std :: cout << "No free slot found. evict ID: " << evictId << "\n";
            placeCacheLine(evictId);
        }
    }
}

void FourWayCache :: printValidCache(){

    for (int i = 0; i < 256; ++i){
        for (int j = 0; j < NUMBER_OF_WAYS; ++j){
            if (table[i][j].valid){
                std :: cout << "index: " << i << ", way: " << j << " : ";
                printCacheLine(table[i][j]);
            }
        }
    }
}
