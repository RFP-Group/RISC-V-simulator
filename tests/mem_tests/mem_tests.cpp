#include <gtest/gtest.h>
#include "phys_mem.hpp"
#include "mmu.hpp"

namespace simulator {
TEST(PhysMMUTest, PhysMMUCreateDestroyTest)
{
    mem::PhysMem *phys_mem = mem::PhysMem::CreatePhysMem(8_GB);
    ASSERT_TRUE(mem::PhysMem::Destroy(phys_mem)) << "Couldn't successfully destroy PhysMem";
    mem::MMU *mmu = mem::MMU::CreateMMU();
    ASSERT_TRUE(mem::MMU::Destroy(mmu)) << "Couldn't successfully destroy MMU";
}

TEST(MMUTest, MMUSmallStoreLoadSequenceTest)
{
    mem::MMU *mmu = mem::MMU::CreateMMU();
    std::vector<uint8_t> arr(100, 3);
    uintptr_t addr_to_load = 436276323;
    mmu->StoreByteSequence(addr_to_load, arr.data(), arr.size());
    std::vector<uint8_t> stored_arr = mmu->LoadByteSequence(addr_to_load, arr.size());
    ASSERT_EQ(arr, stored_arr);
    ASSERT_TRUE(mem::MMU::Destroy(mmu));
}

TEST(MMUTest, MMUSmallStoreLoadFastTest)
{
    mem::MMU *mmu = mem::MMU::CreateMMU();
    std::vector<uint8_t> arr = {255, 255, 5, 5};
    uintptr_t addr_to_load = 436276323;
    mmu->StoreByteSequence(addr_to_load, arr.data(), arr.size());
    uint16_t two_bytes = mmu->LoadTwoBytesFast(addr_to_load);
    ASSERT_EQ(two_bytes, 65535);  // 11111111 11111111
    two_bytes = mmu->LoadTwoBytesFast(addr_to_load + 1);
    ASSERT_EQ(two_bytes, 1535);  // 00000101 11111111
    two_bytes = mmu->LoadTwoBytesFast(addr_to_load + 2);
    ASSERT_EQ(two_bytes, 1285);  // 00000101 00000101
    uint32_t four_bytes = mmu->LoadFourBytesFast(addr_to_load);
    ASSERT_EQ(four_bytes, 84279295);  // 00000101 00000101 11111111 11111111
    ASSERT_TRUE(mem::MMU::Destroy(mmu));
}

TEST(MMUTest, MMUFastStoreLoadFastTest)
{
    mem::MMU *mmu = mem::MMU::CreateMMU();
    uintptr_t addr_to_load = -15;
    mmu->StoreEightBytesFast(addr_to_load, 1);
    // 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    ASSERT_EQ(mmu->LoadTwoBytesFast(addr_to_load), 1);
    ASSERT_EQ(mmu->LoadEightBytesFast(addr_to_load), 1);
    ASSERT_EQ(mmu->LoadTwoBytesFast(addr_to_load + 6), 0);
    ASSERT_EQ(mmu->LoadByte(addr_to_load + 7), 0);
    ASSERT_TRUE(mem::MMU::Destroy(mmu));
}

TEST(MMUTest, MMURandomAllocationTest)
{
    auto seed = std::time(nullptr);
    std::cerr << "Random generation of addresses to load with seed: " << seed << std::endl;
    std::srand(seed);
    std::vector<uint8_t> arr = {255, 255, 5, 5};
    mem::MMU *mmu = mem::MMU::CreateMMU();
    for (size_t i = 0; i < TwoPow<8>(); ++i) {
        uintptr_t addr_to_load = static_cast<uintptr_t>(std::rand());
        for (size_t j = 0; j < 8; ++j) {
            if ((addr_to_load + j) % 8 == 0) {
                addr_to_load = addr_to_load + j;  // Aligning to place objects on one page
                break;
            }
        }
        mmu->StoreByteSequence(addr_to_load, arr.data(), arr.size());
        uint16_t two_bytes = mmu->LoadTwoBytesFast(addr_to_load);
        ASSERT_EQ(two_bytes, 65535);  // 11111111 11111111
        two_bytes = mmu->LoadTwoBytesFast(addr_to_load + 1);
        ASSERT_EQ(two_bytes, 1535);  // 00000101 11111111
        two_bytes = mmu->LoadTwoBytesFast(addr_to_load + 2);
        ASSERT_EQ(two_bytes, 1285);  // 00000101 00000101
        uint32_t four_bytes = mmu->LoadFourBytesFast(addr_to_load);
        ASSERT_EQ(four_bytes, 84279295);  // 00000101 00000101 11111111 11111111
    }
    ASSERT_TRUE(mem::MMU::Destroy(mmu));
}

}  // namespace simulator

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}