#ifndef PINJAMLAB_BUNDLE_HPP
#define PINJAMLAB_BUNDLE_HPP

#include "equipment.hpp"
#include <vector>
#include <memory>
#include <numeric>

namespace pinjamlab {

/**
 * @brief Composite equipment bundle demonstrating composition over inheritance.
 * Holds multiple Equipment instances (via std::unique_ptr or shared references)
 * and delegates the dailyFine() calculation to its constituent elements.
 */
class EquipmentBundle : public Equipment {
private:
    std::vector<std::shared_ptr<Equipment>> items_;

public:
    EquipmentBundle(AssetTag tag, std::string bundleName)
        : Equipment(std::move(tag), std::move(bundleName)) {}

    void addItem(std::shared_ptr<Equipment> item) {
        if (!item) {
            throw std::invalid_argument("Cannot add null item to EquipmentBundle.");
        }
        items_.push_back(std::move(item));
    }

    [[nodiscard]] const std::vector<std::shared_ptr<Equipment>>& getItems() const noexcept {
        return items_;
    }

    [[nodiscard]] int dailyFine() const override {
        int sum = 0;
        for (const auto& item : items_) {
            sum += item->dailyFine();
        }
        // 10% bundle package discount
        return static_cast<int>(sum * 0.9);
    }

    [[nodiscard]] std::string categoryName() const override {
        return "EquipmentBundle";
    }
};

} // namespace pinjamlab

#endif // PINJAMLAB_BUNDLE_HPP
