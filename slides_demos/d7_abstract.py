"""Week 2 slide demo 7 — the contract, and when each language checks it.

C++ refuses at COMPILE time: a class with an unimplemented pure virtual is
abstract, and `Equipment e;` does not build. Python refuses at INSTANTIATION
— later than C++, but still before any caller can touch the object.

Kept short on purpose: this is slide 4's runnable block, and a projected
code pane fits about thirteen lines.

Run:  PYTHONPATH=py python slides_demos/d7_abstract.py
"""
from pinjamlab.asset_tag import AssetTag
from pinjamlab.equipment import Equipment


class Microscope(Equipment):        # your own category from Lab 1
    def daily_fine(self) -> int:
        return 75000                # ...and no category_name()


for cls in (Equipment, Microscope):
    try:
        obj = cls(AssetTag("MC-001"), "Olympus CX23")
        print(f"{cls.__name__:<11} -> OK: Rp {obj.daily_fine():,}/day")
    except TypeError as e:
        print(f"{cls.__name__:<11} -> REFUSED: {e}")
