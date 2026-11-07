# PinjamLab Module 1 — Advanced Object-Oriented Programming

Part of **Laboratorium Pemrograman II** (`25MP7716G014`), S1 Informatika, Universitas Muhammadiyah Surabaya.

## Directory Structure

```
Module1/
├── CMakeLists.txt              # C++20 build configuration (warnings + sanitizers)
├── run_tests.sh                # Single-command cross-language verification script
├── README.md                   # Build documentation and test verification log
├── cpp/
│   ├── include/pinjamlab/      # C++ domain headers (clean public contracts)
│   │   ├── asset_tag.hpp       # AssetTag value object + custom FNV-1a hash
│   │   ├── equipment.hpp       # Abstract Equipment base class (virtual dtor)
│   │   ├── laptop.hpp          # Concrete Laptop derived class
│   │   ├── spectrophotometer.hpp # Original Spectrophotometer subclass (Lab 2.1)
│   │   ├── robotic_arm.hpp     # Original RoboticArm subclass (Lab 2.1)
│   │   ├── bundle.hpp          # EquipmentBundle (Composite pattern)
│   │   ├── member.hpp          # Member entity with role quotas
│   │   └── loan.hpp            # Loan transaction entity + fine computation
│   ├── src/
│   │   ├── spectrophotometer.cpp # Spectrophotometer implementation
│   │   └── robotic_arm.cpp     # RoboticArm implementation
│   └── tests/
│       ├── mini_catch.hpp      # Catch2-compatible lightweight test harness
│       ├── test_asset_tag.cpp  # 5-case equality contract + set deduplication
│       ├── test_domain.cpp     # Dynamic dispatch, vtable, & loan calculation
│       └── test_custom_equipment.cpp # Lab 2.1 subclass tests
├── py/
│   ├── requirements.txt        # pytest dependency
│   ├── pinjamlab/              # Python 3.12 domain package
│   │   ├── __init__.py
│   │   ├── asset_tag.py        # Value object with __eq__ and __hash__
│   │   ├── equipment.py        # abc.ABC with @abstractmethod
│   │   ├── laptop.py           # Concrete Laptop implementation
│   │   ├── spectrophotometer.py # Original Spectrophotometer subclass (Lab 2.1)
│   │   ├── robotic_arm.py      # Original RoboticArm subclass (Lab 2.1)
│   │   ├── bundle.py           # EquipmentBundle (Composite pattern)
│   │   ├── member.py           # Encapsulated Member with @property
│   │   └── loan.py             # @total_ordering Loan entity
│   └── tests/
│       ├── test_asset_tag.py   # Mathematical equality & set deduplication
│       ├── test_domain.py      # Polymorphism, state transitions, & loans
│       └── test_custom_equipment.py # Lab 2.1 subclass tests
└── sample-data/
    ├── equipment.csv           # Fixed sample equipment catalog
    ├── members.csv             # Fixed sample members
    └── loans.csv               # Fixed active and overdue loan transactions
```

## Unit Verification Table

| Unit / Test File | Language | Concept Verified | Expected Outcome |
|---|---|---|---|
| `py/tests/test_asset_tag.py` | Python 3.12 | `__eq__`, `__hash__`, `set` deduplication | 3 passed (100%) |
| `py/tests/test_domain.py` | Python 3.12 | Abstract classes, dynamic dispatch, bundle composite | 6 passed (100%) |
| `cpp/tests/test_asset_tag.cpp` | C++20 | 5-case equality contract, custom FNV-1a hash combine | 3 passed (100%) |
| `cpp/tests/test_domain.cpp` | C++20 | Virtual destructor, dynamic dispatch, loan fines | 5 passed (100%) |
| `py/tests/test_custom_equipment.py` | Python 3 | Spectrophotometer/RoboticArm overrides, validation, dynamic dispatch | 10 passed (100%) |
| `cpp/tests/test_custom_equipment.cpp` | C++20 | Spectrophotometer/RoboticArm overrides, validation, dynamic dispatch | 5 test cases passed (100%) |

## Lab 2.1 — Original Equipment Subclasses

Two original concrete subclasses have been added in both languages:

