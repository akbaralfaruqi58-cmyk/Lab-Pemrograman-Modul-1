"""Week 1 slide demo 2 — Python has one thing where C++ has three.

Slide 10's runnable block; kept short enough to project without scrolling.

Run:  PYTHONPATH=py python slides_demos/d2_values.py
"""
from pinjamlab.asset_tag import AssetTag
a, b = AssetTag("  lp-001  "), AssetTag("LP-001")
print("a =", a, "| b =", b)
print("a == b ?", a == b, " (same VALUE)")
print("a is b ?", a is b, " (different OBJECTS)")
c = a
print("c is a ?", c is a, " (assignment copied the handle, not the object)")

def rename(tags):
    tags.append(AssetTag("LP-999"))   # mutates the CALLER's list
shelf = [a, b]
rename(shelf)
print("shelf grew to", len(shelf), "items <- the function reached the caller's list")
