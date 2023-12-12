set(CORE_SOURCES
    hart_impl.cpp
)

add_library(core STATIC ${CORE_SOURCES})
target_link_libraries(core PUBLIC mem interpreter)

set(SOURCES
    simulator.cpp
)

add_executable(rvsim ${SOURCES})
target_include_directories(rvsim PUBLIC ${PROJECT_ROOT}/memory/includes ${PROJECT_ROOT}/configs)
target_link_libraries(rvsim PRIVATE core mem interpreter CLI11)

add_custom_target(
    run_rvsim
    COMMENT "Running RISK-V simulator"
    COMMAND ./rvsim
)
add_dependencies(run_rvsim rvsim)