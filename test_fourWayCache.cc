#include "src/fourWayCache/fourWayCache.hh"

int main(){
    FourWayCache cache;

    cache.setAddr(0x23, READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.setAddr(0x1023, READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.setAddr(0x2023,READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.setAddr(0x3023,READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.setAddr(0x4023,READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.setAddr(0x4024, READ);
    cache.printAddr();
    cache.run();
    cache.incrementCycleTime();

    cache.printStats();
    cache.printValidCache();

    return 0;
}

