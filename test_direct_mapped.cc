
#include "src/directMapped/directMapped.hh"
#define TOTAL_TRANSACTION_COUNT 10

void test();
void test_random();
void test_victim_cache();

int main(){

    //test();
    //test_random();
    test_victim_cache();
    return 0;
}

void test(){
    DirectMapped cache;
    
    //Setting the address and place it in cache if needed
    cache.setAddr(0x12, WRITE);
    cache.run();

    cache.setAddr(0x13, WRITE);
    cache.run();

    cache.setAddr(0x23, WRITE);
    cache.run();

    cache.setAddr(0x100012, READ);
    cache.run();

    cache.setAddr(0x12345, READ);
    cache.run();

    //Done with test and print out the stats
    cache.printStats();
    cache.printValidCache(); 
}

void test_random(){
    std::cout << "Running random test\n";
    DirectMapped cache;
    unsigned int addr;
    cache.setVictimCacheEnabled(); // Enable victim cache

    addr = rand() % 0xffff;
    TransactionType t;

    for (int i = 0; i < TOTAL_TRANSACTION_COUNT; ++i){
        //Generating random address
        addr = rand() % 0xffff;

        // For even value of i: Request type is Write
        // For odd value of i: Request type is Read
        t = (i%2) ? READ : WRITE;
        
        cache.setAddr(addr,t);
        std::cout << "Random test: Request " << i+1 << " - Address: 0x" << std::hex << addr << ", Type: " << (t == WRITE ? "WRITE" : "READ") << "\n";
        cache.printAddr();
        cache.run();
        //cache.incrementCycleTime();
    }
    cache.printStats();
    cache.printValidCache();
    cache.printVictimCacheStats();
    cache.printVictimCache();
}

void test_victim_cache() {
    std::cout << "Running victim cache test\n";
    DirectMapped cache;
    cache.setVictimCacheEnabled(); // Enable victim cache

    // Simulate a series of accesses that will cause evictions
    unsigned int addresses[] = {0x12, 0x13, 0x23, 0x100012, 0x12345, 0x12, 0x13, 0x23, 0x100012, 0x12345};
    TransactionType types[] = {WRITE, WRITE, WRITE, READ, READ, READ, READ, READ, READ, READ};

    for (int i = 0; i < sizeof(addresses)/sizeof(addresses[0]); ++i) {
        cache.setAddr(addresses[i], types[i]);
        std::cout << "\nVictim cache test: Request " << i+1 << " - Address: 0x" << std::hex << addresses[i] << ", Type: " << (types[i] == WRITE ? "WRITE" : "READ") << "\n";
        cache.printAddr();
        cache.run();
    }

    cache.printStats();
    cache.printValidCache();
    cache.printVictimCacheStats();
    cache.printVictimCache();
}