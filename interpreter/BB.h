#ifndef INTERPRETER_BB_H
#define INTERPRETER_BB_H

#include "interpreter/instruction.h"
#include <algorithm>
#include <array>

namespace simulator::interpreter {

class BB final {
public:
    static constexpr size_t MAX_SIZE = 9;

private:
    size_t curSize = 0;
    std::array<uint32_t, MAX_SIZE> raw_instrs_;

public:
    bool add_instr(uint32_t raw_instr)
    {
        [[unlikely]] if (curSize >= MAX_SIZE) return false;
        raw_instrs_[curSize++] = raw_instr;
        return true;
    }

    [[nodiscard]] inline auto cbegin() const
    {
        return raw_instrs_.cbegin();
    }
    [[nodiscard]] inline auto cend() const
    {
        return raw_instrs_.cbegin() + curSize;
    }
    inline void clear()
    {
        curSize = 0;
    }
    inline size_t size() const
    {
        return curSize;
    }
};

class DecodedBB final {
    size_t curSize = 0;
    std::array<Instruction, BB::MAX_SIZE + 1> body_;

public:
    [[nodiscard]] inline auto getBeginBB() const
    {
        return body_.cbegin();
    }
    inline bool add_instr(const Instruction &decodedInstr)
    {
        [[unlikely]] if (curSize >= BB::MAX_SIZE + 1) return false;  // maybe more verbose output
        body_[curSize++] = decodedInstr;
        return true;
    }
    inline void clear()
    {
        curSize = 0;
    }
    inline void addTerminator()
    {
        body_[curSize++] = Instruction {.inst_id = BB_END_INST};
    }
};

}  // namespace simulator::interpreter

#endif