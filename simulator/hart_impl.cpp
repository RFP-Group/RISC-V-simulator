#include "hart.h"

#include <iostream>
namespace simulator::core {

void Hart::RunImpl(Mode mode)
{
    size_t counter = 0;
    switch (mode) {
        case Mode::SIMPLE: {
            do {
                uint32_t raw_instr = fetch_.loadInstr(executor_.getPC());
                auto instr = decoder_.DecodeInstr(raw_instr);
                executor_.RunInstr(instr);
                ++counter;
            } while (executor_.getPC() != 0);

            break;
        }
        case Mode::BB: {
            interpreter::BB raw_bb;
            interpreter::DecodedBB decodedBB;
            do {
                fetch_.loadBB(executor_.getPC(), raw_bb);
                decoder_.DecodeBB(raw_bb, decodedBB);
                executor_.RunBB(decodedBB);
                counter += raw_bb.size();
            } while (executor_.getPC() != 0);

            break;
        }
        default:
            std::cerr << "Unsupported mode";
    }

    std::cout << counter << std::endl;
}

}  // namespace simulator::core