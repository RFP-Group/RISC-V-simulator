#ifndef INTERPRETER_DECODER_H
#define INTERPRETER_DECODER_H

#include "interpreter/instruction.h"

namespace simulator {

constexpr uint32_t OPCODE_MASK = 0x0000007f;

class Decoder {
public:
    Instruction runImpl(uint32_t raw_inst);

private:
#include "generated/instructions_decode_gen.h"
};

}  // namespace simulator

#endif  // INTERPRETER_DECODER_H