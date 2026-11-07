#ifndef PINJAMLAB_LOAN_HPP
#define PINJAMLAB_LOAN_HPP

#include "equipment.hpp"
#include "member.hpp"
#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>

namespace pinjamlab {

class Loan {
private:
    std::string loanId_;
    std::shared_ptr<Equipment> equipment_;
    std::shared_ptr<Member> member_;
    int daysLoaned_;
    int overdueDays_;

public:
    Loan(std::string loanId, std::shared_ptr<Equipment> equipment, std::shared_ptr<Member> member,
         int daysLoaned, int overdueDays = 0)
        : loanId_(std::move(loanId)),
          equipment_(std::move(equipment)),
          member_(std::move(member)),
          daysLoaned_(daysLoaned),
          overdueDays_(overdueDays) {
        if (!equipment_) {
            throw std::invalid_argument("Equipment cannot be null.");
        }
        if (!member_) {
            throw std::invalid_argument("Member cannot be null.");
        }
        if (daysLoaned_ <= 0) {
            throw std::invalid_argument("daysLoaned must be strictly positive.");
        }
        if (overdueDays_ < 0) {
            throw std::invalid_argument("overdueDays cannot be negative.");
        }
    }

    [[nodiscard]] const std::string& getLoanId() const noexcept { return loanId_; }
    [[nodiscard]] const Equipment& getEquipment() const noexcept { return *equipment_; }
    [[nodiscard]] const Member& getMember() const noexcept { return *member_; }
    [[nodiscard]] int getDaysLoaned() const noexcept { return daysLoaned_; }
    [[nodiscard]] int getOverdueDays() const noexcept { return overdueDays_; }

    void setOverdueDays(int days) {
        if (days < 0) {
            throw std::invalid_argument("Overdue days cannot be negative.");
        }
        overdueDays_ = days;
    }

    /**
     * @brief Computes total overdue fine via dynamic dispatch to equipment_->dailyFine().
     */
    [[nodiscard]] int calculateFine() const {
        return overdueDays_ * equipment_->dailyFine();
    }

    // Equality and ordering by loanId
    bool operator==(const Loan& other) const noexcept {
        return loanId_ == other.loanId_;
    }

    bool operator<(const Loan& other) const noexcept {
        return loanId_ < other.loanId_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Loan& loan) {
        return os << "Loan(" << loan.loanId_ << ", Item: " << loan.equipment_->getTag()
                  << ", Member: " << loan.member_->getName()
                  << ", Overdue: " << loan.overdueDays_ << " days, Fine: Rp" << loan.calculateFine() << ")";
    }
};

} // namespace pinjamlab

#endif // PINJAMLAB_LOAN_HPP
