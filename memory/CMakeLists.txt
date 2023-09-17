add_library(physical_mem STATIC phys_mem/phys_mem.cpp)
target_include_directories(physical_mem PUBLIC ${PROJECT_ROOT}/memory/includes ${PROJECT_ROOT}/configs)

add_subdirectory("tests")