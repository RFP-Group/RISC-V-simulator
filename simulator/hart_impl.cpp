#include "hart.h"

#include <iostream>
#include <chrono>

namespace simulator::core {

void Hart::RunImpl(Mode mode, bool need_to_measure)
{
    size_t counter = 0;

    auto start = std::chrono::high_resolution_clock::now();

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
            Register cache_addr;
            do {
                cache_addr = executor_.getPC() / 4 % BB_CACHE_SIZE;
                auto &&[addr, decodedBB] = bb_cache_[cache_addr];
                [[unlikely]] if (addr != executor_.getPC())
                {
                    fetch_.loadBB(executor_.getPC(), raw_bb);
                    decoder_.DecodeBB(raw_bb, decodedBB);
                    addr = executor_.getPC();
                }
                executor_.RunBB(decodedBB);
                counter += decodedBB.size();
            } while (executor_.getPC() != 0);

            break;
        }
        case Mode::NONE: {
            std::cerr << "None mode is used" << std::endl;
            break;
        }
        default:
            std::cerr << "Unsupported mode" << std::endl;
            break;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    if (need_to_measure) {
        std::cout << "Amount of executed instructions: " << counter << std::endl;
        auto duration = duration_cast<std::chrono::microseconds>(stop - start).count();
        std::cout << "Execution time : " << duration / 1e3 << " ms" << std::endl;
        std::cout << "MIPS: " << static_cast<double>(counter) / duration << std::endl;
    }
}

}  // namespace simulator::core