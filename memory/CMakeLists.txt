find_package(LibElf REQUIRED)

set(SOURCES
    phys_mem.cpp
    virtual_mem.cpp
    page.cpp
)

add_library(mem STATIC ${SOURCES})
target_include_directories(mem PUBLIC ${PROJECT_ROOT}/memory/includes ${PROJECT_ROOT}/configs)

target_include_directories(mem PUBLIC ${LIBELF_INCLUDE_DIRS})
target_link_libraries(mem PUBLIC ${LIBELF_LIBRARIES})
