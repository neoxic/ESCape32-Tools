cmake_minimum_required(VERSION 3.15)
project(ESCape32-Tools)
add_compile_options(-Wall -Wextra -Wpedantic -Wundef -Wshadow -Wredundant-decls -Wstrict-prototypes -Wmissing-prototypes
	-Wno-variadic-macros -Wno-unused-result -Wno-unused-parameter)

function(add_target name)
	file(GLOB srcs ${name}/*.c)
	add_executable(${name} ${srcs})
	include(GNUInstallDirs)
	install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_BINDIR})
endfunction()

add_target(escape32-cli)
add_target(escape32-update)
