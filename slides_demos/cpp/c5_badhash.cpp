// Week 3 slide demo 5 — the contract broken, and the silence that follows.
//
// BadTag is AssetTag with one change: operator== compares the NORMALISED code,
// but the hash is taken over the RAW string the caller passed in. Rule 5 of the
// contract (a == b implies hash(a) == hash(b)) is now false, and nothing in the
// language says so. The set simply keeps both.
//
// This is Defect A of Lab 2.3, reduced to something that fits on a slide.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra slides_demos/cpp/c5_badhash.cpp -o bin/c5_badhash
//   ./bin/c5_badhash

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>

struct BadTag {
    std::string raw;                       // exactly what the caller typed
    std::string code;                      // trimmed + upper-cased

    explicit BadTag(std::string s) : raw(std::move(s)), code(raw) {
        code.erase(code.begin(), std::find_if(code.begin(), code.end(),
                   [](unsigned char c) { return !std::isspace(c); }));
        code.erase(std::find_if(code.rbegin(), code.rend(),
                   [](unsigned char c) { return !std::isspace(c); }).base(), code.end());
        for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    // Equality uses the NORMALISED code -- this part is correct.
    bool operator==(const BadTag& o) const noexcept { return code == o.code; }
};

template <>
struct std::hash<BadTag> {
    // ...but the hash uses the RAW string. Rule 5 is now broken.
    std::size_t operator()(const BadTag& t) const noexcept {
        return std::hash<std::string>{}(t.raw);
    }
};

int main() {
    BadTag a("LP-001");
    BadTag b("  lp-001  ");

    std::cout << "a == b          ? " << std::boolalpha << (a == b)
              << "   <- operator== says they are the same asset\n";
    std::cout << "hash(a) == hash(b)? "
              << (std::hash<BadTag>{}(a) == std::hash<BadTag>{}(b))
              << "   <- and the hash disagrees\n\n";

    std::unordered_set<BadTag> shelf;
    for (const char* s : {"LP-001", "  lp-001  ", "Lp-001", "PJ-001", "MC-001"})
        shelf.insert(BadTag(s));

    std::cout << "5 inputs, 3 distinct assets -> set size = " << shelf.size()
              << "   (expected 3)\n\n";
    for (const auto& t : shelf)
        std::cout << "  kept: " << std::setw(12) << std::left << t.code
                  << "(raw \"" << t.raw << "\")\n";

    std::cout << "\nNo exception. No warning. No compiler error. The inventory\n"
                 "now reports three laptops where one is on the shelf.\n";
    return 0;
}
