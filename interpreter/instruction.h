#ifndef INTERPRETER_INSTRUCTION_H
#define INTERPRETER_INSTRUCTION_H

#include <cstdint>

namespace simulator {

using Register_t = uint8_t;
using Immediate_t = uint32_t;
using Opcode_t = uint8_t;

class Instruction {
public:
    Register_t rs1;
    Register_t rs2;
    Register_t rs3;
    Register_t rd;
    Register_t rm;
    Immediate_t imm;

    Opcode_t opcode;
};

}  // namespace simulator

#endif  // INTERPRETER_INSTRS_H