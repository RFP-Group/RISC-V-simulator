file(MAKE_DIRECTORY ${PROJECT_BINARY_ROOT}/interpreter/generated)

set(INTERPRETER_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/generated/instructions_decode_gen.cpp
)

add_library(interpreter STATIC ${INTERPRETER_SOURCES})

add_dependencies(interpreter
	interpreter_decode_cpp_gen
	interpreter_decode_h_gen
)

set(INSTRUCTION_DECODE_GEN_CPP ${CMAKE_CURRENT_BINARY_DIR}/generated/instructions_decode_gen.cpp)
gen_file(
	GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/gen_inst_decode.rb
	TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/instructions_decode_gen.cpp.erb
	OUTPUT ${INSTRUCTION_DECODE_GEN_CPP}
)
add_custom_target(interpreter_decode_cpp_gen DEPENDS ${INSTRUCTION_DECODE_GEN_CPP})

set(INSTRUCTION_DECODE_GEN_H ${CMAKE_CURRENT_BINARY_DIR}/generated/instructions_decode_gen.h)
gen_file(
	GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/gen_inst_decode.rb
	TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/instructions_decode_gen.h.erb
	OUTPUT ${INSTRUCTION_DECODE_GEN_H}
)
add_custom_target(interpreter_decode_h_gen DEPENDS ${INSTRUCTION_DECODE_GEN_H})

target_include_directories(interpreter PRIVATE
	${PROJECT_ROOT}
	${CMAKE_CURRENT_BINARY_DIR}
)