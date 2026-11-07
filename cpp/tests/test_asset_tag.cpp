#include "mini_catch.hpp"
#include "pinjamlab/asset_tag.hpp"
#include <unordered_set>
#include <vector>

using namespace pinjamlab;

TEST_CASE("AssetTag: Normalization and validation") {
    AssetTag tag("  lp-001  ");
    REQUIRE(tag.getCode() == "LP-001");

    REQUIRE_THROWS_AS(AssetTag("   "), std::invalid_argument);
    REQUIRE_THROWS_AS(AssetTag(""), std::invalid_argument);
}

TEST_CASE("AssetTag: 5-Case Mathematical Equality Contract") {
    // 1. Reflexivity: a == a
    AssetTag a("LP-001");
    REQUIRE(a == a);

    // 2. Symmetry: a == b <=> b == a
    AssetTag b("LP-001");
    REQUIRE(a == b);
    REQUIRE(b == a);

    // 3. Transitivity: a == b and b == c => a == c
    AssetTag c("LP-001");
    REQUIRE(a == b);
    REQUIRE(b == c);
    REQUIRE(a == c);

    // 4. Consistency: repeated evaluations produce identical results
    REQUIRE(a == b);
    REQUIRE(a == b);

    // 5. Inequality
    AssetTag d("PJ-001");
    REQUIRE(a != d);
    REQUIRE(!(a == d));
}

TEST_CASE("AssetTag: Hash Consistency and std::unordered_set Deduplication") {
    AssetTag t1("LP-001");
    AssetTag t2("LP-001");
    AssetTag t3("PJ-001");

    // Equal objects MUST produce equal hash values
    REQUIRE(std::hash<AssetTag>{}(t1) == std::hash<AssetTag>{}(t2));

    // Deduplication test on identical input sequence
    std::vector<AssetTag> inputSequence = {
        AssetTag("LP-001"),
        AssetTag("LP-002"),
        AssetTag("LP-001"), // duplicate
        AssetTag("PJ-001"),
        AssetTag("LP-002")  // duplicate
    };

    std::unordered_set<AssetTag> uniqueTags(inputSequence.begin(), inputSequence.end());
    REQUIRE(uniqueTags.size() == 3);
    REQUIRE(uniqueTags.count(AssetTag("LP-001")) == 1);
    REQUIRE(uniqueTags.count(AssetTag("LP-002")) == 1);
    REQUIRE(uniqueTags.count(AssetTag("PJ-001")) == 1);
}

int main() {
    return test_runner::runAll();
}
