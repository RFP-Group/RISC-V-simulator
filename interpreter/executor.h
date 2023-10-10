#ifndef INTERPRETER_EXECUTOR_H
#define INTERPRETER_EXECUTOR_H

#include "interpreter/instruction.h"
#include "interpreter/gpr.h"
#include "memory/includes/virtual_mem.hpp"

namespace simulator {

class Executor {
public:
    void RunImpl(Instruction inst);

private:
#include "generated/executor_gen.h"
    GPR_file gprf_;
    mem::VirtualMem *vmem_;
};

}  // namespace simulator

#endif  // INTERPRETER_EXECUTOR_H