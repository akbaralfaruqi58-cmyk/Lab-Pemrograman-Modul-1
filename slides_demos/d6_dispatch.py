"""Week 2 slide demo 6 — the same polymorphic loop, in Python.

Identical shelf, identical numbers, identical total as the C++ hero
(slides_demos/cpp/c1_dispatch.cpp). The point of the pair is that the
NUMBERS match and the MECHANISM does not.

The statements below are exactly what slide 6 runs, in the same order and
with the same blank-line-free shape — a projected code pane fits about
thirteen lines, so keep the two in step when you edit either.

Run:  PYTHONPATH=py python slides_demos/d6_dispatch.py
"""
from pinjamlab.asset_tag import AssetTag
from pinjamlab.bundle import EquipmentBundle
from pinjamlab.laptop import Laptop
kit = EquipmentBundle(AssetTag("KIT-01"), "Field Survey Kit")
kit.add_item(Laptop(AssetTag("LP-003"), "ProBook 450"))
kit.add_item(Laptop(AssetTag("LP-004"), "MacBook Air"))
shelf = [Laptop(AssetTag("LP-001"), "ThinkPad T14"),
         Laptop(AssetTag("LP-002"), "Latitude 5440", ram_gb=32), kit]
total = 0
for item in shelf:                    # no vtable, no pointer anywhere
    total += item.daily_fine()
    print(f"{str(item.tag):<9}{item.category_name():<19}{item.daily_fine():>6}")
print(f"{'TOTAL, 1 day overdue':<28}{total:>6}")
