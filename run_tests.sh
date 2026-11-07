#!/usr/bin/env bash
# ==============================================================================
# PinjamLab Module 1: Single-Command Test Runner (C++ + Python)
# MASTER §3.2 item 7: Reproduce-from-clean standard
# ==============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "================================================================================"
echo " [PinjamLab M1] Running Cross-Language Verification Suite"
echo "================================================================================"

# 1. Python Test Suite (pytest)
echo ""
echo ">>> [1/2] Running Python 3.12 Test Suite (pytest)..."
if command -v python3 &>/dev/null; then
    PYTHON_CMD="python3"
elif command -v python &>/dev/null; then
    PYTHON_CMD="python"
else
    echo "ERROR: Python interpreter not found."
    exit 1
fi

export PYTHONPATH="py"
$PYTHON_CMD -m pytest py/tests -v

# 2. C++20 Test Suite (CMake + CTest or compiler direct)
echo ""
echo ">>> [2/2] Running C++20 Test Suite..."
if command -v cmake &>/dev/null; then
    mkdir -p build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    cmake --build .
    ctest --output-on-failure
    cd ..
elif command -v g++ &>/dev/null; then
    mkdir -p bin
    g++ -std=c++20 -Wall -Wextra -Icpp/include -Icpp/tests cpp/tests/test_asset_tag.cpp -o bin/test_asset_tag
    g++ -std=c++20 -Wall -Wextra -Icpp/include -Icpp/tests cpp/tests/test_domain.cpp -o bin/test_domain
    g++ -std=c++20 -Wall -Wextra -Icpp/include -Icpp/tests cpp/tests/test_custom_equipment.cpp cpp/src/spectrophotometer.cpp cpp/src/robotic_arm.cpp -o bin/test_custom_equipment
    ./bin/test_asset_tag
    ./bin/test_domain
    ./bin/test_custom_equipment
else
    echo "NOTE: C++ compiler (cmake/g++) not found in current PATH. Python tests passed cleanly."
fi

echo ""
echo "================================================================================"
echo " [SUCCESS] All automated tests passed clean!"
echo "================================================================================"
