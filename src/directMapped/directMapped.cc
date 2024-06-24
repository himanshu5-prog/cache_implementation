#include "directMapped.hh"

DirectMapped :: DirectMapped(){
    indexMask = 0x3FF;
    indexBitCount = 10;
}

bool DirectMapped :: isCacheHit(){
    int index = currentAddr.index;
    int tag = currentAddr.tag;

    if (table[index].valid){
        if (tag == table[index].tag){
            return true;
        }
    }

    return false;
}

void DirectMapped :: placeCacheLine(){

    int index = currentAddr.index;

    CacheElement c;
    c.addr = currentAddr.value;
    c.dirty = false;
    c.tag = currentAddr.tag;
    c.valid = true;
    c.updateTime = cycleTime;

    if (currentAddr.type == WRITE)
        c.dirty = true;
    
    if (table[index].valid){
        std:: cout << "Evicting index: " << index << ", ";
        printCacheLine(table[index]);
    }

    // Updating the cache
    table[index] = c;

    

}

void DirectMapped :: run(){

    if (currentAddr.valid){

        if (!isCacheHit()){
            std :: cout << "DirectMapped: run :: Cache miss for address: 0x" << currentAddr.value << "\n";
            printAddr();
            cacheMiss += 1;
            placeCacheLine();
        } else {
            cacheHit += 1;
            table[currentAddr.index].updateTime = cycleTime;

            if (currentAddr.type == WRITE)
                table[currentAddr.index].dirty = true;
            
            std :: cout << "DirectMapped: run :: cache hit for address: 0x" << currentAddr.value << "\n";
            printAddr();
        }

        totalTransaction += 1;
    }

    incrementCycleTime();
}

void DirectMapped :: printValidCache(){
    std :: cout << "Cache content\n";
    for (int i =0; i < 1024; ++i){
        if (table[i].valid){
            std:: cout << "index: " << std :: dec << i << ", ";
            printCacheLine(table[i]);
        }
    }
}
