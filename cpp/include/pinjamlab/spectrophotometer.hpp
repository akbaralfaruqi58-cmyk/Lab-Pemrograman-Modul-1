#ifndef PINJAMLAB_SPECTROPHOTOMETER_HPP
#define PINJAMLAB_SPECTROPHOTOMETER_HPP

#include "equipment.hpp"
#include <string>

namespace pinjamlab {

/**
 * @brief UV-Vis spectrophotometer used for laboratory measurements.
 *
 * Specifications chosen for this lab: wavelength range 190–1100 nm and
 * spectral bandwidth 1 nm. The daily overdue fine matches equipment.csv.
 */
class Spectrophotometer final : public Equipment {
private:
    double minWavelengthNm_;
    double maxWavelengthNm_;
    double spectralBandwidthNm_;

public:
    Spectrophotometer(
        AssetTag tag,
        std::string model,
        double minWavelengthNm = 190.0,
        double maxWavelengthNm = 1100.0,
        double spectralBandwidthNm = 1.0);

    [[nodiscard]] double getMinWavelengthNm() const noexcept;
    [[nodiscard]] double getMaxWavelengthNm() const noexcept;
    [[nodiscard]] double getSpectralBandwidthNm() const noexcept;

    [[nodiscard]] int dailyFine() const override;
    [[nodiscard]] std::string categoryName() const override;
};

} // namespace pinjamlab

#endif // PINJAMLAB_SPECTROPHOTOMETER_HPP
