#ifndef INTERPRETER_EXECUTOR_H
#define INTERPRETER_EXECUTOR_H

#include "interpreter/instruction.h"
#include "interpreter/gpr.h"
#include "interpreter/csr.h"
#include "memory/includes/virtual_mem.hpp"

namespace simulator::interpreter {

class Executor {
public:
    Executor(mem::VirtualMem *vmem, uintptr_t entry_point) : vmem_(vmem)
    {
        gprf_.write(GPR_file::GPR_n::PC, entry_point);
    };
    NO_COPY_SEMANTIC(Executor);
    NO_MOVE_SEMANTIC(Executor);

    void RunImpl(Instruction inst);
    [[nodiscard]] inline Register getPC()
    {
        return gprf_.read(GPR_file::GPR_n::PC);
    };

    const GPR_file &getGPRfile()
    {
        return gprf_;
    }

    const CSR_file &getCSRfile()
    {
        return csrf_;
    }

private:
#include "generated/executor_gen.h"
    GPR_file gprf_;
    CSR_file csrf_;
    mem::VirtualMem *vmem_;
};

}  // namespace simulator::interpreter

#endif  // INTERPRETER_EXECUTOR_H