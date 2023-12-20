// #include <asmjit/asmjit.h>
#include <cstddef>
#include "compiler/compiler.hpp"
#include "generated/instructions_enum_gen.h"
#include "interpreter/BB.h"
#include "interpreter/gpr.h"
#include "interpreter/instruction.h"
#include "interpreter/executor.h"

namespace simulator::compiler {


void Compiler::run(interpreter::DecodedBB &decodedBB) {
    asmjit::CodeHolder code_holder;
    code_holder.init(runtime_.environment(), runtime_.cpuFeatures());

    asmjit::x86::Compiler compiler_(&code_holder);

    static auto entry_signature = asmjit::FuncSignatureT<void, interpreter::Executor *, const Instruction *>();
    auto *entry_node = compiler_.addFunc(entry_signature);

    executor_p_ = compiler_.newGpq();
    instruction_p_ = compiler_.newGpq();
    entry_node->setArg(0, executor_p_);
    entry_node->setArg(1, instruction_p_);

    auto offset_to_gprf = interpreter::Executor::getOffsetToGprf() +
                                         GPR_file::getOffsetToStartOfRegisters();

    pc_p_ = compiler_.newGpq();
    compiler_.mov(pc_p_, executor_p_);
    compiler_.add(pc_p_, offset_to_gprf + sizeof(Register)*GPR_file::PC);

    registers_p_ = compiler_.newGpq();
    compiler_.mov(registers_p_, executor_p_);
    compiler_.add(registers_p_, offset_to_gprf);

    auto body = decodedBB.getBody();

    for (size_t i = 0; i < body.size(); ++i) {
        if (body[i].inst_id != InstructionId::BB_END_INST) {
            compileInstr(body[i], i);
        }
    }

    compiler_.endFunc();
    compiler_.finalize();
    interpreter::DecodedBB::CompiledEntry entry = nullptr;
    runtime_.add(&entry, &code_holder);

    decodedBB.setCompiledEntry(entry);
    decodedBB.setCompileStatus(interpreter::DecodedBB::CompileStatus::COMPILED);
}

void Compiler::compileInvoke(interpreter::DecodedBB::CompiledEntry executor, size_t instr_offset) {
    auto instr = compiler_.newGpq();
    compiler_.mov(instr, instruction_p_);
    compiler_.add(instr, sizeof(Instruction) * instr_offset);

    static auto executor_signature = asmjit::FuncSignatureT<void, interpreter::Executor *, const Instruction *>();
    asmjit::InvokeNode *invokeNode = nullptr;
    compiler_.invoke(&invokeNode, executor, executor_signature);
    invokeNode->setArg(0, executor_p_);
    invokeNode->setArg(1, instr);
}

void Compiler::compileInstr(const Instruction &instr, size_t instr_offset) {
    switch (instr.inst_id) {
        case InstructionId::LUI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::AUIPC:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::JAL:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::JALR:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BEQ:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BNE:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BLT:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BGE:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BLTU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::BGEU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LB:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LH:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LD:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LBU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LHU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::LWU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SB:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SH:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SD:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADDI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLLI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLTI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLTIU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::XORI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRLI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRAI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ORI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ANDI:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADDIW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLLIW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRLIW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRAIW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADD:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLL:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLT:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLTU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::XOR:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRL:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::OR:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::AND:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SUB:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRA:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ADDW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SUBW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SLLW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRLW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::SRAW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::FENCE:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::ECALL:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::EBREAK:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::MUL:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::MULH:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::MULHSU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::MULHU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::DIV:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::DIVU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::REM:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::REMU:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::MULW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::DIVW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::DIVUW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::REMW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        case InstructionId::REMUW:
            compileInvoke(interpreter::runInstrIface, instr_offset);
            return;
        default:
            std::abort();
    }
}

}  // namespace simulator::compiler
