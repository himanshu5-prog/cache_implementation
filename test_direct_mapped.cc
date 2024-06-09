
#include "src/directMapped/directMapped.hh"
int main(){

    DirectMapped cache;

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

    cache.printStats();
    cache.printValidCache();
    
}