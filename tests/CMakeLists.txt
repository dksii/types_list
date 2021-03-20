add_executable(tests tests.cpp)
set_target_properties(tests PROPERTIES CXX_EXTENSIONS OFF)

if (MSVC)
	target_compile_options(tests PRIVATE /W4)
else()
	target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(tests types_list)
set_target_properties(tests PROPERTIES FOLDER "Tests")
add_test(NAME TestList COMMAND tests)