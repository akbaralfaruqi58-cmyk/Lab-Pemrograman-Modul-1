# `slides_demos/` — the verified transcripts behind the Module 1 decks

Every `.cr-expected` block in `slides/Module1_Week{1,2,3}_slides.html` is the
output of one of these programs, produced by an actual run — never hand-written.
`MASTER_Presentation_Plan.md` §1: *if a number on a slide cannot be found in the
module or its `code/`, the slide is wrong, not the module.*

That rule earned its keep in Week 3: `c4_buckets.cpp` originally ended with a
hard-coded sentence saying every bucket held one item. The first real run showed
`MC-001` and `LP-001` colliding in bucket 0, so the program was rewritten to
*count* collisions and report what happened. The measured answer is the better
slide — a collision is where `operator==` earns its keep.

## Python (`slides_demos/*.py`)

| Script | Deck · slide | What it shows |
|---|---|---|
| `d1_invariant.py` | W1 · 8 | encapsulation: the object refuses the third loan, and what bypassing the private field costs |
| `d2_values.py` | W1 · 10 | `==` vs `is`; assignment copies the handle; a function mutates the caller's list |
| `d3_ctor.py` | W1 · 11 | constructor normalisation and rejection |
| `d4_none.py` | W1 · 6 | the Hoare hook: `None` → a located `AttributeError`, with the 3.11+ caret line |
| `d5_lifetime.py` | W1 · 12 | `__del__` timing vs C++'s closing brace; a second reference outliving the call |
| `d6_dispatch.py` | W2 · 6 | the polymorphic loop in Python — same Rp 190,000 as the C++ hero |
| `d7_abstract.py` | W2 · 4 | `abc.ABC` refusing an incomplete subclass, naming the method you dropped |
| `d8_unhashable.py` | W3 · 7 | `__eq__` with no `__hash__` → `__hash__ = None` → `TypeError: unhashable type` |
| `d9_badhash.py` | W3 · 6 | `__hash__` inconsistent with `__eq__` → a `set` of 5 where 3 was right |

## C++ (`slides_demos/cpp/*.cpp`)

| Program | Deck · slide | What it shows |
|---|---|---|
| `c1_dispatch.cpp` | W2 · 5 | **the hero**: one loop over `vector<unique_ptr<Equipment>>`, three categories, no `if` |
| `c2_vdtor.cpp` | W2 · 9, 10 | a non-virtual base destructor skipping the derived one — plus the real `-Wdelete-non-virtual-dtor` diagnostic |
| `c3_raii.cpp` | W2 · 11 | manual cleanup leaking on the throwing path; RAII not leaking on the same path |
| `c4_buckets.cpp` | W3 · 4 | the contract honoured: real FNV-1a values, real bucket indices, a real collision |
| `c5_badhash.cpp` | W3 · 5 | hash over `raw` while `==` compares `code` — a set of 5 where 3 was right, silently |
| `c6_nohash.cpp` | W3 · 7 | **does not compile, on purpose** — the 84-line template error for a missing `std::hash` |
| `c7_failing_test.cpp` | W3 · 8 | the same defect under the module's harness: two red tests, exit code 1 |

Captured transcripts live in `transcripts/`, one `.txt` per program, plus:

| Transcript | Slide | Why it is separate |
|---|---|---|
| `pytest_verbose.txt` | W1 · 14 | the suite listing, not a program's stdout |
| `c2_vdtor_diagnostic.txt` | W2 · 10 | the compiler's warning, captured from stderr |
| `c6_nohash_diagnostic.txt` | — | the **full** 84-line template error, for reference |
| `c6_nohash_head12.txt` | W3 · 7 | the same error through `head -12`, which is what the slide shows and what its command line says |

## Reproducing

```bash
cd code/Module1
for d in d1_invariant d2_values d3_ctor d4_none d5_lifetime \
         d6_dispatch d7_abstract d8_unhashable d9_badhash; do
  PYTHONPATH=py python slides_demos/$d.py > slides_demos/transcripts/$d.txt 2>&1
done
PYTHONPATH=py python -m pytest py/tests -v > slides_demos/transcripts/pytest_verbose.txt 2>&1

mkdir -p bin
for c in c1_dispatch c2_vdtor c3_raii c4_buckets c5_badhash; do
  g++ -std=c++20 -Wall -Wextra -Icpp/include slides_demos/cpp/$c.cpp -o bin/$c
  ./bin/$c > slides_demos/transcripts/$c.txt 2>&1
done
# c7 needs the harness header, and EXITS 1 on purpose:
g++ -std=c++20 -Wall -Wextra -Icpp/tests slides_demos/cpp/c7_failing_test.cpp -o bin/c7_failing_test
./bin/c7_failing_test > slides_demos/transcripts/c7_failing_test.txt 2>&1   # exit 1 expected

# Diagnostics are the artifact, so they come from stderr rather than stdout:
g++ -std=c++20 -Wall -Wextra slides_demos/cpp/c2_vdtor.cpp -o bin/c2_vdtor \
  2> slides_demos/transcripts/c2_vdtor_diagnostic.txt
g++ -std=c++20 slides_demos/cpp/c6_nohash.cpp -o /dev/null \
  2> slides_demos/transcripts/c6_nohash_diagnostic.txt            # c6 never links
g++ -std=c++20 slides_demos/cpp/c6_nohash.cpp -o /dev/null 2>&1 \
  | head -12 > slides_demos/transcripts/c6_nohash_head12.txt      # what slide 7 shows
```

Anything that varies between machines has been designed out, so a re-run should
be byte-identical: no timings, no RNG, no `id()` values, no heap addresses in
any printed `repr`.

## Keep the slide and the script in step

A projected code pane fits **about thirteen lines** before it starts scrolling
the last one out of sight, and nobody scrolls a slide. Several of these files
were shortened specifically to fit, so the block on the slide is the whole
program rather than its first two thirds. If you lengthen one, re-run the deck
check before teaching from it.

## Verified

- **Environment:** Windows 11 · Python 3.12.13 (uv-managed) · pytest 8.3.4 ·
  g++ (GCC) 15.2.0 (scoop `gcc`), `-std=c++20 -Wall -Wextra`
- **Date:** 2026-09-15
- **Result:** 9 Python demos and 5 C++ programs run clean;
  `pytest py/tests` → 9 passed; `bin/test_asset_tag` → 3 passed;
  `bin/test_domain` → 5 passed.
- **Expected non-zero exits, not failures:** `c6_nohash.cpp` never compiles and
  `c7_failing_test` exits 1. Both are the artifact. If either one starts
  succeeding, the slide that quotes it is wrong.

## In-browser execution

The Python blocks marked `data-runtime="python"` genuinely execute on the slide
via the vendored Pyodide. Each deck's bootstrap fetches the seven real files
from `py/pinjamlab/` into Pyodide's filesystem on first Run, so the browser runs
*this* package — edit a class here and the slide's behaviour changes with it.
That needs the deck served over http (`python -m http.server`); from `file://`
every block falls back to the transcripts above.

C++ blocks use `data-runtime="verified"`: there is no viable in-browser C++
toolchain (`Presentation_Plan.md` §1 item 1), so Run reveals the recorded
transcript with the beat of a live compile. `d4_none.py`'s slide is `verified`
too, deliberately — its content *is* the exact shape of CPython's traceback, and
Pyodide `exec()`s a string, so it has no source file to quote and prints the
same error without the caret line.
