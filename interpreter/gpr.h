#ifndef INTERPRETER_GPR_H
#define INTERPRETER_GPR_H

#include "interpreter/instruction.h"
#include <array>
#include <cstdint>

namespace simulator {

using Register = uint64_t;
using SRegister = int64_t;

const uint8_t Register_num = 34;

class GPR_file final {
public:
    enum GPR_n : uint8_t {
        X0 = 0,
        X1 = 1,
        X2 = 2,
        X3 = 3,
        X4 = 4,
        X5 = 5,
        X6 = 6,
        X7 = 7,
        X8 = 8,
        X9 = 9,
        X10 = 10,
        X11 = 11,
        X12 = 12,
        X13 = 13,
        X14 = 14,
        X15 = 15,
        X16 = 16,
        X17 = 17,
        X18 = 18,
        X19 = 19,
        X20 = 20,
        X21 = 21,
        X22 = 22,
        X23 = 23,
        X24 = 24,
        X25 = 25,
        X26 = 26,
        X27 = 27,
        X28 = 28,
        X29 = 29,
        X30 = 30,
        X31 = 31,
        X32 = 32,
        PC = 33
    };
    Register read(uint8_t reg_n) const
    {
        return gpr_[reg_n];
    }
    void write(uint8_t reg_n, Register value)
    {
        gpr_[reg_n] = value;
        gpr_[GPR_n::X0] = 0;
    }

private:
    std::array<Register, Register_num> gpr_ {};
};

}  // namespace simulator

#endif  // INTERPRETER_GPR_H