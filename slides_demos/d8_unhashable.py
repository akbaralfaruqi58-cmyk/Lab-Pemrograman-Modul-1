"""Week 3 slide demo 8 — Python's answer to a missing __hash__.

Define __eq__ and say nothing about __hash__, and Python sets __hash__ = None
for you. The class becomes unhashable: no silent duplicates, just a refusal at
the first insert. That is a different trade from C++'s, and worth naming.

Run:  PYTHONPATH=py python slides_demos/d8_unhashable.py
"""
import sys
import traceback

class PlainTag:
    def __init__(self, code: str) -> None:
        self.code = code.strip().upper()
    def __eq__(self, other: object) -> bool:
        return isinstance(other, PlainTag) and self.code == other.code

print("PlainTag('LP-001') == PlainTag('  lp-001  ') ?",
      PlainTag("LP-001") == PlainTag("  lp-001  "))
print("PlainTag.__hash__ is", PlainTag.__hash__, "  <- Python set it for you")
sys.stdout.flush()
try:
    {PlainTag("LP-001")}
except TypeError:
    traceback.print_exc(file=sys.stdout)
