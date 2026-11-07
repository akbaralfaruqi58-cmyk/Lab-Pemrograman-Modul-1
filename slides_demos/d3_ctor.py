"""Week 1 slide demo 3 — the constructor is where the invariant is established.

Slide 11's runnable block; kept short enough to project without scrolling.

Run:  PYTHONPATH=py python slides_demos/d3_ctor.py
"""
from pinjamlab.asset_tag import AssetTag
from pinjamlab.laptop import Laptop

for raw in ["LP-001", "  lp-001  ", "Lp-001", "", "   "]:
    try:
        print(f"AssetTag({raw!r}) -> {AssetTag(raw)}")
    except ValueError as e:
        print(f"AssetTag({raw!r}) -> REFUSED: {e}")
try:
    Laptop(AssetTag("LP-002"), "ThinkPad T14", ram_gb=0)
except ValueError as e:
    print("Laptop(ram_gb=0) -> REFUSED:", e)
ok = Laptop(AssetTag("LP-002"), "ThinkPad T14", ram_gb=16)
print("constructed ->", ok, "| Rp", format(ok.daily_fine(), ","))
