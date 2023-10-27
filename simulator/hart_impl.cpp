#include "hart.h"

namespace simulator::core {

void Hart::RunImpl()
{
    Register last_PC;
    do {
        last_PC = executor_.getPC();
        uint32_t raw_instr = fetch_.RunImpl(executor_.getPC());
        auto instr = decoder_.RunImpl(raw_instr);
        executor_.RunImpl(instr);
    } while (last_PC != executor_.getPC());
}

}  // namespace simulator::core