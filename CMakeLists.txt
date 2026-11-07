cmake_minimum_required(VERSION 3.20)
project(PinjamLab_Module1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Enable sanitizers for Debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT WIN32)
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

include_directories(cpp/include cpp/tests)

# Test targets
add_executable(test_asset_tag cpp/tests/test_asset_tag.cpp)
add_executable(test_domain cpp/tests/test_domain.cpp)
add_executable(test_custom_equipment cpp/tests/test_custom_equipment.cpp cpp/src/spectrophotometer.cpp cpp/src/robotic_arm.cpp)

enable_testing()
add_test(NAME TestAssetTag COMMAND test_asset_tag)
add_test(NAME TestDomain COMMAND test_domain)
add_test(NAME TestCustomEquipment COMMAND test_custom_equipment)
