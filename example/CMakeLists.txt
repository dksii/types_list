set(SRC_LIST

	inheritance.cpp
	runtime.cpp
)

foreach(srcfile ${SRC_LIST})
	get_filename_component(target "${srcfile}" NAME_WE)
	add_executable(${target} ${srcfile})

	set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

	if (MSVC)
		target_compile_options(${target} PRIVATE /W4)
	else()
		target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
	endif()

	target_link_libraries(${target} types_list)
	set_target_properties(${target} PROPERTIES FOLDER "Examples")
endforeach()