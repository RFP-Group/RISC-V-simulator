find_package(LibElf REQUIRED)

add_library(physical_mem STATIC phys_mem/phys_mem.cpp)
target_include_directories(physical_mem PUBLIC ${PROJECT_ROOT}/memory/includes ${PROJECT_ROOT}/configs)

target_include_directories(physical_mem PUBLIC ${LIBELF_INCLUDE_DIRS})
target_link_libraries(physical_mem PUBLIC ${LIBELF_LIBRARIES})

add_subdirectory("tests")