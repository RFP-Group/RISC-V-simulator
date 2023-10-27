set(TEST_SOURCES
    main.cpp
    executor_tests.cpp
    decoder_tests.cpp
)

add_executable(interpreter_tests ${TEST_SOURCES})
target_link_libraries(interpreter_tests interpreter mem GTest::gtest_main)
target_include_directories(interpreter_tests PUBLIC
    ${PROJECT_SOURCE_DIR}/configs
    ${PROJECT_SOURCE_DIR}/third-party/googletest
)

target_compile_options(interpreter_tests PUBLIC -fsanitize=address)
set_target_properties(interpreter_tests PROPERTIES LINK_FLAGS "-fsanitize=address")

add_custom_target(
    run_interpreter_tests
    COMMENT "Running interpreter tests"
    COMMAND ./interpreter_tests
)
add_dependencies(run_interpreter_tests interpreter_tests)
add_dependencies(run_all_tests run_interpreter_tests)
