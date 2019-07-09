cmake_minimum_required(VERSION 3.0.0)
project(EmbGen)

option(EMB_ENABLE_TESTING "Enable Testing for EmbGen" ON)

add_subdirectory(ext)
add_subdirectory(parser)
add_subdirectory(gen)

if (EMB_ENABLE_TESTING)
    enable_testing()
    cmake_policy(SET CMP0057 NEW)
    include(GoogleTest)
    gtest_add_tests(EmbGenParserTest "" AUTO)
endif()