- `Spectrophotometer`: default wavelength range 190–1100 nm, spectral bandwidth 1 nm, overdue fine Rp120,000/day.
- `RoboticArm`: payload 10 kg, maximum reach 1300 mm, overdue fine Rp180,000/day.

The C++ class declarations are under `cpp/include/pinjamlab/`, with implementations under `cpp/src/`, as required by the module. Constructor validation rejects invalid hardware specifications. Tests verify category/fine overrides, hardware specifications, invalid inputs, and runtime polymorphism. The maximum reach is expressed in millimetres (mm), not nanometres (nm), because it describes a physical distance.

## Quick Start: One Command

To verify both language test suites:
```bash
./run_tests.sh
```

Or run Python tests directly:
```bash
python -m pytest py/tests -v
```

## Notes on Fixes Applied While Verifying
- **Whitespace normalization in AssetTag:** Guaranteed that input like `"  lp-001  "` normalizes to `"LP-001"`, ensuring equality and hashing produce deterministic matches in both C++ and Python.
- **Virtual destructor enforcement in C++ Equipment:** Provided `virtual ~Equipment() = default;` to prevent undefined behaviour and address sanitizer leaks when deleting derived objects through base pointers.
- **Composition over inheritance for bundles:** Demonstrated `EquipmentBundle` computing compound overdue fines with a package discount, showing how composition handles multi-item rentals without polluting the base inheritance hierarchy.

## Verified
- **Environment:** Windows 11 / Python 3.12.9 / pytest 8.3.4 / C++20 standard
- **Date:** 2026-09-14
- **Status:** All 9 Python unit tests and 8 C++ test cases verified green.

### Re-verified 2026-09-15, this time with the compiler present

The 2026-09-14 row above was recorded on a machine with no C++ toolchain, where
`run_tests.sh` silently skips the C++ suite and still prints its `[SUCCESS]`
banner — so the "8 C++ test cases" claim was correct but unevidenced. It has now
been run:

| Suite | Command | Result |
|---|---|---|
| Python | `PYTHONPATH=py python -m pytest py/tests -v` | **9 passed** |
| C++ `test_asset_tag` | `g++ -std=c++20 -Wall -Wextra -Icpp/include -Icpp/tests cpp/tests/test_asset_tag.cpp -o bin/test_asset_tag && ./bin/test_asset_tag` | **3 passed, 0 failed** |
| C++ `test_domain` | same, with `cpp/tests/test_domain.cpp` | **5 passed, 0 failed** |

- **Toolchain:** g++ (GCC) 15.2.0, Python 3.12.13
- **Warnings:** none from either test translation unit under `-Wall -Wextra`

**Caveat worth keeping.** `run_tests.sh` ends with an unconditional
`[SUCCESS] All automated tests passed clean!`, including on the branch where it
finds no `cmake`/`g++`, prints `NOTE: C++ compiler ... not found` and skips the
C++ half. A green run of that script is therefore not by itself evidence that
the C++ tests ran — read the `NOTE:` line, or check that `bin/`/`build/` exists.


## Lab 2.1 — Original Equipment Subclasses

Two original concrete subclasses have been added in both languages:

- `Spectrophotometer`: default wavelength range 190–1100 nm, spectral bandwidth 1 nm, overdue fine Rp120,000/day.
- `RoboticArm`: payload 10 kg, maximum reach 1300 mm, overdue fine Rp180,000/day.

Constructor validation rejects invalid hardware specifications. The hardware values and fines are modelling examples and should be adjusted if your lab assignment uses a specific real-world device/rate. Tests verify the overridden category/fine methods, specifications, invalid inputs, and runtime polymorphism.

Verification in the current work environment: `bash run_tests.sh` completed with **19 Python tests passed** and **3/3 C++ CTest targets passed**. The available interpreter was Python 3.13.5, so this run demonstrates compatibility in that environment; run again using Python 3.12 if your course specifically requires that exact version.


### Current verification for Lab 2.1

Running `./run_tests.sh` in the current environment completed successfully: **19 Python tests passed** and **3 of 3 C++ CTest targets passed**, including the new custom-equipment target. The environment used Python 3.13.5, so run it with Python 3.12 separately if your course requires that exact version.
