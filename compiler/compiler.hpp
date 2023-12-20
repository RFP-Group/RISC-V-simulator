#ifndef COMPILER_COMPILER_HPP
#define COMPILER_COMPILER_HPP

#include <asmjit/asmjit.h>
#include "interpreter/BB.h"

namespace simulator::compiler {

class Compiler {
public:
    static const size_t MAX_HOTNESS = 10;

    void run(interpreter::DecodedBB &decodedBB);

    void compileInstr(const Instruction &instr, size_t instr_offset);
    void compileInvoke(interpreter::DecodedBB::CompiledEntry executor, size_t instr_offset);

private:
    interpreter::Executor *executor_;
    asmjit::JitRuntime runtime_;
    asmjit::x86::Compiler compiler_;
    asmjit::x86::Gp executor_p_;
    asmjit::x86::Gp pc_p_;
    asmjit::x86::Gp registers_p_;
    asmjit::x86::Gp instruction_p_;
};

}  // namespace simulator::compiler

#endif  // COMPILER_COMPILER_HPP
