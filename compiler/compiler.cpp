// #include <asmjit/asmjit.h>
#include <cstddef>
#include "asmjit/core/logger.h"
#include "bitops.h"
#include "compiler/compiler.hpp"
#include "generated/instructions_enum_gen.h"
#include "interpreter/BB.h"
#include "interpreter/gpr.h"
#include "interpreter/instruction.h"
#include "interpreter/executor.h"

namespace simulator::compiler {

void Compiler::run(interpreter::DecodedBB &decodedBB, bool is_cosim)
{
    asmjit::CodeHolder code_holder;
    code_holder.init(runtime_.environment(), runtime_.cpuFeatures());

    asmjit::x86::Compiler compiler(&code_holder);

    static auto entry_signature = asmjit::FuncSignatureT<void, interpreter::Executor *, const Instruction *>();
    auto *entry_node = compiler.addFunc(entry_signature);

    executor_p_ = compiler.newGpq();
    instruction_p_ = compiler.newGpq();
    entry_node->setArg(0, executor_p_);
    entry_node->setArg(1, instruction_p_);

    auto offset_to_gprf = interpreter::Executor::getOffsetToGprf();

    pc_p_ = compiler.newGpq();
    compiler.mov(pc_p_, executor_p_);
    compiler.add(pc_p_, offset_to_gprf + sizeof(Register) * GPR_file::PC);

    registers_p_ = compiler.newGpq();
    compiler.mov(registers_p_, executor_p_);
    compiler.add(registers_p_, offset_to_gprf);

    auto body = decodedBB.getBody();

    for (size_t i = 0; i < body.size(); ++i) {
        if (body[i].inst_id == InstructionId::BB_END_INST)
            break;
        if (is_cosim) {
            compileInvoke(compiler, interpreter::runInstrIface, i);
        } else {
            compileInstr(compiler, &body[i], i);
        }
    }

    compiler.endFunc();
    compiler.finalize();
    interpreter::DecodedBB::CompiledEntry entry = nullptr;
    runtime_.add(&entry, &code_holder);
    decodedBB.setCompiledEntry(entry);
    decodedBB.setCompileStatus(interpreter::DecodedBB::CompileStatus::COMPILED);
}

void Compiler::compileInvoke(asmjit::x86::Compiler &compiler, const interpreter::DecodedBB::CompiledEntry executor,
                             size_t instr_offset)
{
    auto instr = compiler.newGpq();
    compiler.mov(instr, instruction_p_);
    compiler.add(instr, sizeof(Instruction) * instr_offset);

    static auto executor_signature = asmjit::FuncSignatureT<void, interpreter::Executor *, const Instruction *>();
    asmjit::InvokeNode *invokeNode = nullptr;
    compiler.invoke(&invokeNode, executor, executor_signature);
    invokeNode->setArg(0, executor_p_);
    invokeNode->setArg(1, instr);
}

asmjit::x86::Gp Compiler::compileGetReg(asmjit::x86::Compiler &compiler, size_t index)
{
    auto reg = compiler.newGpq();
    compiler.mov(reg, asmjit::x86::qword_ptr(registers_p_, sizeof(Register) * index));
    return reg;
}

asmjit::x86::Gp Compiler::compileGetPC(asmjit::x86::Compiler &compiler)
{
    auto pc = compiler.newGpq();
    compiler.mov(pc, asmjit::x86::qword_ptr(pc_p_));
    return pc;
}

void Compiler::compileSetReg(asmjit::x86::Compiler &compiler, size_t index, asmjit::x86::Gp reg)
{
    compiler.mov(asmjit::x86::qword_ptr(registers_p_, sizeof(Register) * index), reg);
}

void Compiler::compileSetReg(asmjit::x86::Compiler &compiler, size_t index, Immediate_t imm)
{
    compiler.mov(asmjit::x86::qword_ptr(registers_p_, sizeof(Register) * index), imm);
}

void Compiler::compileIncrementPC(asmjit::x86::Compiler &compiler)
{
    compiler.add(asmjit::x86::qword_ptr(pc_p_), 4);
}

void Compiler::compileIncrementPC(asmjit::x86::Compiler &compiler, Immediate_t imm)
{
    compiler.add(asmjit::x86::qword_ptr(pc_p_), imm);
}

void Compiler::compileSetPC(asmjit::x86::Compiler &compiler, Immediate_t imm)
{
    compiler.mov(asmjit::x86::qword_ptr(pc_p_), imm);
}

void Compiler::compileSetPC(asmjit::x86::Compiler &compiler, asmjit::x86::Gp pc)
{
    compiler.mov(asmjit::x86::qword_ptr(pc_p_), pc);
}

void Compiler::compileADDI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.add(op1, GetSignedExtension<Register, 12>(instr->imm));
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileLUI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    compileSetReg(compiler, instr->rd, instr->imm);
    compileIncrementPC(compiler);
}

