#include <gtest/gtest.h>
#include <vector>
#include <interpreter/executor.h>
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}