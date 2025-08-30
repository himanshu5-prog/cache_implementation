#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <fstream>

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

enum CacheType {
    DIRECT_MAPPED,
    FOUR_WAY,
    EIGHT_WAY,
    TOTAL_CACHE_TYPE
};

enum CacheReplacementPolicy {
    LRU, // Least Recently Used
    FIFO, // First In First Out
    SRRIP, // Static Re-Reference Interval Prediction
    RANDOM, // Random Replacement
    TOTAL_CACHE_POLICY
};

struct Transaction {
    TransactionType type;
    unsigned int value;
    Transaction() : type(READ), value(0) {}
    Transaction(TransactionType t, unsigned int v) : type(t), value(v) {}
};

std :: vector<Transaction> parseTraceFile(const std::string &filePath);
//--------------------------------------------------------------------------------
#endif 