void Compiler::compileBEQ(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.jne(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileBNE(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.je(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileBLT(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.jge(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileBLTU(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.jae(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileBGE(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.jl(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileBGEU(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto label = compiler.newLabel();
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.jb(label);
    compileIncrementPC(compiler, GetSignedExtension<Register, 12>(instr->imm) - 4);
    compiler.bind(label);
    compileIncrementPC(compiler);
}

void Compiler::compileJALR(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto pc = compileGetPC(compiler);
    compiler.add(pc, 4);
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.add(op1, instr->imm);
    compiler.and_(op1, ~1ULL);
    compileSetReg(compiler, instr->rd, pc);
    compileSetPC(compiler, op1);
}

void Compiler::compileJAL(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto pc = compileGetPC(compiler);
    compileSetReg(compiler, instr->rd, pc);
    auto advanced_pc = compileGetReg(compiler, instr->rd);
    compiler.add(advanced_pc, 4);
    compiler.add(pc, GetSignedExtension<Register, 20>(instr->imm));
    compileSetReg(compiler, instr->rd, advanced_pc);
    compileSetPC(compiler, pc);
}

void Compiler::compileSLLI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto rs = compileGetReg(compiler, instr->rs1);
    compiler.shl(rs, instr->GetShamt());
    compileSetReg(compiler, instr->rd, rs);
    compileIncrementPC(compiler);
}

void Compiler::compileSLL(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.shl(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSLT(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.setl(op1);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSLTU(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.cmp(op1, op2);
    compiler.setb(op1);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileXOR(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.xor_(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSRL(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.shr(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileOR(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.or_(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileAND(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.and_(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSUB(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.sub(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSRA(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.sar(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileADD(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    auto op2 = compileGetReg(compiler, instr->rs2);
    compiler.add(op1, op2);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSLTI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.cmp(op1, instr->imm);
    compiler.setl(op1);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSLTIU(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.cmp(op1, instr->imm);
    compiler.setb(op1);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileXORI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.xor_(op1, instr->imm);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSRLI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.shr(op1, instr->imm);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileSRAI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.sar(op1, instr->imm);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileORI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.or_(op1, instr->imm);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileANDI(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.and_(op1, instr->imm);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileADDIW(asmjit::x86::Compiler &compiler, const Instruction *instr)
{
    auto op1 = compileGetReg(compiler, instr->rs1);
    compiler.add(op1, GetSignedExtension<Register, 12>(instr->imm));
    compiler.movsx(op1, op1);
    compileSetReg(compiler, instr->rd, op1);
    compileIncrementPC(compiler);
}

void Compiler::compileInstr(asmjit::x86::Compiler &compiler, const Instruction *instr, size_t instr_offset)
{
    switch (instr->inst_id) {
        case InstructionId::LUI:
            compileLUI(compiler, instr);
            return;
        case InstructionId::AUIPC:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::JAL:
            compileJAL(compiler, instr);
            return;
        case InstructionId::JALR:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BEQ:
            compileBEQ(compiler, instr);
            return;
        case InstructionId::BNE:
            compileBNE(compiler, instr);
            return;
        case InstructionId::BLT:
            compileBLT(compiler, instr);
            return;
        case InstructionId::BGE:
            compileBGE(compiler, instr);
            return;
        case InstructionId::BLTU:
            compileBLTU(compiler, instr);
            return;
        case InstructionId::BGEU:
            compileBGEU(compiler, instr);
            return;
        case InstructionId::LB:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LH:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LD:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LBU:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LHU:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LWU:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SB:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SH:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SD:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADDI:
            compileADDI(compiler, instr);
            return;
        case InstructionId::SLLI:
            compileSLLI(compiler, instr);
            return;
        case InstructionId::SLTI:
            compileSLTI(compiler, instr);
            return;
        case InstructionId::SLTIU:
            compileSLTIU(compiler, instr);
            return;
        case InstructionId::XORI:
            compileXORI(compiler, instr);
            return;
        case InstructionId::SRLI:
            compileSRLI(compiler, instr);
            return;
        case InstructionId::SRAI:
            compileSRAI(compiler, instr);
            return;
        case InstructionId::ORI:
            compileORI(compiler, instr);
            return;
        case InstructionId::ANDI:
            compileANDI(compiler, instr);
            return;
        case InstructionId::ADDIW:
            compileADDIW(compiler, instr);
            return;
        case InstructionId::SLLIW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRLIW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRAIW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADD:
            compileADD(compiler, instr);
            return;
        case InstructionId::SLL:
            compileSLL(compiler, instr);
            return;
        case InstructionId::SLT:
            compileSLT(compiler, instr);
            return;
        case InstructionId::SLTU:
            compileSLTU(compiler, instr);
            return;
        case InstructionId::XOR:
            compileXOR(compiler, instr);
            return;
        case InstructionId::SRL:
            compileSRL(compiler, instr);
            return;
        case InstructionId::OR:
            compileOR(compiler, instr);
            return;
        case InstructionId::AND:
            compileAND(compiler, instr);
            return;
        case InstructionId::SUB:
            compileSUB(compiler, instr);
            return;
        case InstructionId::SRA:
            compileSRA(compiler, instr);
            return;
        case InstructionId::ADDW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SUBW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLLW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRLW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRAW:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ECALL:
            compileInvoke(compiler, interpreter::runInstrIface, instr_offset);
            return;
        default:
            std::abort();
    }
}

}  // namespace simulator::compiler
