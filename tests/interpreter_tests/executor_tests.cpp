#include <gtest/gtest.h>
#include <vector>
#include <interpreter/executor.h>
#include "interpreter/gpr.h"
#include "virtual_mem.hpp"

namespace simulator {

class ExecutorTest : public ::testing::Test {
protected:
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    interpreter::Executor exec_ {virtual_mem, 0};

    void TearDown() override
    {
        mem::VirtualMem::Destroy(virtual_mem);
    }
};

TEST_F(ExecutorTest, LUITest)
{
    // lui x3, 0x5a
    Instruction lui = {0, 0, 0, GPR_file::X3, 0, 0x5a, 55, InstructionId::LUI};

    exec_.RunImpl(lui);

    auto gpr = exec_.getGPRfile();
    ASSERT_EQ(gpr.read(GPR_file::X3), 0x5a000);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x4);
}

TEST_F(ExecutorTest, AUIPCTest)
{
    // auipc x3, 0x34f0
    Instruction auipc = {0, 0, 0, GPR_file::X3, 0, 0x34f0, 23, InstructionId::AUIPC};

    exec_.RunImpl(auipc);

    auto gpr = exec_.getGPRfile();
    ASSERT_EQ(gpr.read(GPR_file::X3), 0x34f0000);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x4);
}

TEST_F(ExecutorTest, JALTest)
{
    // jal x3, 0x200
    Instruction jal = {0, 0, 0, GPR_file::X7, 0, 0x200, 111, InstructionId::JAL};

    exec_.RunImpl(jal);

    auto gpr = exec_.getGPRfile();
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x4);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x200);
}

TEST_F(ExecutorTest, JALRTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x12
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // jalr x7, x4, 0x300
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x300, 103, InstructionId::JALR}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X7), 0x8);
    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x310);
}

TEST_F(ExecutorTest, BEQTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // addi x5, x0, 0x21
                                             {GPR_file::X0, 0, 0, GPR_file::X5, 0, 0x21, 19, InstructionId::ADDI},
                                             // addi x6, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X6, 0, 0x11, 19, InstructionId::ADDI},
                                             // beq x5, x4, 0x300
                                             {GPR_file::X4, GPR_file::X5, 0, 0, 0, 0x300, 99, InstructionId::BEQ},
                                             // beq x6, x4, 0x300
                                             {GPR_file::X4, GPR_file::X6, 0, 0, 0, 0x300, 99, InstructionId::BEQ}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::X5), 0x21);
    ASSERT_EQ(gpr.read(GPR_file::X6), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x310);
}

TEST_F(ExecutorTest, BNETest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // addi x5, x0, 0x21
                                             {GPR_file::X0, 0, 0, GPR_file::X5, 0, 0x21, 19, InstructionId::ADDI},
                                             // addi x6, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X6, 0, 0x11, 19, InstructionId::ADDI},
                                             // bne x6, x4, 0x300
                                             {GPR_file::X4, GPR_file::X6, 0, 0, 0, 0x300, 99, InstructionId::BNE},
                                             // bne x5, x4, 0x300
                                             {GPR_file::X4, GPR_file::X5, 0, 0, 0, 0x300, 99, InstructionId::BNE}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::X5), 0x21);
    ASSERT_EQ(gpr.read(GPR_file::X6), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x310);
}

TEST_F(ExecutorTest, BLTTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // addi x5, x0, 0x21
                                             {GPR_file::X0, 0, 0, GPR_file::X5, 0, 0x21, 19, InstructionId::ADDI},
                                             // addi x6, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X6, 0, 0x11, 19, InstructionId::ADDI},
                                             // blt x6, x4, 0x300
                                             {GPR_file::X4, GPR_file::X6, 0, 0, 0, 0x300, 99, InstructionId::BLT},
                                             // blt x5, x4, 0x300
                                             {GPR_file::X4, GPR_file::X5, 0, 0, 0, 0x300, 99, InstructionId::BLT}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::X5), 0x21);
    ASSERT_EQ(gpr.read(GPR_file::X6), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x310);
}

TEST_F(ExecutorTest, BGETest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // addi x5, x0, 0x21
                                             {GPR_file::X0, 0, 0, GPR_file::X5, 0, 0x21, 19, InstructionId::ADDI},
                                             // addi x6, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X6, 0, 0x11, 19, InstructionId::ADDI},
                                             // blt x5, x4, 0x300
                                             {GPR_file::X4, GPR_file::X5, 0, 0, 0, 0x300, 99, InstructionId::BLT},
                                             // blt x6, x4, 0x300
                                             {GPR_file::X4, GPR_file::X6, 0, 0, 0, 0x300, 99, InstructionId::BLT}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::X5), 0x21);
    ASSERT_EQ(gpr.read(GPR_file::X6), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x310);
}

