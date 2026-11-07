"""Member domain entity with role-based loan quotas."""
from __future__ import annotations
from enum import Enum


class Role(Enum):
    STUDENT = 2
    FACULTY = 5
    STAFF = 3


class Member:
    """Represents a registered borrower in the campus laboratory system."""

    def __init__(self, member_id: str, name: str, role: Role) -> None:
        self._member_id: str = member_id.strip()
        self._name: str = name.strip()
        self._role: Role = role
        self._active_loans: int = 0

    @property
    def member_id(self) -> str:
        return self._member_id

    @property
    def name(self) -> str:
        return self._name

    @property
    def role(self) -> Role:
        return self._role

    @property
    def max_loans(self) -> int:
        return self._role.value

    @property
    def active_loans(self) -> int:
        return self._active_loans

    def can_borrow(self) -> bool:
        return self._active_loans < self.max_loans

    def record_loan(self) -> None:
        if not self.can_borrow():
            raise RuntimeError(
                f"Member {self._name} ({self._member_id}) has reached maximum quota ({self.max_loans})."
            )
        self._active_loans += 1

    def record_return(self) -> None:
        if self._active_loans <= 0:
            raise RuntimeError(f"Member {self._name} has no active loans to return.")
        self._active_loans -= 1

    def __repr__(self) -> str:
        return (
            f"Member(id='{self._member_id}', name='{self._name}', "
            f"role={self._role.name}, active={self._active_loans}/{self.max_loans})"
        )
