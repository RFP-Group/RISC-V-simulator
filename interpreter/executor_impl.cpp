#include "interpreter/executor.h"
#include "interpreter/gpr.h"
#include "interpreter/instruction.h"

#include <iostream>

namespace simulator::interpreter {

#define NEXT()                                                                 \
    {                                                                          \
        gprf_.write(GPR_file::GPR_n::PC, gprf_.read(GPR_file::GPR_n::PC) + 4); \
    }

void Executor::exec_LUI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    gprf_.write(rd, imm);
    NEXT()
}

void Executor::exec_AUIPC([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register advanced_pc = gprf_.read(GPR_file::GPR_n::PC);
    uint32_t shifted_imm = ApplyLeftShift<Immediate_t, 12>(imm);
    advanced_pc += shifted_imm;
    gprf_.write(rd, advanced_pc);
    NEXT()
}

void Executor::exec_JAL([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register rd = inst.rd;
    Register signed_imm = GetSignedExtension<Register, 20>(imm);
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    auto advanced_pc = pc + 4;
    pc += signed_imm;
    gprf_.write(rd, advanced_pc);
    gprf_.write(GPR_file::GPR_n::PC, pc);
}

void Executor::exec_JALR([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register offset = (gprf_.read(rs1) + signed_imm) & (~static_cast<Register>(1));
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    pc += 4;
    gprf_.write(GPR_file::GPR_n::PC, offset);
    gprf_.write(rd, pc);
}

void Executor::exec_BEQ([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    if (rs1_val == rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BNE([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    if (rs1_val != rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BLT([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister rs2_val = GetSignedForm<Register>(gprf_.read(rs2));
    if (rs1_val < rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BLTU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    if (rs1_val < rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BGE([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister rs2_val = GetSignedForm<Register>(gprf_.read(rs2));
    if (rs1_val >= rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BGEU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    if (rs1_val >= rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_ADDI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register res = gprf_.read(rs1) + signed_imm;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLTI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    SRegister signed_imm = GetSignedForm<Register>(GetSignedExtension<Register, 12>(imm));
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    Register res = rs1_val < signed_imm ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLTIU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val < signed_imm ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_XORI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val ^ signed_imm;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ANDI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val & signed_imm;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ORI([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val | signed_imm;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLLI([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val << shamt;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRLI([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1_val >> shamt;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRAI([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister res = rs1_val >> shamt;
    gprf_.write(rd, GetUnsignedForm<SRegister>(res));
    NEXT()
}

void Executor::exec_ADD([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val + rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SUB([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val - rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLL([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val << rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRL([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val >> rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRA([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    SRegister res = rs1_val >> rs2_val;
    gprf_.write(rd, GetUnsignedForm<SRegister>(res));
    NEXT()
}

void Executor::exec_SLT([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister rs2_val = GetSignedForm<Register>(gprf_.read(rs2));
    Register res = rs1_val < rs2_val ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLTU([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = gprf_.read(rs1);
    SRegister rs2_val = gprf_.read(rs2);
    Register res = rs1_val < rs2_val ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_XOR([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val ^ rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_AND([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val & rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_OR([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val | rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ADDIW([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register res = GetSignedExtension<Register, 32>(rs1_val + signed_imm);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLLIW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = GetSignedExtension<Register, 32>(rs1_val << shamt);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRLIW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = GetSignedExtension<Register, 32>(rs1_val >> shamt);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRAIW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    Register res = GetSignedExtension<Register, 32>(GetUnsignedForm<SRegister>(rs1_val >> shamt));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ADDW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = GetSignedExtension<Register, 32>(rs1_val + rs2_val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SUBW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = GetSignedExtension<Register, 32>(rs1_val - rs2_val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLLW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = GetSignedExtension<Register, 32>(rs1_val << rs2_val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRLW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = GetSignedExtension<Register, 32>(rs1_val >> rs2_val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRAW([[maybe_unused]] Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = GetSignedExtension<Register, 32>(GetUnsignedForm<SRegister>(rs1_val >> rs2_val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LB([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int8_t val = GetSignedForm<uint8_t>(vmem_->LoadByte(addr));
    Register res = GetSignedExtension<Register, 8>(GetUnsignedForm<int8_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LH([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int16_t val = GetSignedForm<uint16_t>(vmem_->LoadTwoBytesFast(addr));
    Register res = GetSignedExtension<Register, 16>(GetUnsignedForm<int16_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LW([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int32_t val = GetSignedForm<uint32_t>(vmem_->LoadFourBytesFast(addr));
    Register res = GetSignedExtension<Register, 32>(GetUnsignedForm<int32_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LD([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    SRegister val = GetSignedForm<Register>(vmem_->LoadEightBytesFast(addr));
    Register res = GetUnsignedForm<SRegister>(val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SB([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(rs2);
    uint8_t val = static_cast<uint8_t>(rs2_val);
    vmem_->StoreByte(addr, val);
    NEXT()
}

void Executor::exec_SH([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(rs2);
    uint16_t val = static_cast<uint16_t>(rs2_val);
    vmem_->StoreTwoBytesFast(addr, val);
    NEXT()
}

void Executor::exec_SW([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(rs2);
    uint32_t val = static_cast<uint32_t>(rs2_val);
    vmem_->StoreFourBytesFast(addr, val);
    NEXT()
}

void Executor::exec_SD([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(rs2);
    Register val = static_cast<Register>(rs2_val);
    vmem_->StoreEightBytesFast(addr, val);
    NEXT()
}

void Executor::exec_LBU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->LoadByte(addr);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LHU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->LoadTwoBytesFast(addr);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LWU([[maybe_unused]] Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->LoadFourBytesFast(addr);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ECALL([[maybe_unused]] Instruction inst)
{
    Register syscall = gprf_.read(GPR_file::GPR_n::X17);
    switch (syscall) {
        case 64: {
            Register addr = gprf_.read(GPR_file::GPR_n::X11);
            Register length = gprf_.read(GPR_file::GPR_n::X12);

            std::vector<uint8_t> out_vector = vmem_->LoadByteSequence(addr, length);
            std::string out_str(out_vector.begin(), out_vector.end());
            std::cout << out_str;
            break;
        }
        default: {
            std::abort();
            break;
        }
    }
    NEXT()
}

void Executor::exec_FENCE([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FENCE_I([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MUL([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MULH([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MULHSU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MULHU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_DIV([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_DIVU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_REM([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_REMU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MULW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_DIVW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_DIVUW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_REMW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_REMUW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOADD_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOXOR_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOOR_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOAND_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMIN_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMAX_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMINU_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMAXU_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOSWAP_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_LR_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_SC_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOADD_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOXOR_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOOR_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOAND_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMIN_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMAX_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMINU_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOMAXU_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_AMOSWAP_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_LR_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_SC_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_EBREAK([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_URET([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_SRET([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_MRET([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_DRET([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_SFENCE_VMA([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_WFI([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_CSRRW([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, gprf_.read(rs1));
    gprf_.write(rd, csr_val);
}
void Executor::exec_CSRRS([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, csr_val | gprf_.read(rs1));
    gprf_.write(rd, csr_val);
}
void Executor::exec_CSRRC([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, csr_val & (~gprf_.read(rs1)));
    gprf_.write(rd, csr_val);
}
void Executor::exec_CSRRWI([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    uint64_t zimm = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, zimm);
    gprf_.write(rd, csr_val);
}
void Executor::exec_CSRRSI([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    uint64_t zimm = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, csr_val | zimm);
    gprf_.write(rd, csr_val);
}
void Executor::exec_CSRRCI([[maybe_unused]] Instruction inst)
{
    uint16_t csr_addr = inst.imm;
    Register_t rd = inst.rd;
    uint64_t zimm = inst.rs1;
    Register csr_val = csrf_.read(csr_addr);
    csrf_.write(csr_addr, csr_val & (~zimm));
    gprf_.write(rd, csr_val);
}
void Executor::exec_HFENCE_VVMA([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_HFENCE_GVMA([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FADD_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSUB_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMUL_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FDIV_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJ_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJN_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJX_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMIN_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMAX_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSQRT_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FADD_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSUB_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMUL_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FDIV_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJ_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJN_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJX_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMIN_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMAX_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSQRT_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FADD_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSUB_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMUL_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FDIV_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJ_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJN_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSGNJX_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMIN_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMAX_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSQRT_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLE_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLT_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FEQ_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLE_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLT_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FEQ_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLE_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLT_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FEQ_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_W_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_WU_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_L_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_LU_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_X_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCLASS_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_W_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_WU_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_L_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_LU_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_X_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCLASS_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_W_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_WU_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_L_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_LU_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_X_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCLASS_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_WU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_L([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_S_LU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_W_X([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_WU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_L([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_D_LU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_D_X([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_W([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_WU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_L([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FCVT_Q_LU([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMV_Q_X([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLD([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FLQ([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSW([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSD([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FSQ([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMADD_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMSUB_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMSUB_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMADD_S([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMADD_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMSUB_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMSUB_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMADD_D([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMADD_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FMSUB_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMSUB_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}
void Executor::exec_FNMADD_Q([[maybe_unused]] Instruction inst)
{
    std::abort();
}

}  // namespace simulator::interpreter