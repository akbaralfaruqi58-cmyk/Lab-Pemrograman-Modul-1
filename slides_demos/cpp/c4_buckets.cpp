// Week 3 slide demo 4 — the contract honoured, with the container's own numbers.
//
// Prints the REAL FNV-1a values AssetTag::customHash() produces and the REAL
// bucket indices std::unordered_set chose for them. Module 1 Figure 7 draws
// this picture with illustrative values; this is the same picture measured.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra -Icpp/include
//       slides_demos/cpp/c4_buckets.cpp -o bin/c4_buckets
//   ./bin/c4_buckets

#include "pinjamlab/asset_tag.hpp"
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace pinjamlab;

int main() {
    // The five-element sequence Lab 2.2 asks you to build. Three are the SAME
    // asset spelled three ways; the contract has to fold them into one.
    const std::vector<std::string> raw = {
        "LP-001", "  lp-001  ", "Lp-001", "PJ-001", "MC-001"
    };

    std::unordered_set<AssetTag> unique;
    std::cout << "input           normalised   hash (FNV-1a, low 16 bits)   inserted?\n";
    std::cout << "--------------  -----------  --------------------------  ---------\n";

    for (const auto& s : raw) {
        AssetTag t(s);
        const auto [it, added] = unique.insert(t);
        (void)it;
        std::cout << std::left << std::setw(16) << ("\"" + s + "\"")
                  << std::setw(13) << t.getCode()
                  << "0x" << std::hex << std::setw(4) << std::setfill('0')
                  << (t.customHash() & 0xFFFF) << std::dec << std::setfill(' ')
                  << std::setw(24) << ""
                  << (added ? "yes" : "NO - duplicate") << '\n';
    }

    std::cout << "\nset size = " << unique.size()
              << "   (5 inputs, 3 distinct assets)\n";
    std::cout << "bucket_count = " << unique.bucket_count() << '\n';

    std::size_t collisions = 0;
    std::cout << "\ncode      bucket   items in that bucket\n";
    std::cout << "--------  -------  --------------------\n";
    for (const auto& t : unique) {
        const auto b = unique.bucket(t);
        const auto n = unique.bucket_size(b);
        if (n > 1) ++collisions;
        std::cout << std::left << std::setw(10) << t.getCode()
                  << std::setw(9) << b << n
                  << (n > 1 ? "   <- collision" : "") << '\n';
    }

    std::cout << "\n" << collisions << " of " << unique.size()
              << " keys share a bucket with another key.\n"
                 "A collision is not a bug: two DIFFERENT assets landed in the\n"
                 "same bucket, so the container called operator== to tell them\n"
                 "apart -- and kept both. That is the contract working.\n";
    return 0;
}
