file(MAKE_DIRECTORY ${PROJECT_BINARY_ROOT}/interpreter/generated)

set(INTERPRETER_SOURCES
	${CMAKE_CURRENT_BINARY_DIR}/generated/instructions_decode_gen.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/executor_gen.cpp
	executor_impl.cpp
)

add_library(interpreter STATIC ${INTERPRETER_SOURCES})

add_dependencies(interpreter
	interpreter_decode_cpp_gen
	interpreter_decode_h_gen
	interpreter_enum_h_gen
	interpreter_executor_cpp_gen
	interpreter_executor_h_gen
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

set(INSTRUCTION_ENUM_GEN_H ${CMAKE_CURRENT_BINARY_DIR}/generated/instructions_enum_gen.h)
gen_file(
	GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/gen_inst_decode.rb
	TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/instructions_enum_gen.h.erb
	OUTPUT ${INSTRUCTION_ENUM_GEN_H}
)
add_custom_target(interpreter_enum_h_gen DEPENDS ${INSTRUCTION_ENUM_GEN_H})

set(EXECUTOR_CPP ${CMAKE_CURRENT_BINARY_DIR}/generated/executor_gen.cpp)
gen_file(
	GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/gen_inst_decode.rb
	TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/executor_gen.cpp.erb
	OUTPUT ${EXECUTOR_CPP}
)
add_custom_target(interpreter_executor_cpp_gen DEPENDS ${EXECUTOR_CPP})

set(EXECUTOR_H ${CMAKE_CURRENT_BINARY_DIR}/generated/executor_gen.h)
gen_file(
	GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/gen_inst_decode.rb
	TEMPLATE ${CMAKE_CURRENT_SOURCE_DIR}/templates/executor_gen.h.erb
	OUTPUT ${EXECUTOR_H}
)
add_custom_target(interpreter_executor_h_gen DEPENDS ${EXECUTOR_H})

target_link_libraries(interpreter mem)

target_include_directories(interpreter PUBLIC
	${PROJECT_ROOT}
	${CMAKE_CURRENT_BINARY_DIR}
)