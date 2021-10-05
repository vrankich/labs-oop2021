cmake_minimum_required(VERSION 2.6)
project(google_test)

################################
# GTest
################################
# ADD_SUBDIRECTORY (~/usr/include/gtest/)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
include_directories(../)
# Add test cpp file
add_executable(test_table test_table.cpp ../table.cpp)
# Link test executable against gtest & gtest_main
target_link_libraries(test_table gtest gtest_main)
add_test(test_table test_table)
