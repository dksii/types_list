cmake_minimum_required(VERSION 3.20)

project(types_list LANGUAGES CXX)

# library
add_library(types_list INTERFACE include/types_list/list.hpp)

target_include_directories(types_list INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(types_list INTERFACE cxx_std_17)

# build tests and examples only when project used as main (not included in other)
string(COMPARE EQUAL "${CMAKE_PROJECT_NAME}" "${PROJECT_NAME}" IS_MAIN_PROJ)

if (IS_MAIN_PROJ)
	enable_testing()
	# support folders in IDE's
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# examples
if (IS_MAIN_PROJ AND BUILD_EXAMPLES)
	add_subdirectory(example)
endif()

#tests
if (IS_MAIN_PROJ AND BUILD_TESTING)
	add_subdirectory(tests)
endif()
