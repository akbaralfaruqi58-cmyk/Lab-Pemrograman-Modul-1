// Week 3 slide demo 7 — what the seeded defect looks like from the test runner.
//
// The same BadTag as c5_badhash.cpp, but now asserted against by the module's
// own harness. This is the transcript Lab 2.3 asks you to produce BEFORE you
// fix anything: a red test that names the contract it broke.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra -Icpp/tests
//       slides_demos/cpp/c7_failing_test.cpp -o bin/c7_failing_test
//   ./bin/c7_failing_test        # exits 1

#include "mini_catch.hpp"
#include <string>
#include <unordered_set>

struct BadTag {
    std::string raw, code;
    explicit BadTag(std::string s) : raw(std::move(s)), code(raw) {
        while (!code.empty() && code.front() == ' ') code.erase(code.begin());
        while (!code.empty() && code.back() == ' ') code.pop_back();
        for (char& c : code) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    bool operator==(const BadTag& o) const noexcept { return code == o.code; }
};
template <> struct std::hash<BadTag> {
    std::size_t operator()(const BadTag& t) const noexcept {
        return std::hash<std::string>{}(t.raw);          // <- breaks rule 5
    }
};

TEST_CASE("Rule 5: a == b implies hash(a) == hash(b)") {
    BadTag a("LP-001"), b("  lp-001  ");
    REQUIRE(a == b);
    REQUIRE(std::hash<BadTag>{}(a) == std::hash<BadTag>{}(b));
}

TEST_CASE("unordered_set folds three spellings of LP-001 into one") {
    std::unordered_set<BadTag> s;
    for (const char* t : {"LP-001", "  lp-001  ", "Lp-001", "PJ-001", "MC-001"})
        s.insert(BadTag(t));
    REQUIRE(s.size() == 3);
}

int main() { return test_runner::runAll(); }
