#ifndef INTERPRETER_INSTRUCTION_H
#define INTERPRETER_INSTRUCTION_H

#include <cstdint>
#include "generated/instructions_enum_gen.h"
#include "common/bitops.h"

namespace simulator {

using Register_t = uint8_t;
using Immediate_t = uint32_t;
using Opcode_t = uint8_t;

class Instruction {
public:
    Immediate_t GetShamt()
    {
        return ApplyMaskAndShift<0x03f00000, 20, 0>(imm);
    }
    Register_t rs1;
    Register_t rs2;
    Register_t rs3;
    Register_t rd;
    Register_t rm;
    Immediate_t imm;

    Opcode_t opcode;
    InstructionId inst_id;
};

}  // namespace simulator

#endif  // INTERPRETER_INSTRS_H