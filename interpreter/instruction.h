#ifndef INTERPRETER_INSTRUCTION_H
#define INTERPRETER_INSTRUCTION_H

#include <cstdint>
#include "generated/instructions_enum_gen.h"
#include "common/bitops.h"

namespace simulator {

using Register_t = uint8_t;
using Immediate_t = uint32_t;
using Opcode_t = uint8_t;

constexpr uint32_t OPCODE_MASK = 0x0000007f;

class Instruction final {
public:
    Immediate_t GetShamt()
    {
        return ApplyMaskAndShift<0x03f00000, 20, 0>(imm);
    }
    Register_t rs1 = 0;
    Register_t rs2 = 0;
    Register_t rs3 = 0;
    Register_t rd = 0;
    Register_t rm = 0;
    Immediate_t imm = 0;

    Opcode_t opcode = 0;
    InstructionId inst_id = WRONG_INST;
};

}  // namespace simulator

#endif  // INTERPRETER_INSTRS_H