set(TEST_SOURCES
    mem_tests.cpp
)

add_executable(mem_tests ${TEST_SOURCES})
target_link_libraries(mem_tests mem GTest::gtest_main)
target_include_directories(mem_tests PUBLIC ${PROJECT_SOURCE_DIR}/memory/includes ${PROJECT_SOURCE_DIR}/configs ${PROJECT_SOURCE_DIR}/third-party/googletest)
target_compile_options(mem_tests PUBLIC -fsanitize=address)
set_target_properties(mem_tests PROPERTIES LINK_FLAGS "-fsanitize=address")

add_custom_target(
    mem_gtests
    COMMENT "Running memory tests"
    COMMAND ./mem_tests
)
add_dependencies(mem_gtests mem_tests)