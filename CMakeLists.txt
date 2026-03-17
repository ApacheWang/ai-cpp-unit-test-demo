cmake_minimum_required(VERSION 3.14)
project(AICppUnitTestDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Google Test ---
# 优先使用系统安装的 gtest，如果没有则 FetchContent 下载
find_package(GTest QUIET)
if(GTest_FOUND)
    message(STATUS "Using system Google Test")
else()
    message(STATUS "System Google Test not found, fetching from source...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

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
