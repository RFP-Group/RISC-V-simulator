cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(rvsim)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(PROJECT_BINARY_ROOT ${PROJECT_BINARY_DIR})
set(PROJECT_THIRD_PARTY ${PROJECT_ROOT}/third-party)
set(PROJECT_SCRIPTS ${PROJECT_ROOT}/scripts)

find_package(Python3 REQUIRED)

include(cmake/Generation.cmake)

execute_process(
	COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/install_third_party.py
		${PROJECT_SCRIPTS}/third_party_list.txt ${PROJECT_THIRD_PARTY}
		RESULT_VARIABLE INSTALL_RES
)

add_custom_target(
	run-clang-format
	COMMENT Run clang-format
	COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/run_clang_format.py ${PROJECT_ROOT}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(generate_all)

add_custom_target(
	run-clang-tidy
	COMMENT Run clang-tidy
	COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/run_clang_tidy.py ${PROJECT_ROOT}
	DEPENDS generate_all
)

if (NOT INSTALL_RES EQUAL 0)
	message(FATAL_ERROR "Unable to install required third-party dependencies")
endif()

set(ASMJIT_DIR "${PROJECT_THIRD_PARTY}/asmjit")
include("${ASMJIT_DIR}/CMakeLists.txt")

add_subdirectory(compiler)
add_subdirectory(simulator)

add_subdirectory(third-party/googletest)
add_subdirectory(third-party/CLI11)

add_subdirectory(memory)
add_subdirectory(interpreter)
add_subdirectory(tests)