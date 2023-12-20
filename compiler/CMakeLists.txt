set(ASMJIT_EMBED TRUE)

add_definitions(-DASMJIT_STATIC)

set(COMPILER_SRC
    compiler.cpp
)

add_library(asmjit_compiler STATIC ${ASMJIT_SRC})
target_link_libraries(asmjit_compiler PUBLIC ${ASMJIT_DEPS})
target_include_directories(asmjit_compiler PUBLIC ${ASMJIT_DIR}/src)

add_library(compiler ${COMPILER_SRC})
target_link_libraries(compiler PUBLIC asmjit ${ASMJIT_DEPS})

target_include_directories(compiler
    PUBLIC ${PROJECT_BINARY_DIR}
    PUBLIC ${PROJECT_SOURCE_DIR}
)

target_link_libraries(compiler 
    PUBLIC asmjit_compiler interpreter
)