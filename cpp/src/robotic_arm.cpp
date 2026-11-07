#include "pinjamlab/robotic_arm.hpp"

#include <stdexcept>
#include <utility>

namespace pinjamlab {

RoboticArm::RoboticArm(AssetTag tag, std::string model, double payloadKg, double maxReachMm)
    : Equipment(std::move(tag), std::move(model)),
      payloadKg_(payloadKg),
      maxReachMm_(maxReachMm) {
    if (payloadKg_ <= 0.0) {
        throw std::invalid_argument("Robotic arm payload must be positive.");
    }
    if (maxReachMm_ <= 0.0) {
        throw std::invalid_argument("Robotic arm maximum reach must be positive.");
    }
}

double RoboticArm::getPayloadKg() const noexcept {
    return payloadKg_;
}

double RoboticArm::getMaxReachMm() const noexcept {
    return maxReachMm_;
}

int RoboticArm::dailyFine() const {
    return 180000; // Rp 180,000 / day, consistent with equipment.csv
}

std::string RoboticArm::categoryName() const {
    return "RoboticArm";
}

} // namespace pinjamlab