TEST_F(ExecutorTest, SLTITest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, -5
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, static_cast<Immediate_t>(-5), 19, InstructionId::ADDI},
        // slti x7, x4, 4
        {GPR_file::X4, 0, 0, GPR_file::X7, 0, 4, 19, InstructionId::SLTI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), static_cast<Register>(-5));
    ASSERT_EQ(gpr.read(GPR_file::X7), 1);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, SLTIUTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 2
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 2, 19, InstructionId::ADDI},
                                             // slti x7, x4, 4
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 4, 19, InstructionId::SLTI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 2);
    ASSERT_EQ(gpr.read(GPR_file::X7), 1);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, XORITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x5e
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5e, 19, InstructionId::ADDI},
                                             // xori x7, x4, 0xff
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0xff, 19, InstructionId::XORI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5e);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0xa1);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, ANDITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x5e
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5e, 19, InstructionId::ADDI},
                                             // andi x7, x4, 0x3d
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x3d, 19, InstructionId::ANDI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5e);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x1c);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, ORITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x5e
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5e, 19, InstructionId::ADDI},
                                             // ori x7, x4, 0x3d
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x3d, 19, InstructionId::ORI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5e);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x7f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, SLLITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x11
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x11, 19, InstructionId::ADDI},
                                             // slli x7, x4, 0x5
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x500000, 19, InstructionId::SLLI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x11);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x220);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, SRLITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x403ff
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x403ff, 19, InstructionId::ADDI},
                                             // srli x7, x4, 0x5
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x500000, 19, InstructionId::SRLI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x3ff);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x1f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, SRAITest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0xff5a
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0xff5a, 19, InstructionId::ADDI},
                                             // srai x7, x4, 0x5
                                             {GPR_file::X4, 0, 0, GPR_file::X7, 0, 0x500000, 19, InstructionId::SRAI}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), static_cast<Register>(-166));
    ASSERT_EQ(gpr.read(GPR_file::X7), static_cast<Register>(-6));
    ASSERT_EQ(gpr.read(GPR_file::PC), 0x8);
}

