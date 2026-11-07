#include "pinjamlab/spectrophotometer.hpp"

#include <stdexcept>
#include <utility>

namespace pinjamlab {

Spectrophotometer::Spectrophotometer(
    AssetTag tag,
    std::string model,
    double minWavelengthNm,
    double maxWavelengthNm,
    double spectralBandwidthNm)
    : Equipment(std::move(tag), std::move(model)),
      minWavelengthNm_(minWavelengthNm),
      maxWavelengthNm_(maxWavelengthNm),
      spectralBandwidthNm_(spectralBandwidthNm) {
    if (minWavelengthNm_ <= 0.0) {
        throw std::invalid_argument("Minimum wavelength must be positive.");
    }
    if (maxWavelengthNm_ <= minWavelengthNm_) {
        throw std::invalid_argument("Maximum wavelength must exceed minimum wavelength.");
    }
    if (spectralBandwidthNm_ <= 0.0 || spectralBandwidthNm_ > maxWavelengthNm_ - minWavelengthNm_) {
        throw std::invalid_argument("Spectral bandwidth must be positive and fit within the wavelength range.");
    }
}

double Spectrophotometer::getMinWavelengthNm() const noexcept {
    return minWavelengthNm_;
}

double Spectrophotometer::getMaxWavelengthNm() const noexcept {
    return maxWavelengthNm_;
}

double Spectrophotometer::getSpectralBandwidthNm() const noexcept {
    return spectralBandwidthNm_;
}

int Spectrophotometer::dailyFine() const {
    return 120000; // Rp 120,000 / day, consistent with equipment.csv
}

std::string Spectrophotometer::categoryName() const {
    return "Spectrophotometer";
}

} // namespace pinjamlab
