#ifndef INTERPRETER_EXECUTOR_H
#define INTERPRETER_EXECUTOR_H

#include "interpreter/instruction.h"
#include "interpreter/gpr.h"
#include "interpreter/csr.h"
#include "memory/includes/mmu.hpp"
#include "interpreter/BB.h"
#include <iostream>

namespace simulator::interpreter {

class Executor {
public:
    Executor(mem::MMU *mmu, uintptr_t entry_point, bool is_cosim) : mmu_(mmu), is_cosim_(is_cosim)
    {
        gprf_.write(GPR_file::GPR_n::PC, entry_point);
    };
    NO_COPY_SEMANTIC(Executor)
    NO_MOVE_SEMANTIC(Executor)

    void RunInstr(Instruction inst);
    void RunBB(const DecodedBB &bb);

    void GetTrace(const simulator::Instruction *cur_instr);

    [[nodiscard]] inline Register getPC()
    {
        return gprf_.read(GPR_file::GPR_n::PC);
    };

    inline const GPR_file &getGPRfile()
    {
        return gprf_;
    }

    inline const CSR_file &getCSRfile()
    {
        return csrf_;
    }

private:
#include "generated/executor_gen.h"
    GPR_file gprf_;
    CSR_file csrf_;
    mem::MMU *mmu_;
    bool is_cosim_ = false;
};

}  // namespace simulator::interpreter

#include "interpreter/executor_impl.h"

#endif  // INTERPRETER_EXECUTOR_H