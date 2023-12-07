#include <gtest/gtest.h>
#include <vector>
#include <interpreter/decoder.h>

namespace simulator {

class DecoderTest : public ::testing::Test {
protected:
    interpreter::Decoder decode_;
};

TEST_F(DecoderTest, BEQTest)
{
    uint32_t raw_inst = 0x5e220863;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x4);
    ASSERT_EQ(instr.rs2, 0x2);
    ASSERT_EQ(instr.imm, 1520);
    ASSERT_EQ(instr.inst_id, InstructionId::BEQ);
}

TEST_F(DecoderTest, BNETest)
{
    uint32_t raw_inst = 0xc0349263;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x9);
    ASSERT_EQ(instr.rs2, 0x3);
    ASSERT_EQ(instr.imm, 5124);
    ASSERT_EQ(instr.inst_id, InstructionId::BNE);
}

TEST_F(DecoderTest, BLTTest)
{
    uint32_t raw_inst = 0x1234cc63;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x9);
    ASSERT_EQ(instr.rs2, 0x3);
    ASSERT_EQ(instr.imm, 312);
    ASSERT_EQ(instr.inst_id, InstructionId::BLT);
}

TEST_F(DecoderTest, BGETest)
{
    uint32_t raw_inst = 0xc186db63;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xd);
    ASSERT_EQ(instr.rs2, 0x18);
    ASSERT_EQ(instr.imm, 5142);
    ASSERT_EQ(instr.inst_id, InstructionId::BGE);
}

TEST_F(DecoderTest, BLTUTest)
{
    uint32_t raw_inst = 0x41bf6963;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x1e);
    ASSERT_EQ(instr.rs2, 0x1b);
    ASSERT_EQ(instr.imm, 1042);
    ASSERT_EQ(instr.inst_id, InstructionId::BLTU);
}

TEST_F(DecoderTest, BGEUTest)
{
    uint32_t raw_inst = 0x1db67463;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xc);
    ASSERT_EQ(instr.rs2, 0x1b);
    ASSERT_EQ(instr.imm, 456);
    ASSERT_EQ(instr.inst_id, InstructionId::BGEU);
}

TEST_F(DecoderTest, JALRTest)
{
    uint32_t raw_inst = 0x042389e7;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rd, 0x13);
    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.imm, 66);
    ASSERT_EQ(instr.inst_id, InstructionId::JALR);
}

TEST_F(DecoderTest, JALTest)
{
    uint32_t raw_inst = 0x78800fef;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rd, 0x1f);
    ASSERT_EQ(instr.imm, 1928);
    ASSERT_EQ(instr.inst_id, InstructionId::JAL);
}

TEST_F(DecoderTest, LUITest)
{
    uint32_t raw_inst = 0x0101dbb7;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rd, 0x17);
    ASSERT_EQ(instr.imm, 16896000);
    ASSERT_EQ(instr.inst_id, InstructionId::LUI);
}

TEST_F(DecoderTest, AUIPCTest)
{
    uint32_t raw_inst = 0x011af697;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rd, 0xd);
    ASSERT_EQ(instr.imm, 18542592);
    ASSERT_EQ(instr.inst_id, InstructionId::AUIPC);
}

TEST_F(DecoderTest, ADDITest)
{
    uint32_t raw_inst = 0x1af68293;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xd);
    ASSERT_EQ(instr.rd, 0x5);
    ASSERT_EQ(instr.imm, 431);
    ASSERT_EQ(instr.inst_id, InstructionId::ADDI);
}

TEST_F(DecoderTest, SLLITest)
{
    uint32_t raw_inst = 0x00c49393;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x9);
    ASSERT_EQ(instr.rd, 0x7);
    ASSERT_EQ(instr.imm, 12 << 20);
    ASSERT_EQ(instr.inst_id, InstructionId::SLLI);
}

TEST_F(DecoderTest, SLTITest)
{
    uint32_t raw_inst = 0x00c4a393;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x9);
    ASSERT_EQ(instr.rd, 0x7);
    ASSERT_EQ(instr.imm, 12);
    ASSERT_EQ(instr.inst_id, InstructionId::SLTI);
}

TEST_F(DecoderTest, SLIUTest)
{
    uint32_t raw_inst = 0x00c4b393;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x9);
    ASSERT_EQ(instr.rd, 0x7);
    ASSERT_EQ(instr.imm, 12);
    ASSERT_EQ(instr.inst_id, InstructionId::SLTIU);
}

TEST_F(DecoderTest, SRLITest)
{
    uint32_t raw_inst = 0x02dada13;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x15);
    ASSERT_EQ(instr.rd, 0x14);
    ASSERT_EQ(instr.imm, 45 << 20);
    ASSERT_EQ(instr.inst_id, InstructionId::SRLI);
}

