#ifndef PINJAMLAB_MEMBER_HPP
#define PINJAMLAB_MEMBER_HPP

#include <string>
#include <stdexcept>
#include <iostream>

namespace pinjamlab {

enum class Role {
    STUDENT = 2,
    FACULTY = 5,
    STAFF = 3
};

inline const char* toString(Role role) {
    switch (role) {
        case Role::STUDENT: return "STUDENT";
        case Role::FACULTY: return "FACULTY";
        case Role::STAFF:   return "STAFF";
    }
    return "UNKNOWN";
}

class Member {
private:
    std::string memberId_;
    std::string name_;
    Role role_;
    int activeLoans_;

public:
    Member(std::string memberId, std::string name, Role role)
        : memberId_(std::move(memberId)), name_(std::move(name)), role_(role), activeLoans_(0) {
        if (memberId_.empty() || name_.empty()) {
            throw std::invalid_argument("Member ID and Name cannot be empty.");
        }
    }

    [[nodiscard]] const std::string& getMemberId() const noexcept { return memberId_; }
    [[nodiscard]] const std::string& getName() const noexcept { return name_; }
    [[nodiscard]] Role getRole() const noexcept { return role_; }
    [[nodiscard]] int getMaxLoans() const noexcept { return static_cast<int>(role_); }
    [[nodiscard]] int getActiveLoans() const noexcept { return activeLoans_; }

    [[nodiscard]] bool canBorrow() const noexcept {
        return activeLoans_ < getMaxLoans();
    }

    void recordLoan() {
        if (!canBorrow()) {
            throw std::runtime_error("Member " + name_ + " (" + memberId_ + ") has reached loan quota limit.");
        }
        ++activeLoans_;
    }

    void recordReturn() {
        if (activeLoans_ <= 0) {
            throw std::runtime_error("Member " + name_ + " has no active loans to return.");
        }
        --activeLoans_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Member& m) {
        return os << "Member(" << m.memberId_ << ", " << m.name_ << ", " << toString(m.role_)
                  << ", Active: " << m.activeLoans_ << "/" << m.getMaxLoans() << ")";
    }
};

} // namespace pinjamlab

#endif // PINJAMLAB_MEMBER_HPP
