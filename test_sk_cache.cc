#include "src/skewedTwoWay/skTwoWay.hh"

#define TRANSACTIONS 20

void test();
int main() {
    test();
    return 0;
}

void test() {
    SkTwoWay cache;

    cache.setAddr(0x1234, READ);
    cache.run();

    cache.setAddr(0x1234, WRITE);
    cache.run();

    cache.setAddr(0x234, READ);
    cache.run();

    cache.setAddr(0xffff345, WRITE);
    cache.run();

    cache.printStats();

}

