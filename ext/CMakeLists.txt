cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ext)

add_subdirectory(EmbGenParser)

if (NOT TARGET gtest AND EmbGen_ENABLE_TESTING)
    set(gtest_force_shared_crt TRUE CACHE BOOL "")
    add_subdirectory(googletest)
endif()

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE json/include)

add_library(args INTERFACE)
target_include_directories(args INTERFACE args)