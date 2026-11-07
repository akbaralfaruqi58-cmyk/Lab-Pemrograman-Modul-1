"""Integration and domain tests for PinjamLab hierarchy and polymorphic loans."""
import pytest
from pinjamlab.asset_tag import AssetTag
from pinjamlab.equipment import Equipment, EquipmentState
from pinjamlab.laptop import Laptop
from pinjamlab.bundle import EquipmentBundle
from pinjamlab.member import Member, Role
from pinjamlab.loan import Loan


class Projector(Equipment):
    """Custom equipment subclass for test and original artifact verification."""

    def __init__(self, tag: AssetTag, model: str, lumens: int = 3600) -> None:
        super().__init__(tag, model)
        self.lumens = lumens

    def daily_fine(self) -> int:
        return 35000

    def category_name(self) -> str:
        return "Projector"


def test_abstract_class_cannot_be_instantiated():
    with pytest.raises(TypeError):
        Equipment(AssetTag("XX-001"), "Generic Tool")  # type: ignore


def test_polymorphic_daily_fine_and_dispatch():
    laptop = Laptop(AssetTag("LP-001"), "ThinkPad T14", ram_gb=32)
    projector = Projector(AssetTag("PJ-001"), "Epson EB-X51", lumens=3800)

    assert laptop.daily_fine() == 50000
    assert projector.daily_fine() == 35000

    items: list[Equipment] = [laptop, projector]
    fines = [item.daily_fine() for item in items]
    assert fines == [50000, 35000]
    assert sum(fines) == 85000


def test_equipment_state_transitions():
    laptop = Laptop(AssetTag("LP-001"), "ThinkPad T14")
    assert laptop.state == EquipmentState.AVAILABLE

    laptop.mark_loaned()
    assert laptop.state == EquipmentState.LOANED

    with pytest.raises(RuntimeError):
        laptop.mark_loaned()  # Already loaned

    laptop.mark_returned()
    assert laptop.state == EquipmentState.AVAILABLE


def test_member_quota_encapsulation():
    student = Member("M-101", "Budi Santoso", Role.STUDENT)
    assert student.max_loans == 2
    assert student.active_loans == 0
    assert student.can_borrow() is True

    student.record_loan()
    student.record_loan()
    assert student.active_loans == 2
    assert student.can_borrow() is False

    with pytest.raises(RuntimeError):
        student.record_loan()  # Exceeds quota

    student.record_return()
    assert student.can_borrow() is True


def test_loan_fine_calculation_and_ordering():
    laptop = Laptop(AssetTag("LP-001"), "ThinkPad T14")
    student = Member("M-101", "Budi Santoso", Role.STUDENT)

    loan1 = Loan("L-001", laptop, student, days_loaned=7, overdue_days=3)
    # Fine = 3 days * 50,000 = 150,000
    assert loan1.calculate_fine() == 150000

    projector = Projector(AssetTag("PJ-001"), "Epson EB-X51")
    loan2 = Loan("L-002", projector, student, days_loaned=3, overdue_days=2)
    # Fine = 2 days * 35,000 = 70,000
    assert loan2.calculate_fine() == 70000

    # Total ordering by loan_id
    assert loan1 < loan2
    loans = [loan2, loan1]
    loans.sort()
    assert loans == [loan1, loan2]


def test_composition_over_inheritance_bundle():
    laptop = Laptop(AssetTag("LP-001"), "ThinkPad T14")
    projector = Projector(AssetTag("PJ-001"), "Epson EB-X51")

    bundle = EquipmentBundle(AssetTag("BN-001"), "Presentation Kit", [laptop, projector])
    assert bundle.category_name() == "EquipmentBundle"
    assert len(bundle.items) == 2
    # 50,000 + 35,000 = 85,000 -> 10% discount = 76,500
    assert bundle.daily_fine() == 76500
