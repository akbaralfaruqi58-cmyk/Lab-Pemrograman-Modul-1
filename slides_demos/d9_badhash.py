"""Week 3 slide demo 9 — the same broken contract, in Python.

__eq__ compares the NORMALISED code; __hash__ hashes the RAW string. Rule 5
is false, and Python is exactly as silent about it as C++ was in c5_badhash.
Then the real AssetTag, which keeps the contract, folds the same five inputs
into three.

Run:  PYTHONPATH=py python slides_demos/d9_badhash.py
"""
from pinjamlab.asset_tag import AssetTag

class BadTag:
    def __init__(self, raw: str) -> None:
        self.raw, self.code = raw, raw.strip().upper()
    def __eq__(self, o: object) -> bool:
        return isinstance(o, BadTag) and self.code == o.code
    def __hash__(self) -> int:
        return hash(self.raw)          # <- the bug: raw, not code

RAW = ["LP-001", "  lp-001  ", "Lp-001", "PJ-001", "MC-001"]
a, b = BadTag("LP-001"), BadTag("  lp-001  ")
print("a == b ?", a == b, "| hash(a) == hash(b) ?", hash(a) == hash(b))
print("BadTag  set size:", len({BadTag(s) for s in RAW}), " (expected 3)")
print("AssetTag set size:", len({AssetTag(s) for s in RAW}), " (expected 3)")
