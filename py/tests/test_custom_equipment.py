"""Unit tests for the original Spectrophotometer and RoboticArm subclasses."""
import pytest

from pinjamlab.asset_tag import AssetTag
from pinjamlab.equipment import Equipment
from pinjamlab.robotic_arm import RoboticArm
from pinjamlab.spectrophotometer import Spectrophotometer


def test_spectrophotometer_overrides_contract_and_stores_specifications():
    spectro = Spectrophotometer(AssetTag("SP-001"), "Spectro-UV-Vis-01")

    assert isinstance(spectro, Equipment)
    assert spectro.category_name() == "Spectrophotometer"
    assert spectro.daily_fine() == 120000
    assert spectro.min_wavelength_nm == 190.0
    assert spectro.max_wavelength_nm == 1100.0
    assert spectro.spectral_bandwidth_nm == 1.0


def test_spectrophotometer_accepts_custom_specifications():
    spectro = Spectrophotometer(
        AssetTag("SP-002"), "Spectro-custom", 200.0, 900.0, 2.0
    )
    assert spectro.min_wavelength_nm == 200.0
    assert spectro.max_wavelength_nm == 900.0
    assert spectro.spectral_bandwidth_nm == 2.0


@pytest.mark.parametrize(
    "min_nm,max_nm,bandwidth_nm",
    [(0.0, 1100.0, 1.0), (1100.0, 190.0, 1.0), (190.0, 1100.0, 0.0)],
)
def test_spectrophotometer_rejects_invalid_specifications(min_nm, max_nm, bandwidth_nm):
    with pytest.raises(ValueError):
        Spectrophotometer(
            AssetTag("SP-ERR"), "Invalid spectro", min_nm, max_nm, bandwidth_nm
        )


def test_robotic_arm_overrides_contract_and_stores_specifications():
    arm = RoboticArm(AssetTag("RA-001"), "RoboticArm-01")

    assert isinstance(arm, Equipment)
    assert arm.category_name() == "RoboticArm"
    assert arm.daily_fine() == 180000
    assert arm.payload_kg == 10.0
    assert arm.max_reach_mm == 1300.0


def test_robotic_arm_accepts_custom_specifications():
    arm = RoboticArm(
        AssetTag("RA-002"), "RoboticArm-custom", payload_kg=8.0, max_reach_mm=900.0
    )
    assert arm.payload_kg == 8.0
    assert arm.max_reach_mm == 900.0


@pytest.mark.parametrize("payload_kg,max_reach_mm", [(0.0, 1300.0), (10.0, 0.0)])
def test_robotic_arm_rejects_invalid_specifications(payload_kg, max_reach_mm):
    with pytest.raises(ValueError):
        RoboticArm(AssetTag("RA-ERR"), "Invalid arm", payload_kg, max_reach_mm)


def test_dynamic_dispatch_works_for_both_new_subclasses():
    inventory: list[Equipment] = [
        Spectrophotometer(AssetTag("SP-010"), "Spectro"),
        RoboticArm(AssetTag("RA-010"), "Robot"),
    ]

    assert [item.category_name() for item in inventory] == [
        "Spectrophotometer",
        "RoboticArm",
    ]
    assert [item.daily_fine() for item in inventory] == [120000, 180000]
    assert sum(item.daily_fine() for item in inventory) == 300000
