#ifndef INTERPRETER_DECODER_H
#define INTERPRETER_DECODER_H

#include "interpreter/instruction.h"
#include "interpreter/BB.h"

namespace simulator::interpreter {

class Decoder final {
public:
    [[nodiscard]] Instruction DecodeInstr(uint32_t raw_inst);
    inline void DecodeBB(const BB &raw_bb, DecodedBB &decoded_bb)
    {
        decoded_bb.clear();
        for (auto curIt = raw_bb.cbegin(), endIt = raw_bb.cend(); curIt != endIt; ++curIt)
            decoded_bb.add_instr(DecodeInstr(*curIt));
        decoded_bb.addTerminator();
    }

private:
#include "generated/instructions_decode_gen.h"
};

}  // namespace simulator::interpreter

#endif  // INTERPRETER_DECODER_H