// Week 3 slide demo 6 — DOES NOT COMPILE, on purpose.
//
// A value type with operator== but NO std::hash specialisation, dropped into
// an unordered_set. C++ catches this one at compile time; the price is the
// diagnostic, which is the artifact this file exists to produce.
//
// Capture the error (from code/Module1):
//   g++ -std=c++20 slides_demos/cpp/c6_nohash.cpp -o /dev/null \
//       2> slides_demos/transcripts/c6_nohash_diagnostic.txt
//
// Compare with c5_badhash.cpp, which DOES compile and is the worse outcome.

#include <string>
#include <unordered_set>

struct PlainTag {
    std::string code;
    bool operator==(const PlainTag& o) const noexcept { return code == o.code; }
    // no std::hash<PlainTag> anywhere
};

int main() {
    std::unordered_set<PlainTag> shelf;
    shelf.insert(PlainTag{"LP-001"});
    return 0;
}
