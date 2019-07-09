cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ext)

add_library(tinyxml2 tinyxml2/tinyxml2.cpp)
target_include_directories(tinyxml2 PUBLIC tinyxml2)

if (EMB_ENABLE_TESTING)
    set(gtest_force_shared_crt TRUE CACHE BOOL "")
    add_subdirectory(googletest)
endif()

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE json/include)