TEST_F(ExecutorTest, ADDTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x5a
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5a, 19, InstructionId::ADDI},
        // addi x9, x0, 0xac
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0xac, 19, InstructionId::ADDI},
        // add x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 51, InstructionId::ADD}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5a);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x106);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0xac);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SUBTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x5a
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5a, 19, InstructionId::ADDI},
        // addi x9, x0, 0xac
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0xac, 19, InstructionId::ADDI},
        // sub x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 51, InstructionId::SUB}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5a);
    ASSERT_EQ(gpr.read(GPR_file::X7), static_cast<Register>(-82));
    ASSERT_EQ(gpr.read(GPR_file::X9), 0xac);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SLLTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x5a
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5a, 19, InstructionId::ADDI},
        // addi x9, x0, 0xa
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0xa, 19, InstructionId::ADDI},
        // sll x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 51, InstructionId::SLL}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x5a);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x16800);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0xa);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SRLTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x5a
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5a00, 19, InstructionId::ADDI},
        // addi x9, x0, 0x5
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x5, 19, InstructionId::ADDI},
        // srl x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 51, InstructionId::SRL}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), static_cast<Register>(-1536));
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x7ffffffffffffd0);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x5);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SRATest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x5a
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x5a00, 19, InstructionId::ADDI},
        // addi x9, x0, 0x5
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x5, 19, InstructionId::ADDI},
        // sra x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 51, InstructionId::SRA}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), static_cast<Register>(-1536));
    ASSERT_EQ(gpr.read(GPR_file::X7), static_cast<Register>(-48));
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x5);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SLTTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x54
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x54, 19, InstructionId::ADDI},
        // addi x9, x0, 0x32f
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x32f, 19, InstructionId::ADDI},
        // slt x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 19, InstructionId::SLT}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x54);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x1);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x32f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SLTUTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x54
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x54, 19, InstructionId::ADDI},
        // addi x9, x0, 0x32f
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x32f, 19, InstructionId::ADDI},
        // slt x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 19, InstructionId::SLTU}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x54);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x1);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x32f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, XORTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x54
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x54, 19, InstructionId::ADDI},
        // addi x9, x0, 0x32f
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x32f, 19, InstructionId::ADDI},
        // xor x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 0, InstructionId::XOR}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x54);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x37b);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x32f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, ANDTest)
{
    std::vector<Instruction> instructions = {
        // addi x4, x0, 0x54
        {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x54, 19, InstructionId::ADDI},
        // addi x9, x0, 0x32f
        {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x32f, 19, InstructionId::ADDI},
        // and x7, x4, x9
        {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 0, InstructionId::AND}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x54);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x4);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x32f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, ORTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x54
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x54, 19, InstructionId::ADDI},
                                             // addi x9, x0, 0x32f
                                             {GPR_file::X0, 0, 0, GPR_file::X9, 0, 0x32f, 19, InstructionId::ADDI},
                                             // or x7, x4, x9
                                             {GPR_file::X4, GPR_file::X9, 0, GPR_file::X7, 0, 0, 0, InstructionId::OR}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x54);
    ASSERT_EQ(gpr.read(GPR_file::X7), 0x37f);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x32f);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, SW_LWTest)
{
    std::vector<Instruction> instructions = {// addi x4, x0, 0x10
                                             {GPR_file::X0, 0, 0, GPR_file::X4, 0, 0x10, 19, InstructionId::ADDI},
                                             // sw x4, 0x4(x5)
                                             {GPR_file::X5, GPR_file::X4, 0, 0, 0, 0x4, 19, InstructionId::ADDI},
                                             // lw x9, 0x4(x5)
                                             {GPR_file::X4, 0, 0, GPR_file::X9, 0, 0, 0, InstructionId::OR}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    auto gpr = exec_.getGPRfile();

    ASSERT_EQ(gpr.read(GPR_file::X4), 0x10);
    ASSERT_EQ(gpr.read(GPR_file::X5), 0);
    ASSERT_EQ(gpr.read(GPR_file::X9), 0x10);
    ASSERT_EQ(gpr.read(GPR_file::PC), 0xc);
}

TEST_F(ExecutorTest, CSRTest)
{
    std::vector<Instruction> instructions = {
        // addi t0, zero, 1
        {GPR_file::X0, 0, 0, GPR_file::X5, 0, 1, 19, InstructionId::ADDI},
        // addi t1, zero, 2
        {GPR_file::X0, 0, 0, GPR_file::X6, 0, 2, 19, InstructionId::ADDI},
        // addi t2, zero, 3
        {GPR_file::X0, 0, 0, GPR_file::X7, 0, 3, 0, InstructionId::ADDI},
        // csrrw zero, mstatus, t0
        {GPR_file::X5, 0, 0, GPR_file::X0, 0, CSR_file::MSTATUS, 0, InstructionId::CSRRW},
        // csrrs zero, mtvec, t1
        {GPR_file::X6, 0, 0, GPR_file::X0, 0, CSR_file::MTVEC, 0, InstructionId::CSRRS},
        // csrrw zero, mepc, t2
        {GPR_file::X7, 0, 0, GPR_file::X0, 0, CSR_file::MEPC, 0, InstructionId::CSRRW},
        // csrrc t2, mepc, zero
        {GPR_file::X0, 0, 0, GPR_file::X7, 0, CSR_file::MEPC, 0, InstructionId::CSRRC},
        // csrrwi zero, sstatus, 4
        {4, 0, 0, GPR_file::X0, 0, CSR_file::SSTATUS, 0, InstructionId::CSRRWI},
        // csrrsi zero, stvec, 5
        {5, 0, 0, GPR_file::X0, 0, CSR_file::STVEC, 0, InstructionId::CSRRSI},
        // csrrwi zero, sepc, 6
        {6, 0, 0, GPR_file::X0, 0, CSR_file::SEPC, 0, InstructionId::CSRRWI},
        // csrrci zero, sepc, 0
        {0, 0, 0, GPR_file::X0, 0, CSR_file::SEPC, 0, InstructionId::CSRRCI},
        // csrrw x0, mstatus, x5
        {GPR_file::X5, 0, 0, GPR_file::X0, 0, CSR_file::MSTATUS, 0, InstructionId::CSRRW}};

    for (auto &&instr : instructions)
        exec_.RunImpl(instr);

    CSR_file csr = exec_.getCSRfile();
    ASSERT_EQ(csr.read(CSR_file::MSTATUS), 1);
    ASSERT_EQ(csr.read(CSR_file::MTVEC), 2);
    ASSERT_EQ(csr.read(CSR_file::MEPC), 3);
    ASSERT_EQ(csr.read(CSR_file::SSTATUS), 0);
    ASSERT_EQ(csr.read(CSR_file::STVEC), 5);
    ASSERT_EQ(csr.read(CSR_file::SEPC), 6);
}

}  // namespace simulator