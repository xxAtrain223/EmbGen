cmake_minimum_required(VERSION 3.1)
project(EmbGen LANGUAGES CXX)

include(FileOutputs)

file(GLOB_RECURSE _${PROJECT_NAME}_HEADERS "include/*.hpp")
file(GLOB_RECURSE _${PROJECT_NAME}_SOURCES "src/*.cpp")
list(REMOVE_ITEM _${PROJECT_NAME}_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "template.ino" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)" template_ino)
else()
    make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "template.ino" "${CMAKE_CURRENT_BINARY_DIR}" template_ino)
endif()

add_library(_${PROJECT_NAME} ${_${PROJECT_NAME}_SOURCES} ${_${PROJECT_NAME}_HEADERS} ${template_ino})
set_target_properties(_${PROJECT_NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
target_include_directories(_${PROJECT_NAME} PUBLIC include)
target_link_libraries(_${PROJECT_NAME} EmbGenParser nlohmann_json)

if (EMB_ENABLE_TESTING)
    target_compile_options(_${PROJECT_NAME} PRIVATE -g -O0 --coverage -DEMB_TESTING)
    set_target_properties(_${PROJECT_NAME} PROPERTIES LINK_FLAGS "--coverage")

    file(GLOB_RECURSE _${PROJECT_NAME}_TEST_SOURCES "test/*.[ch]pp")
    file(GLOB_RECURSE _${PROJECT_NAME}_TEST_DATA "test/data/*")
    
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "${_${PROJECT_NAME}_TEST_DATA}" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)" testDataOutputs)
    else()
        make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "${_${PROJECT_NAME}_TEST_DATA}" "${CMAKE_CURRENT_BINARY_DIR}" testDataOutputs)
    endif()

    add_executable(_${PROJECT_NAME}Test ${_${PROJECT_NAME}_TEST_SOURCES} ${testDataOutputs})
    set_target_properties(_${PROJECT_NAME}Test PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
    target_link_libraries(_${PROJECT_NAME}Test _${PROJECT_NAME} gtest gmock gtest_main)
    target_compile_options(_${PROJECT_NAME}Test PRIVATE -g -O0 --coverage -DEMB_TESTING)
    set_target_properties(_${PROJECT_NAME}Test PROPERTIES LINK_FLAGS "--coverage")

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set_target_properties(_${PROJECT_NAME}Test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
    endif()
else()
    add_executable(${PROJECT_NAME} "src/main.cpp")
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
    target_link_libraries(${PROJECT_NAME} _${PROJECT_NAME} args)
    
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_link_libraries(${PROJECT_NAME} stdc++fs)
    endif()
endif()

