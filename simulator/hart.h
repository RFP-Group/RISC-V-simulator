#ifndef INTERPRETER_HART_H
#define INTERPRETER_HART_H

#include "compiler/compiler.hpp"
#include "macros.hpp"
#include "mmu.hpp"
#include "interpreter/fetch.h"
#include "interpreter/decoder.h"
#include "interpreter/executor.h"
#include "interpreter/BB.h"
#include <array>
#include <utility>

namespace simulator::core {

class Hart final {
public:
    enum class Mode { NONE, SIMPLE, BB };

    void RunImpl(Mode mode, bool need_to_measure);

    Hart(mem::MMU *mmu, uintptr_t entry_point, bool is_cosim)
        : mmu_(mmu), fetch_(mmu), executor_(mmu_, entry_point, is_cosim), is_cosim_(is_cosim) {};
    NO_COPY_SEMANTIC(Hart)
    NO_MOVE_SEMANTIC(Hart)

private:
    mem::MMU *mmu_;
    compiler::Compiler compiler_;
    interpreter::Fetch fetch_;
    interpreter::Decoder decoder_;
    interpreter::Executor executor_;
    static constexpr size_t BB_CACHE_SIZE = 256;
    std::array<std::pair<Register, interpreter::DecodedBB>, BB_CACHE_SIZE> bb_cache_;
    bool is_cosim_;
};

}  // namespace simulator::core

#endif