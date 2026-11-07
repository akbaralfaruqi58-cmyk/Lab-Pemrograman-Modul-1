#ifndef PINJAMLAB_ROBOTIC_ARM_HPP
#define PINJAMLAB_ROBOTIC_ARM_HPP

#include "equipment.hpp"
#include <string>

namespace pinjamlab {

/**
 * @brief Robotic arm used for automation and robotics laboratory practice.
 *
 * Specifications chosen for this lab: 10 kg payload and 1300 mm maximum reach.
 */
class RoboticArm final : public Equipment {
private:
    double payloadKg_;
    double maxReachMm_;

public:
    RoboticArm(
        AssetTag tag,
        std::string model,
        double payloadKg = 10.0,
        double maxReachMm = 1300.0);

    [[nodiscard]] double getPayloadKg() const noexcept;
    [[nodiscard]] double getMaxReachMm() const noexcept;

    [[nodiscard]] int dailyFine() const override;
    [[nodiscard]] std::string categoryName() const override;
};

} // namespace pinjamlab

#endif // PINJAMLAB_ROBOTIC_ARM_HPP
