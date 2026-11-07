"""Week 1 slide demo 4 — Hoare's mistake, in the language that survives it.

The same missing object that is undefined behaviour in C++ is a named,
located, immediate exception in Python. That difference is the hook slide.

Run:  PYTHONPATH=py python slides_demos/d4_none.py
"""
import sys
import traceback

from pinjamlab.asset_tag import AssetTag
from pinjamlab.laptop import Laptop

shelf = {"LP-001": Laptop(AssetTag("LP-001"), "ThinkPad T14")}

found = shelf.get("LP-404")          # not there -> None, not a wild pointer
print("lookup LP-404 ->", found)
sys.stdout.flush()

try:
    print(found.daily_fine())
except AttributeError:
    traceback.print_exc(file=sys.stdout)

print()
print("Python stopped at the line that was wrong.")
print("C++ would have dereferenced whatever that address happened to hold.")
