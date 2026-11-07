#ifndef PINJAMLAB_ASSET_TAG_HPP
#define PINJAMLAB_ASSET_TAG_HPP

#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace pinjamlab {

/**
 * @brief Immutable value object representing a campus inventory asset identifier (e.g. "LP-001").
 * 
 * Satisfies the standard equality and hash contract:
 * - Reflexive: x == x
 * - Symmetric: x == y <=> y == x
 * - Transitive: x == y and y == z => x == z
 * - Hash consistency: x == y => hash(x) == hash(y)
 */
class AssetTag {
private:
    std::string code_;

    static std::string normalize(std::string s) {
        // Trim leading and trailing whitespace
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());

        if (s.empty()) {
            throw std::invalid_argument("AssetTag cannot be empty.");
        }
        for (char &c : s) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return s;
    }

public:
    explicit AssetTag(std::string code) : code_(normalize(std::move(code))) {}

    [[nodiscard]] const std::string& getCode() const noexcept {
        return code_;
    }

    // Equality comparison
    bool operator==(const AssetTag& other) const noexcept {
        return code_ == other.code_;
    }

    bool operator!=(const AssetTag& other) const noexcept {
        return !(*this == other);
    }

    // Strict weak ordering for std::set and std::map
    bool operator<(const AssetTag& other) const noexcept {
        return code_ < other.code_;
    }

    /**
     * @brief Hand-crafted hash calculation (from-scratch primitive without external libraries).
     * Implements FNV-1a / polynomial hash combine over characters.
     */
    [[nodiscard]] std::size_t customHash() const noexcept {
        std::size_t hash_val = 14695981039346656037ULL; // FNV offset basis (64-bit)
        for (unsigned char c : code_) {
            hash_val ^= static_cast<std::size_t>(c);
            hash_val *= 1099511628211ULL; // FNV prime (64-bit)
        }
        return hash_val;
    }

    friend std::ostream& operator<<(std::ostream& os, const AssetTag& tag) {
        return os << tag.code_;
    }
};

} // namespace pinjamlab

// Template specialization for std::hash to enable use in std::unordered_set and std::unordered_map
template <>
struct std::hash<pinjamlab::AssetTag> {
    std::size_t operator()(const pinjamlab::AssetTag& tag) const noexcept {
        return tag.customHash();
    }
};

#endif // PINJAMLAB_ASSET_TAG_HPP
