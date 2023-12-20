#ifndef INTERPRETER_BB_H
#define INTERPRETER_BB_H

// #include "interpreter/executor.h"
#include "interpreter/instruction.h"
#include <algorithm>
#include <array>
#include <cstdint>

namespace simulator::interpreter {

class Executor;

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
public:
    constexpr static size_t MAX_HOTNESS = 10;
    using CompiledEntry = void (*)(Executor *, const Instruction *);
    enum class CompileStatus : uint8_t { COMPILED, RAW };

private:
    size_t curSize = 0;
    size_t hotness_counter_ = 0;
    std::array<Instruction, BB::MAX_SIZE + 1> body_;
    CompiledEntry compiled_entry_ = nullptr;
    CompileStatus comp_status_ = CompileStatus::RAW;

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
        body_[curSize] = Instruction {.inst_id = BB_END_INST};
    }
    inline size_t size() const
    {
        return curSize;
    }
    inline void incrementHotness()
    {
        hotness_counter_ = std::min(hotness_counter_ + 1, MAX_HOTNESS);
    }
    inline auto getHotness()
    {
        return hotness_counter_;
    }
    inline auto getCompileStatus()
    {
        return comp_status_;
    }
    inline void setCompileStatus(CompileStatus comp_status)
    {
        comp_status_ = comp_status;
    }
    inline auto getCompiledEntry()
    {
        return compiled_entry_;
    }
    inline void setCompiledEntry(auto compiled_entry)
    {
        compiled_entry_ = compiled_entry;
    }
    inline auto getRawData()
    {
        return body_.data();
    }
    const auto &getBody() const
    {
        return body_;
    }
    auto &getBody()
    {
        return body_;
    }
};

}  // namespace simulator::interpreter

#endif