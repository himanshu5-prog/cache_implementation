#include "cache.hh"

Cache :: Cache(){
    Address a;
    a.index = 0;
    a.offset = 0;
    a.tag = 0;
    a.value = 0;
    a.valid = false;

    currentAddr = a;
    cacheHit = 0;
    cacheMiss = 0;
    totalTransaction = 0;

    offsetBitCount = 4;
    indexBitCount = 0;
    indexMask = 0;
    cycleTime = 0;
}

void Cache :: setAddr(unsigned int a, TransactionType type){
    Address addr;
    addr.index = 0;
    addr.offset = 0;
    addr.tag = 0;
    addr.valid = true;
    addr.value = a;
    addr.type = type;

    currentAddr = addr;

    setAddrParam();
}

void Cache :: setOffset(){
    currentAddr.offset = currentAddr.value & 0xF;
}

void Cache :: setIndex(){
    //std :: cout << "setIndex:: indexMask: 0x" << std:: hex << indexMask << "\n";
    currentAddr.index = (currentAddr.value >> 4 ) & indexMask;
}

void Cache :: setTag(){
    //std :: cout << "setTag :: offset bit count: " << offsetBitCount << ", index bit count: " <<std :: dec <<  indexBitCount << "\n";
    currentAddr.tag = currentAddr.value >> (offsetBitCount + indexBitCount);
}

void Cache :: setAddrParam(){
    setOffset();
    setTag();
    setIndex();
}

void Cache :: incrementCycleTime(){
    cycleTime += 1;
}

void Cache :: printStats(){
    std :: cout << "Total transaction: " <<  std:: dec << totalTransaction << "\n";
    std :: cout << "Cache miss: " << std :: dec <<cacheMiss << "\n";
    std :: cout << "Cache hit: " << std :: dec << cacheHit << "\n";
}

void Cache :: printAddr(){
    std :: string trType;

    if (currentAddr.valid){

        if (currentAddr.type == READ){
            trType = "Read";
        } else if ( currentAddr.type == WRITE){
            trType = "Write";
        } else {
            trType = "Unknown";
        }

        std :: cout << "Printing address:: value: 0x" << std :: hex << currentAddr.value << ", valid: " << currentAddr.valid << ", index: " << std::dec << currentAddr.index
        << ", offset: 0x" << std :: hex << currentAddr.offset<< ", tag: 0x" << std :: hex << currentAddr.tag << ", type: " << trType << "\n";

    }
}

void printCacheLine(CacheElement c){
    std :: cout << "valid: " << c.valid << ", dirty: " << c.dirty << ", tag: 0x" << std :: hex << c.tag << ", addr: 0x" << std :: hex << c.addr << ", updateTime: " << std :: dec << c.updateTime << "\n";
}