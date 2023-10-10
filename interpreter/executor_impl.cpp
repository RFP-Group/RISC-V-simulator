#include "interpreter/executor.h"
#include "interpreter/gpr.h"
#include "interpreter/instruction.h"

namespace simulator {

#define NEXT()                                                                 \
    {                                                                          \
        gprf_.write(GPR_file::GPR_n::PC, gprf_.read(GPR_file::GPR_n::PC) + 4); \
    }

void Executor::exec_LUI(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    uint32_t shifted_imm = ApplyLeftShift<Immediate_t, 12>(imm);
    gprf_.write(rd, shifted_imm);
    NEXT()
}

void Executor::exec_AUIPC(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    uint32_t shifted_imm = ApplyLeftShift<Immediate_t, 12>(imm);
    Register advanced_pc = gprf_.read(GPR_file::GPR_n::PC);
    advanced_pc += shifted_imm;
    gprf_.write(rd, advanced_pc);
    NEXT()
}

void Executor::exec_JAL(Instruction inst)
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

void Executor::exec_JALR(Instruction inst)
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

void Executor::exec_BEQ(Instruction inst)
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

void Executor::exec_BNE(Instruction inst)
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

void Executor::exec_BLT(Instruction inst)
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

void Executor::exec_BLTU(Instruction inst)
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

void Executor::exec_BGE(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister rs2_val = GetSignedForm<Register>(gprf_.read(rs2));
    if (rs1_val > rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_BGEU(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register pc = gprf_.read(GPR_file::GPR_n::PC);
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    if (rs1_val > rs2_val) {
        Register offset = pc + GetSignedExtension<Register, 12>(imm);
        gprf_.write(GPR_file::GPR_n::PC, offset);
    } else {
        NEXT()
    }
}

void Executor::exec_ADDI(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register res = gprf_.read(rs1) + signed_imm;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLTI(Instruction inst)
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

void Executor::exec_SLTIU(Instruction inst)
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

void Executor::exec_XORI(Instruction inst)
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

void Executor::exec_ANDI(Instruction inst)
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

void Executor::exec_ORI(Instruction inst)
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

void Executor::exec_SLLI(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1 << shamt;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRLI(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = rs1 >> shamt;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRAI(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister res = rs1 >> shamt;
    gprf_.write(rd, GetUnsignedForm<SRegister>(res));
    NEXT()
}

void Executor::exec_ADD(Instruction inst)
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

void Executor::exec_SUB(Instruction inst)
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

void Executor::exec_SLL(Instruction inst)
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

void Executor::exec_SRL(Instruction inst)
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

void Executor::exec_SRA(Instruction inst)
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

void Executor::exec_SLT(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    SRegister rs2_val = GetSignedForm<Register>(gprf_.read(rs2));
    Register res = rs1_val < rs2_val ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SLTU(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    SRegister rs1_val = gprf_.read(rs1);
    SRegister rs2_val = gprf_.read(rs2);
    Register res = rs1_val < rs2_val ? 1 : 0;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_XOR(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val ^ rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_AND(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val & rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_OR(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register rs1_val = gprf_.read(rs1);
    Register rs2_val = gprf_.read(rs2);
    Register res = rs1_val | rs2_val;
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ADDIW(Instruction inst)
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

void Executor::exec_SLLIW(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = GetSignedExtension<Register, 32>(rs1_val << shamt);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRLIW(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    Register rs1_val = gprf_.read(rs1);
    Register res = GetSignedExtension<Register, 32>(rs1_val >> shamt);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SRAIW(Instruction inst)
{
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Immediate_t shamt = inst.GetShamt();
    SRegister rs1_val = GetSignedForm<Register>(gprf_.read(rs1));
    Register res = GetSignedExtension<Register, 32>(GetUnsignedForm<SRegister>(rs1_val >> shamt));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_ADDW(Instruction inst)
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

void Executor::exec_SUBW(Instruction inst)
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

void Executor::exec_SLLW(Instruction inst)
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

void Executor::exec_SRLW(Instruction inst)
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

void Executor::exec_SRAW(Instruction inst)
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

void Executor::exec_LB(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int8_t val = GetSignedForm<uint8_t>(vmem_->ReadByte(addr));
    Register res = GetSignedExtension<Register, 8>(GetUnsignedForm<int8_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LH(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int16_t val = GetSignedForm<uint16_t>(vmem_->ReadTwoBytesFast(addr));
    Register res = GetSignedExtension<Register, 16>(GetUnsignedForm<int16_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LW(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    int32_t val = GetSignedForm<uint32_t>(vmem_->ReadFourBytesFast(addr));
    Register res = GetSignedExtension<Register, 32>(GetUnsignedForm<int32_t>(val));
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LD(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    SRegister val = GetSignedForm<Register>(vmem_->ReadEightBytesFast(addr));
    Register res = GetUnsignedForm<SRegister>(val);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_SB(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(inst.rs2);
    uint8_t val = static_cast<uint8_t>(rs2_val);
    vmem_->LoadByte(addr, val);
    NEXT()
}

void Executor::exec_SH(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(inst.rs2);
    uint16_t val = static_cast<uint16_t>(rs2_val);
    vmem_->LoadTwoBytesFast(addr, val);
    NEXT()
}

void Executor::exec_SW(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(inst.rs2);
    uint32_t val = static_cast<uint32_t>(rs2_val);
    vmem_->LoadFourBytesFast(addr, val);
    NEXT()
}

void Executor::exec_SD(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register_t rs2 = inst.rs2;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register rs2_val = gprf_.read(inst.rs2);
    Register val = static_cast<Register>(rs2_val);
    vmem_->LoadEightBytesFast(addr, val);
    NEXT()
}

void Executor::exec_LBU(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->ReadByte(addr);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LHU(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->ReadTwoBytesFast(addr);
    gprf_.write(rd, res);
    NEXT()
}

void Executor::exec_LWU(Instruction inst)
{
    Immediate_t imm = inst.imm;
    Register_t rd = inst.rd;
    Register_t rs1 = inst.rs1;
    Register signed_imm = GetSignedExtension<Register, 12>(imm);
    Register rs1_val = gprf_.read(rs1);
    Register addr = rs1_val + signed_imm;
    Register res = vmem_->ReadFourBytesFast(addr);
    gprf_.write(rd, res);
    NEXT()
}

}  // namespace simulator