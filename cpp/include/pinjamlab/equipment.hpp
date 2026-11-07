#ifndef PINJAMLAB_EQUIPMENT_HPP
#define PINJAMLAB_EQUIPMENT_HPP

#include "asset_tag.hpp"
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace pinjamlab {

enum class EquipmentState {
    AVAILABLE,
    LOANED,
    MAINTENANCE,
    DECOMMISSIONED
};

inline const char* toString(EquipmentState state) {
    switch (state) {
        case EquipmentState::AVAILABLE:      return "AVAILABLE";
        case EquipmentState::LOANED:         return "LOANED";
        case EquipmentState::MAINTENANCE:    return "MAINTENANCE";
        case EquipmentState::DECOMMISSIONED: return "DECOMMISSIONED";
    }
    return "UNKNOWN";
}

/**
 * @brief Abstract base class representing any campus laboratory equipment.
 * 
 * Defines the public interface contract and enforces virtual destruction
 * to prevent object slicing and undefined behaviour when releasing derived objects.
 */
class Equipment {
protected:
    AssetTag tag_;
    std::string model_;
    EquipmentState state_;

public:
    Equipment(AssetTag tag, std::string model)
        : tag_(std::move(tag)), model_(std::move(model)), state_(EquipmentState::AVAILABLE) {
        if (model_.empty()) {
            throw std::invalid_argument("Equipment model name cannot be empty.");
        }
    }

    // MANDATORY VIRTUAL DESTRUCTOR: Ensures derived destructors run via base pointer
    virtual ~Equipment() = default;

    // Rule of 5: Prevent accidental slicing via copy
    Equipment(const Equipment&) = default;
    Equipment& operator=(const Equipment&) = default;
    Equipment(Equipment&&) noexcept = default;
    Equipment& operator=(Equipment&&) noexcept = default;

    [[nodiscard]] const AssetTag& getTag() const noexcept { return tag_; }
    [[nodiscard]] const std::string& getModel() const noexcept { return model_; }
    [[nodiscard]] EquipmentState getState() const noexcept { return state_; }

    void markLoaned() {
        if (state_ != EquipmentState::AVAILABLE) {
            throw std::runtime_error("Cannot loan item " + tag_.getCode() + ": state is " + toString(state_));
        }
        state_ = EquipmentState::LOANED;
    }

    void markReturned() {
        if (state_ != EquipmentState::LOANED) {
            throw std::runtime_error("Cannot return item " + tag_.getCode() + ": state is " + toString(state_));
        }
        state_ = EquipmentState::AVAILABLE;
    }

    void markMaintenance() noexcept {
        state_ = EquipmentState::MAINTENANCE;
    }

    // Pure virtual functions: derived classes must provide concrete implementations
    [[nodiscard]] virtual int dailyFine() const = 0;
    [[nodiscard]] virtual std::string categoryName() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Equipment& eq) {
        return os << eq.categoryName() << "(Tag: " << eq.tag_ << ", Model: " << eq.model_
                  << ", State: " << toString(eq.state_) << ", Fine: Rp" << eq.dailyFine() << "/day)";
    }
};

} // namespace pinjamlab

#endif // PINJAMLAB_EQUIPMENT_HPP
