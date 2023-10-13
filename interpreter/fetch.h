#ifndef INTERPRETER_FETCH_H
#define INTERPRETER_FETCH_H

#include "configs/macros.hpp"
#include "memory/includes/virtual_mem.hpp"
#include "gpr.h"

namespace simulator::interpreter {

class Fetch final {
public:
    Fetch(mem::VirtualMem *vmem) : vmem_(vmem) {};
    NO_COPY_SEMANTIC(Fetch)
    NO_MOVE_SEMANTIC(Fetch)

    [[nodiscard]] inline uint32_t RunImpl(Register PC_)
    {
        return vmem_->LoadFourBytesFast(uintptr_t(PC_));
    };

private:
    mem::VirtualMem *vmem_;
};

}  // namespace simulator::interpreter

#endif