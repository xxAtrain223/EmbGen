cmake_minimum_required(VERSION 3.1)
project(EmbGen LANGUAGES CXX)

include(FileOutputs)

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "include/*.hpp")
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} EmbGenParser nlohmann_json)

if (MSVC)
    make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "template.ino" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)" template_ino)
else()
    make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "template.ino" "${CMAKE_CURRENT_BINARY_DIR}" template_ino)
endif()

if (EMB_ENABLE_TESTING)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 --coverage -DEMB_TESTING)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--coverage")

    file(GLOB_RECURSE ${PROJECT_NAME}_TEST_SOURCES "test/*.[ch]pp")
    file(GLOB_RECURSE ${PROJECT_NAME}_TEST_DATA "test/data/*")
    
    if (MSVC)
        make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "${${PROJECT_NAME}_TEST_DATA}" "${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)" testDataOutputs)
    else()
        make_outputs(${CMAKE_CURRENT_SOURCE_DIR} "${${PROJECT_NAME}_TEST_DATA}" "${CMAKE_CURRENT_BINARY_DIR}" testDataOutputs)
    endif()

    add_executable(${PROJECT_NAME}Test ${${PROJECT_NAME}_TEST_SOURCES} ${testDataOutputs})
    set_target_properties(${PROJECT_NAME}Test PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES)
    target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME} gtest gmock gtest_main)
    target_compile_options(${PROJECT_NAME}Test PRIVATE -g -O0 --coverage -DEMB_TESTING)
    set_target_properties(${PROJECT_NAME}Test PROPERTIES LINK_FLAGS "--coverage")

	if (MSVC)
		set_target_properties(${PROJECT_NAME}Test PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
	endif()
endif()
