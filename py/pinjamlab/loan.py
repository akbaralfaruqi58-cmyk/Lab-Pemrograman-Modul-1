"""Loan transaction entity linking Member and Equipment with fine calculation."""
from __future__ import annotations
from functools import total_ordering
from .equipment import Equipment
from .member import Member


@total_ordering
class Loan:
    """Represents an active or historical loan record.

    Honours the total ordering contract based on loan_id.
    """

    def __init__(
        self,
        loan_id: str,
        equipment: Equipment,
        member: Member,
        days_loaned: int,
        overdue_days: int = 0,
    ) -> None:
        if days_loaned <= 0:
            raise ValueError("days_loaned must be strictly positive.")
        if overdue_days < 0:
            raise ValueError("overdue_days cannot be negative.")

        self._loan_id: str = loan_id
        self._equipment: Equipment = equipment
        self._member: Member = member
        self._days_loaned: int = days_loaned
        self._overdue_days: int = overdue_days

    @property
    def loan_id(self) -> str:
        return self._loan_id

    @property
    def equipment(self) -> Equipment:
        return self._equipment

    @property
    def member(self) -> Member:
        return self._member

    @property
    def days_loaned(self) -> int:
        return self._days_loaned

    @property
    def overdue_days(self) -> int:
        return self._overdue_days

    def set_overdue_days(self, days: int) -> None:
        if days < 0:
            raise ValueError("Overdue days cannot be negative.")
        self._overdue_days = days

    def calculate_fine(self) -> int:
        """Polymorphically delegates fine rate to the associated equipment."""
        return self._overdue_days * self._equipment.daily_fine()

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Loan):
            return NotImplemented
        return self._loan_id == other._loan_id

    def __lt__(self, other: object) -> bool:
        if not isinstance(other, Loan):
            return NotImplemented
        return self._loan_id < other._loan_id

    def __hash__(self) -> int:
        return hash(self._loan_id)

    def __repr__(self) -> str:
        return (
            f"Loan(id='{self._loan_id}', item={self._equipment.tag}, "
            f"member='{self._member.name}', overdue={self._overdue_days}d, "
            f"fine=Rp{self.calculate_fine():,})"
        )