TEST_F(DecoderTest, SRAITest)
{
    uint32_t raw_inst = 0x42dada13;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x15);
    ASSERT_EQ(instr.rd, 0x14);
    ASSERT_EQ(instr.imm, 45 << 20);
    ASSERT_EQ(instr.inst_id, InstructionId::SRAI);
}

TEST_F(DecoderTest, XORITest)
{
    uint32_t raw_inst = 0x1a574213;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.imm, 421);
    ASSERT_EQ(instr.inst_id, InstructionId::XORI);
}

TEST_F(DecoderTest, ORITest)
{
    uint32_t raw_inst = 0x1a576213;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.imm, 421);
    ASSERT_EQ(instr.inst_id, InstructionId::ORI);
}

TEST_F(DecoderTest, ANDITest)
{
    uint32_t raw_inst = 0x1a577213;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.imm, 421);
    ASSERT_EQ(instr.inst_id, InstructionId::ANDI);
}

TEST_F(DecoderTest, ADDTest)
{
    uint32_t raw_inst = 0x00870233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::ADD);
}

TEST_F(DecoderTest, SUBTest)
{
    uint32_t raw_inst = 0x40870233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SUB);
}

TEST_F(DecoderTest, SLLTest)
{
    uint32_t raw_inst = 0x00871233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SLL);
}

TEST_F(DecoderTest, SLTTest)
{
    uint32_t raw_inst = 0x00872233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SLT);
}

TEST_F(DecoderTest, SLTUTest)
{
    uint32_t raw_inst = 0x00873233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SLTU);
}

TEST_F(DecoderTest, XORTest)
{
    uint32_t raw_inst = 0x00874233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::XOR);
}

TEST_F(DecoderTest, SRLTest)
{
    uint32_t raw_inst = 0x00875233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SRL);
}

TEST_F(DecoderTest, SRATest)
{
    uint32_t raw_inst = 0x40875233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::SRA);
}

TEST_F(DecoderTest, ORTest)
{
    uint32_t raw_inst = 0x00876233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::OR);
}

TEST_F(DecoderTest, ANDTest)
{
    uint32_t raw_inst = 0x00877233;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0xe);
    ASSERT_EQ(instr.rs2, 0x8);
    ASSERT_EQ(instr.rd, 0x4);
    ASSERT_EQ(instr.inst_id, InstructionId::AND);
}

TEST_F(DecoderTest, LBTest)
{
    uint32_t raw_inst = 0x23638303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LB);
}

TEST_F(DecoderTest, LHTest)
{
    uint32_t raw_inst = 0x23639303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LH);
}

TEST_F(DecoderTest, LWTest)
{
    uint32_t raw_inst = 0x2363a303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LW);
}

TEST_F(DecoderTest, LDTest)
{
    uint32_t raw_inst = 0x2363b303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LD);
}

TEST_F(DecoderTest, LBUTest)
{
    uint32_t raw_inst = 0x2363c303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LBU);
}

TEST_F(DecoderTest, LHUTest)
{
    uint32_t raw_inst = 0x2363d303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LHU);
}

TEST_F(DecoderTest, LWUTest)
{
    uint32_t raw_inst = 0x2363e303;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x7);
    ASSERT_EQ(instr.rd, 0x6);
    ASSERT_EQ(instr.imm, 566);
    ASSERT_EQ(instr.inst_id, InstructionId::LWU);
}

TEST_F(DecoderTest, SBTest)
{
    uint32_t raw_inst = 0x31990723;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x12);
    ASSERT_EQ(instr.rs2, 0x19);
    ASSERT_EQ(instr.imm, 782);
    ASSERT_EQ(instr.inst_id, InstructionId::SB);
}

TEST_F(DecoderTest, SHTest)
{
    uint32_t raw_inst = 0x31991723;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x12);
    ASSERT_EQ(instr.rs2, 0x19);
    ASSERT_EQ(instr.imm, 782);
    ASSERT_EQ(instr.inst_id, InstructionId::SH);
}

TEST_F(DecoderTest, SWTest)
{
    uint32_t raw_inst = 0x31992723;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x12);
    ASSERT_EQ(instr.rs2, 0x19);
    ASSERT_EQ(instr.imm, 782);
    ASSERT_EQ(instr.inst_id, InstructionId::SW);
}

TEST_F(DecoderTest, SDTest)
{
    uint32_t raw_inst = 0x31993723;

    Instruction instr = decode_.DecodeInstr(raw_inst);

    ASSERT_EQ(instr.rs1, 0x12);
    ASSERT_EQ(instr.rs2, 0x19);
    ASSERT_EQ(instr.imm, 782);
    ASSERT_EQ(instr.inst_id, InstructionId::SD);
}

}  // namespace simulator