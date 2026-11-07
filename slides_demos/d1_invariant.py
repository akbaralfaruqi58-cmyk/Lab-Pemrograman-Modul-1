"""Week 1 slide demo 1 — a class is a boundary of trust.

Kept to thirteen statements on purpose: this is slide 8's runnable block,
and a projected code pane fits about that many lines before it starts
scrolling the last one out of sight.

Run:  PYTHONPATH=py python slides_demos/d1_invariant.py
"""
from pinjamlab.member import Member, Role

ayu = Member("M-101", "Ayu Lestari", Role.STUDENT)
print(ayu, "| quota:", ayu.max_loans)
ayu.record_loan()
ayu.record_loan()
print("after 2 loans ->", ayu, "| can borrow a 3rd?", ayu.can_borrow())
try:
    ayu.record_loan()
except RuntimeError as e:
    print("REFUSED:", e)
ayu._active_loans = 0       # never do this; shown to prove the point
print("bypassed ->", ayu, "<- the object is now lying about itself")
