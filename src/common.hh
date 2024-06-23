#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <string>


enum TransactionType {
    WRITE,
    READ
};

// Cache state for MOESI protocol-----------------------------------------------------
enum CacheState {
    INVALID,
    MODIFIED,
    OWNED,
    EXCLUSIVE,
    SHARED
};
//-----------------------------------------------------------------------------------
// Breakdown of address--------------------------------------------------------------
struct Address {
    bool valid;
    unsigned int value;
    unsigned int tag;
    unsigned int index;
    unsigned int offset;
    TransactionType type;

    Address() : valid(false), value(0), tag(0), index(0), offset(0), type(READ) {}
};
//--------------------------------------------------------------------------------
#endif 
