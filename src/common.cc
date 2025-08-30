#include "common.hh"

std::vector<Transaction> parseTraceFile(const std::string &filePath) {
    std::vector<Transaction> transactions;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return transactions; // Return empty vector on error
    }

    std::string line;
    while (std::getline(file, line)) {
         if (line.empty() || line[0] == '#') {
            continue; // Skip empty lines or comments
        }
        std::istringstream iss(line);
        char typeChar;
        unsigned int address;

        if (!(iss >> typeChar >> std::hex >> address)) {
            //std::cerr << "Error parsing line: " << line << std::endl;
            continue; // Skip malformed lines
        }

        TransactionType type;
        if (typeChar == 'R' || typeChar == 'r') {
            type = READ;
        } else if (typeChar == 'W' || typeChar == 'w') {
            type = WRITE;
        } else {
            std::cerr << "Unknown transaction type: " << typeChar << " in line: " << line << std::endl;
            continue; // Skip unknown transaction types
        }

        transactions.emplace_back(type, address);
    }

    file.close();
    return transactions;
}