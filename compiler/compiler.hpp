#ifndef COMPILER_COMPILER_HPP
#define COMPILER_COMPILER_HPP

#include <asmjit/asmjit.h>
#include "interpreter/BB.h"
#include "interpreter/instruction.h"

namespace simulator::compiler {

class Compiler {
public:
    void run(interpreter::DecodedBB &decodedBB, bool is_cosim);

    asmjit::x86::Gp compileGetReg(asmjit::x86::Compiler &compiler, size_t index);
    void compileSetReg(asmjit::x86::Compiler &compiler, size_t index, asmjit::x86::Gp reg);
    void compileSetReg(asmjit::x86::Compiler &compiler, size_t index, Immediate_t imm);
    asmjit::x86::Gp compileGetPC(asmjit::x86::Compiler &compiler);
    void compileIncrementPC(asmjit::x86::Compiler &compiler);
    void compileIncrementPC(asmjit::x86::Compiler &compiler, Immediate_t imm);
    void compileSetPC(asmjit::x86::Compiler &compiler, Immediate_t imm);
    void compileSetPC(asmjit::x86::Compiler &compiler, asmjit::x86::Gp pc);

    void compileADDI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileLUI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBEQ(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBNE(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBLT(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBLTU(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBGE(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileBGEU(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileJALR(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileJAL(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLLI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLL(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLT(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLTU(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileXOR(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSRL(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileOR(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileAND(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSUB(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSRA(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileADD(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLTI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSLTIU(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileXORI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSRLI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileSRAI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileORI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileANDI(asmjit::x86::Compiler &compiler, const Instruction *instr);
    void compileADDIW(asmjit::x86::Compiler &compiler, const Instruction *instr);

    void compileInstr(asmjit::x86::Compiler &compiler_, const Instruction *instr, size_t instr_offset);
    void compileInvoke(asmjit::x86::Compiler &compiler_, interpreter::DecodedBB::CompiledEntry executor,
                       size_t instr_offset);

private:
    asmjit::JitRuntime runtime_;
    asmjit::x86::Gp executor_p_;
    asmjit::x86::Gp pc_p_;
    asmjit::x86::Gp registers_p_;
    asmjit::x86::Gp instruction_p_;
};

}  // namespace simulator::compiler

#endif  // COMPILER_COMPILER_HPP
