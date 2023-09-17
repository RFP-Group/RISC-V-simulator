enable_testing()

add_executable(phys_mem_tests phys_mem_tests.cpp )
target_link_libraries(phys_mem_tests physical_mem GTest::gtest_main)
target_include_directories(phys_mem_tests PUBLIC ${PROJECT_SOURCE_DIR}/memory/includes ${PROJECT_SOURCE_DIR}/configs ${PROJECT_SOURCE_DIR}/third-party/googletest)
target_compile_options(phys_mem_tests PUBLIC -fsanitize=address -g)
set_target_properties(phys_mem_tests PROPERTIES LINK_FLAGS "-fsanitize=address")

add_custom_target(
    mem_gtests
    COMMENT "Running memory tests"
    COMMAND ./phys_mem_tests
)
add_dependencies(mem_gtests phys_mem_tests)