cmake_minimum_required(VERSION 3.14)
project(AICppUnitTestDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Google Test ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Library under test ---
add_library(cpp_demo_lib
    src/calculator.cpp
    src/string_utils.cpp
    src/data_parser.cpp
)
target_include_directories(cpp_demo_lib PUBLIC include)

# --- Tests ---
enable_testing()
add_subdirectory(tests)
