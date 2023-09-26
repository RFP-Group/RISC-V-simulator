#include <gtest/gtest.h>
#include "phys_mem.hpp"

namespace simulator {
TEST(PhysMemTest, PhysMemCreateDestroyTest)
{
    mem::PhysMem *phys_mem = mem::PhysMem::CreatePhysMem(16_GB);
    if (!mem::PhysMem::Destroy(phys_mem)) {
        std::cerr << "Couldn't successfully destroy PhysMem" << std::endl;
    }
}
}  // namespace simulator

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}