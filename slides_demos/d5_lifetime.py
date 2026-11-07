"""Week 1 slide demo 5 — when does the object actually go away?

C++ destroys a stack object at the closing brace, deterministically.
CPython destroys when the last reference drops -- which LOOKS deterministic
until a second name keeps the object alive. That is the whole lesson.

Run:  PYTHONPATH=py python slides_demos/d5_lifetime.py
"""


class Tracked:
    def __init__(self, name):
        self.name = name
        print(f"  + {self.name} constructed")

    def __del__(self):
        print(f"  - {self.name} released")


def scope_a():
    print("enter scope_a")
    Tracked("A")                 # nothing holds it: released immediately
    print("leave scope_a")


def scope_b():
    print("enter scope_b")
    t = Tracked("B")             # one name holds it: released at function exit
    print("leave scope_b")


keep = []


def scope_c():
    print("enter scope_c")
    t = Tracked("C")
    keep.append(t)               # a second reference outlives the function
    print("leave scope_c")


scope_a()
scope_b()
scope_c()
print("after scope_c, still alive:", [t.name for t in keep])
keep.clear()
print("keep.clear() ran")
