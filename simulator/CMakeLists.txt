set(SOURCES
    simulator.cpp
)

add_executable(rvsim ${SOURCES})
target_include_directories(rvsim PUBLIC ${PROJECT_ROOT}/memory/includes ${PROJECT_ROOT}/configs)

add_custom_target(
    run_rvsim
    COMMENT "Running RISK-V simulator"
    COMMAND ./rvsim
)
add_dependencies(run_rvsim rvsim)