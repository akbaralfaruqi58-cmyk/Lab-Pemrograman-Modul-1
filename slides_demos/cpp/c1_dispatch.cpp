// Week 2 slide demo 1 -- THE HERO: one loop, three categories, no if/else.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra -Icpp/include
//       slides_demos/cpp/c1_dispatch.cpp -o bin/c1_dispatch
//   ./bin/c1_dispatch
//
// The transcript this prints is what the Week 2 deck shows as .cr-expected.

#include "pinjamlab/bundle.hpp"
#include "pinjamlab/laptop.hpp"
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

using namespace pinjamlab;

int main() {
    // The vector owns its elements. Each unique_ptr owns exactly one object,
    // and the static type of every element is the SAME: Equipment.
    std::vector<std::unique_ptr<Equipment>> shelf;

    shelf.push_back(std::make_unique<Laptop>(AssetTag{"LP-001"}, "ThinkPad T14"));
    shelf.push_back(std::make_unique<Laptop>(AssetTag{"LP-002"}, "Latitude 5440", 32));

    auto kit = std::make_unique<EquipmentBundle>(AssetTag{"KIT-01"}, "Field Survey Kit");
    kit->addItem(std::make_shared<Laptop>(AssetTag{"LP-003"}, "ProBook 450"));
    kit->addItem(std::make_shared<Laptop>(AssetTag{"LP-004"}, "MacBook Air"));
    shelf.push_back(std::move(kit));

    std::cout << "tag      category           Rp/day\n";
    std::cout << "-------  -----------------  ------\n";

    int total = 0;
    for (const auto& item : shelf) {
        // ONE call site. Which dailyFine() runs is decided at RUN time, by the
        // vptr inside the object -- not by the type of the pointer holding it.
        const int fine = item->dailyFine();
        total += fine;
        std::cout << std::left << std::setw(9) << item->getTag().getCode()
                  << std::setw(19) << item->categoryName()
                  << std::right << std::setw(6) << fine << '\n';
    }

    std::cout << "-------  -----------------  ------\n";
    std::cout << std::left << std::setw(28) << "TOTAL, 1 day overdue"
              << std::right << std::setw(6) << total << '\n';

    // The bundle's 100000 is 2 x 50000 minus the 10% package discount.
    std::cout << "\nadd one category tomorrow: this loop does not change.\n";
    return 0;
}
