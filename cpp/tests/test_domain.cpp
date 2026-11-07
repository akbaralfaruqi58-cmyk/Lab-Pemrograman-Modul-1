#include "mini_catch.hpp"
#include "pinjamlab/equipment.hpp"
#include "pinjamlab/laptop.hpp"
#include "pinjamlab/bundle.hpp"
#include "pinjamlab/member.hpp"
#include "pinjamlab/loan.hpp"
#include <vector>
#include <memory>
#include <numeric>

using namespace pinjamlab;

// Test concrete subclass for original artifact verification
class Projector : public Equipment {
private:
    int lumens_;
public:
    Projector(AssetTag tag, std::string model, int lumens = 3600)
        : Equipment(std::move(tag), std::move(model)), lumens_(lumens) {}

    [[nodiscard]] int dailyFine() const override {
        return 35000; // Rp 35,000 / day
    }

    [[nodiscard]] std::string categoryName() const override {
        return "Projector";
    }
};

TEST_CASE("Polymorphism: Dynamic dispatch across Equipment hierarchy") {
    std::vector<std::unique_ptr<Equipment>> inventory;
    inventory.push_back(std::make_unique<Laptop>(AssetTag("LP-001"), "ThinkPad T14", 16));
    inventory.push_back(std::make_unique<Projector>(AssetTag("PJ-001"), "Epson EB-X51", 3800));

    REQUIRE(inventory[0]->dailyFine() == 50000);
    REQUIRE(inventory[1]->dailyFine() == 35000);

    int totalDailyFine = 0;
    for (const auto& item : inventory) {
        totalDailyFine += item->dailyFine();
    }
    REQUIRE(totalDailyFine == 85000);
}

TEST_CASE("Equipment: State transitions and exception guards") {
    Laptop laptop(AssetTag("LP-001"), "ThinkPad T14");
    REQUIRE(laptop.getState() == EquipmentState::AVAILABLE);

    laptop.markLoaned();
    REQUIRE(laptop.getState() == EquipmentState::LOANED);

    REQUIRE_THROWS_AS(laptop.markLoaned(), std::runtime_error);

    laptop.markReturned();
    REQUIRE(laptop.getState() == EquipmentState::AVAILABLE);
}

TEST_CASE("Member: Encapsulation and quota limits") {
    Member student("M-101", "Budi Santoso", Role::STUDENT);
    REQUIRE(student.getMaxLoans() == 2);
    REQUIRE(student.getActiveLoans() == 0);
    REQUIRE(student.canBorrow() == true);

    student.recordLoan();
    student.recordLoan();
    REQUIRE(student.getActiveLoans() == 2);
    REQUIRE(student.canBorrow() == false);

    REQUIRE_THROWS_AS(student.recordLoan(), std::runtime_error);

    student.recordReturn();
    REQUIRE(student.getActiveLoans() == 1);
    REQUIRE(student.canBorrow() == true);
}

TEST_CASE("Loan: Overdue fine calculation and ordering") {
    auto laptop = std::make_shared<Laptop>(AssetTag("LP-001"), "ThinkPad T14");
    auto student = std::make_shared<Member>("M-101", "Budi Santoso", Role::STUDENT);

    Loan loan1("L-001", laptop, student, 7, 3);
    // 3 overdue days * 50,000 = 150,000
    REQUIRE(loan1.calculateFine() == 150000);

    auto projector = std::make_shared<Projector>(AssetTag("PJ-001"), "Epson EB-X51");
    Loan loan2("L-002", projector, student, 3, 2);
    // 2 overdue days * 35,000 = 70,000
    REQUIRE(loan2.calculateFine() == 70000);

    REQUIRE(loan1 < loan2);
}

TEST_CASE("Composition over inheritance: EquipmentBundle") {
    auto laptop = std::make_shared<Laptop>(AssetTag("LP-001"), "ThinkPad T14");
    auto projector = std::make_shared<Projector>(AssetTag("PJ-001"), "Epson EB-X51");

    EquipmentBundle bundle(AssetTag("BN-001"), "Multimedia Presentation Kit");
    bundle.addItem(laptop);
    bundle.addItem(projector);

    REQUIRE(bundle.categoryName() == "EquipmentBundle");
    REQUIRE(bundle.getItems().size() == 2);

    // Sum = 50,000 + 35,000 = 85,000 -> 10% discount = 76,500
    REQUIRE(bundle.dailyFine() == 76500);
}

int main() {
    return test_runner::runAll();
}
