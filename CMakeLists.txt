cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(rvsim)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})
set(PROJECT_BINARY_ROOT ${PROJECT_BINARY_DIR})
set(PROJECT_THIRD_PARTY ${PROJECT_ROOT}/third-party)
set(PROJECT_SCRIPTS ${PROJECT_ROOT}/scripts)

find_package(Python3 REQUIRED)

execute_process(
		COMMAND ${Python3_EXECUTABLE} ${PROJECT_SCRIPTS}/install_third_party.py ${PROJECT_SCRIPTS}/third_party_list.txt ${PROJECT_THIRD_PARTY}
		RESULT_VARIABLE INSTALL_RES
	)

if (NOT INSTALL_RES EQUAL 0)
	message(FATAL_ERROR "Unable to install required third-party dependencies")
endif()