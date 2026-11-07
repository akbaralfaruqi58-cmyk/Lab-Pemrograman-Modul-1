#include "pinjamlab/structures/hash_map.hpp"
#include <fstream>
#include <iostream>
#include <string>
using pinjamlab::structures::HashMap;

int main() {
    std::ifstream in("sample-data/my-catalog.csv");
    std::string line;
    std::getline(in, line);                          // buang header
    HashMap<std::string, int> m;                     // mulai dengan 8 bucket
    for (int i = 1; i <= 8 && std::getline(in, line); ++i) {
        const std::string tag = line.substr(0, line.find(','));
        m.insert(tag, i);
        std::cout << "insert #" << i << " " << tag
                  << "   m=" << m.bucketCount() << " n=" << m.size()
                  << " lambda=" << m.loadFactor()
                  << " rehash=" << m.rehashCount() << "\n";
        m.dump(std::cout);
    }
}