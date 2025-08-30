
#include "src/directMapped/directMapped.hh"
#define TOTAL_TRANSACTION_COUNT 10

void test();
void test_random();

int main(){

    //test();
    test_random();
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

    addr = rand() % 0xffff;
    TransactionType t;

    for (int i = 0; i < TOTAL_TRANSACTION_COUNT; ++i){
        //Generating random address
        addr = rand() % 0xffff;

        // For even value of i: Request type is Write
        // For odd value of i: Request type is Read
        t = (i%2) ? READ : WRITE;
        
        cache.setAddr(addr,t);
        cache.printAddr();
        cache.run();
        //cache.incrementCycleTime();
    }
    cache.printStats();
    cache.printValidCache();
}