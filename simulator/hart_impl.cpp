#include "hart.h"

#include <iostream>
namespace simulator::core {

void Hart::RunImpl()
{
    Register last_PC;
    size_t counter = 0;
    do {
        last_PC = executor_.getPC();
        uint32_t raw_instr = fetch_.RunImpl(executor_.getPC());
        auto instr = decoder_.RunImpl(raw_instr);
        executor_.RunImpl(instr);
        counter++;
    } while (last_PC != executor_.getPC());
    std::cout << counter << std::endl;
}

}  // namespace simulator::core