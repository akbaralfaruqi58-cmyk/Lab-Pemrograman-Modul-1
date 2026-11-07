#ifndef PINJAMLAB_LAPTOP_HPP
#define PINJAMLAB_LAPTOP_HPP

#include "equipment.hpp"

namespace pinjamlab {

class Laptop : public Equipment {
private:
    int ramGb_;
    std::string processor_;

public:
    Laptop(AssetTag tag, std::string model, int ramGb = 16, std::string processor = "Intel Core i7")
        : Equipment(std::move(tag), std::move(model)), ramGb_(ramGb), processor_(std::move(processor)) {
        if (ramGb_ <= 0) {
            throw std::invalid_argument("RAM must be strictly positive.");
        }
    }

    [[nodiscard]] int getRamGb() const noexcept { return ramGb_; }
    [[nodiscard]] const std::string& getProcessor() const noexcept { return processor_; }

    // Override pure virtual methods
    [[nodiscard]] int dailyFine() const override {
        return 50000; // Rp 50,000 / day
    }

    [[nodiscard]] std::string categoryName() const override {
        return "Laptop";
    }
};

} // namespace pinjamlab

#endif // PINJAMLAB_LAPTOP_HPP
