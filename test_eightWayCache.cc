#include "src/eightWayCache/eightWayCache.hh"
#define TOTAL_TRANSACTION_COUNT 2000

int main(int argc, char* argv[]) {
    std :: unordered_map<std::string, CacheReplacementPolicy> policyMap = {
        {"LRU", LRU},
        {"RANDOM", RANDOM},
        {"SRRIP", SRRIP}
    };
    std :: string policyStr="LRU";
    std:: string inputFile = "trace/simple_trace.txt";
    bool cacheDebugMode = false;
    for (int i = 1; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;

        if (argv[i] == std::string("--help") || argv[i] == std::string("-h")) {
            std::cout << "Usage: " << argv[0] << " [--debug] [--policy POLICY] [--trace TRACE]\n";
            std::cout << "Options:\n";
            std::cout << "  --debug, -d           Enable debug mode\n";
            std::cout << "  --policy POLICY, -p POLICY  Set cache replacement policy (LRU, RANDOM, SRRIP)\n";
            std ::cout << "  --trace TRACE, -t TRACE    Set input trace file path\n";
            return 0;
        }

        if (argv[i] == std::string("--debug") || argv[i] == std::string("-d")) {
            std::cout << "Debug mode enabled\n";
            cacheDebugMode = true;
        } else if (argv[i] == std::string("--policy") || argv[i] == std::string("-p")) {
            if (i + 1 < argc) {
                std::string policy = argv[++i];
                if (policy == "LRU" || policy == "RANDOM" || policy == "SRRIP") {
                    policyStr = policy;
                    std::cout << "Cache replacement policy set to: " << policy << std::endl;
                } else {
                    std::cerr << "Unknown policy: " << policy << ". Supported policies are LRU, RANDOM, SRRIP.\n";
                    return 1;
                }
            } else {
                std::cerr << "--policy option requires one argument.\n";
                return 1;
            }
        } else if (argv[i] == std::string("--trace") || argv[i] == std::string("-t")) {
            if (i + 1 < argc) {
                inputFile = argv[++i];
                std::cout << "Trace file set to: " << inputFile << std::endl;
            } else {
                std::cerr << "--trace option requires one argument.\n";
                return 1;
            }
        } else {
            std::cerr << "Unknown argument: " << argv[i] << "\n";
            return 1;
        }
    }
    EightWayCache cache;
    unsigned int addr;
    TransactionType t;

    bool debugMode;
    CacheReplacementPolicy policy;

    debugMode = false;
    policy = policyMap[policyStr];
    cache.setDebugMode(cacheDebugMode);
    cache.setReplacementPolicy(policy);
    
    std :: string inputFile2 = "trace/stress_trace.txt";
    std :: string inputFile3 = "trace/gemini_trace_2.txt";
    std :: string inputFile4 = "trace/same_set_mixed_trace_16B.txt";
    std :: vector<Transaction> transactions = parseTraceFile(inputFile);
    for (const auto &trans : transactions) {
        unsigned int value = trans.value;
        TransactionType type = trans.type;
        cache.setAddr(value, type);
        cache.run();
    }
    cache.printStats();
    std :: cout << cache.getReplacementPolicyString() << " Policy was used\n";
    return 0;
}