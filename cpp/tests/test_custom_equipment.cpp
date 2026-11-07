#include "mini_catch.hpp"
#include "pinjamlab/spectrophotometer.hpp"
#include "pinjamlab/robotic_arm.hpp"
#include <memory>
#include <string>
#include <vector>

using namespace pinjamlab;

TEST_CASE("Spectrophotometer overrides Equipment contract and stores specifications") {
    Spectrophotometer spectro(AssetTag("SP-001"), "Spectro-UV-Vis-01");

    REQUIRE(spectro.categoryName() == "Spectrophotometer");
    REQUIRE(spectro.dailyFine() == 120000);
    REQUIRE(spectro.getMinWavelengthNm() == 190.0);
    REQUIRE(spectro.getMaxWavelengthNm() == 1100.0);
    REQUIRE(spectro.getSpectralBandwidthNm() == 1.0);
}

TEST_CASE("Spectrophotometer rejects invalid wavelength specifications") {
    REQUIRE_THROWS_AS(
        Spectrophotometer(AssetTag("SP-002"), "Invalid min", 0.0, 1100.0, 1.0),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        Spectrophotometer(AssetTag("SP-003"), "Invalid range", 1100.0, 190.0, 1.0),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        Spectrophotometer(AssetTag("SP-004"), "Invalid bandwidth", 190.0, 1100.0, 0.0),
        std::invalid_argument);
}

TEST_CASE("RoboticArm overrides Equipment contract and stores specifications") {
    RoboticArm arm(AssetTag("RA-001"), "RoboticArm-01");

    REQUIRE(arm.categoryName() == "RoboticArm");
    REQUIRE(arm.dailyFine() == 180000);
    REQUIRE(arm.getPayloadKg() == 10.0);
    REQUIRE(arm.getMaxReachMm() == 1300.0);
}

TEST_CASE("RoboticArm rejects invalid hardware specifications") {
    REQUIRE_THROWS_AS(
        RoboticArm(AssetTag("RA-002"), "Invalid payload", 0.0, 1300.0),
        std::invalid_argument);
    REQUIRE_THROWS_AS(
        RoboticArm(AssetTag("RA-003"), "Invalid reach", 10.0, 0.0),
        std::invalid_argument);
}

TEST_CASE("New equipment subclasses support polymorphic dynamic dispatch") {
    std::vector<std::unique_ptr<Equipment>> inventory;
    inventory.push_back(std::make_unique<Spectrophotometer>(AssetTag("SP-010"), "Spectro"));
    inventory.push_back(std::make_unique<RoboticArm>(AssetTag("RA-010"), "Robot"));

    REQUIRE(inventory[0]->categoryName() == "Spectrophotometer");
    REQUIRE(inventory[1]->categoryName() == "RoboticArm");
    REQUIRE(inventory[0]->dailyFine() == 120000);
    REQUIRE(inventory[1]->dailyFine() == 180000);
    REQUIRE(inventory[0]->dailyFine() + inventory[1]->dailyFine() == 300000);
}

int main() {
    return test_runner::runAll();
}
