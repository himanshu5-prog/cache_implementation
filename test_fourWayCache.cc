#include "src/fourWayCache/fourWayCache.hh"

void test();
void test_random();

int main(){

    //test();
    test_random();
    return 0;
}

void test(){
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
 
}

void test_random(){
    //Test function to generate random address
    FourWayCache cache;
    unsigned int addr;

    addr = rand() % 0xffff;
    TransactionType t;
    for (int i = 0; i < 20; ++i){
        //Generating random address
        addr = rand() % 0xffff;

        // For even value of i: Request type is Write
        // For odd value of i: Request type is Read
        t = (i%2) ? READ : WRITE;
        
        cache.setAddr(addr,t);
        cache.printAddr();
        cache.run();
        cache.incrementCycleTime();
    }

    cache.printStats();
    cache.printValidCache();
}