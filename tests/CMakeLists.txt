# --- Calculator Tests (AI Generated) ---
add_executable(test_calculator test_calculator.cpp)
target_link_libraries(test_calculator PRIVATE cpp_demo_lib GTest::gtest_main)
add_test(NAME CalculatorTest COMMAND test_calculator)

# --- String Utils Tests (AI Generated) ---
add_executable(test_string_utils test_string_utils.cpp)
target_link_libraries(test_string_utils PRIVATE cpp_demo_lib GTest::gtest_main)
add_test(NAME StringUtilsTest COMMAND test_string_utils)

# --- Data Parser Tests (AI Generated) ---
add_executable(test_data_parser test_data_parser.cpp)
target_link_libraries(test_data_parser PRIVATE cpp_demo_lib GTest::gtest_main)
add_test(NAME DataParserTest COMMAND test_data_parser)
