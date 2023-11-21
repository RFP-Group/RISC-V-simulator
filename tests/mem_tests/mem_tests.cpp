#include <gtest/gtest.h>
#include "phys_mem.hpp"
#include "virtual_mem.hpp"

namespace simulator {
TEST(PhysVirtualMemTest, PhysVirtualMemCreateDestroyTest)
{
    mem::PhysMem *phys_mem = mem::PhysMem::CreatePhysMem(16_GB);
    ASSERT_TRUE(mem::PhysMem::Destroy(phys_mem)) << "Couldn't successfully destroy PhysMem";
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    ASSERT_TRUE(mem::VirtualMem::Destroy(virtual_mem)) << "Couldn't successfully destroy VirtualMem";
}

TEST(VirtualMemTest, VirtualMemSmallStoreLoadSequenceTest)
{
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    std::vector<uint8_t> arr(100, 3);
    uintptr_t addr_to_load = 436276323;
    virtual_mem->StoreByteSequence(addr_to_load, arr.data(), arr.size());
    std::vector<uint8_t> stored_arr = virtual_mem->LoadByteSequence(addr_to_load, arr.size());
    ASSERT_EQ(arr, stored_arr);
    ASSERT_TRUE(mem::VirtualMem::Destroy(virtual_mem));
}

TEST(VirtualMemTest, VirtualMemSmallStoreLoadFastTest)
{
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    std::vector<uint8_t> arr = {255, 255, 5, 5};
    uintptr_t addr_to_load = 436276323;
    virtual_mem->StoreByteSequence(addr_to_load, arr.data(), arr.size());
    uint16_t two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load);
    ASSERT_EQ(two_bytes, 65535);  // 11111111 11111111
    two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load + 1);
    ASSERT_EQ(two_bytes, 1535);  // 00000101 11111111
    two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load + 2);
    ASSERT_EQ(two_bytes, 1285);  // 00000101 00000101
    uint32_t four_bytes = virtual_mem->LoadFourBytesFast(addr_to_load);
    ASSERT_EQ(four_bytes, 84279295);  // 00000101 00000101 11111111 11111111
    ASSERT_TRUE(mem::VirtualMem::Destroy(virtual_mem));
}

TEST(VirtualMemTest, VirtualMemFastStoreLoadFastTest)
{
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    uintptr_t addr_to_load = -15;
    virtual_mem->StoreEightBytesFast(addr_to_load, 1);
    // 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000
    ASSERT_EQ(virtual_mem->LoadTwoBytesFast(addr_to_load), 1);
    ASSERT_EQ(virtual_mem->LoadEightBytesFast(addr_to_load), 1);
    ASSERT_EQ(virtual_mem->LoadTwoBytesFast(addr_to_load + 6), 0);
    ASSERT_EQ(virtual_mem->LoadByte(addr_to_load + 7), 0);
    ASSERT_TRUE(mem::VirtualMem::Destroy(virtual_mem));
}

TEST(VirtualMemTest, VirtualMemRandomAllocationTest)
{
    auto seed = std::time(nullptr);
    std::cerr << "Random generation of addresses to load with seed: " << seed << std::endl;
    std::srand(seed);
    std::vector<uint8_t> arr = {255, 255, 5, 5};
    mem::VirtualMem *virtual_mem = mem::VirtualMem::CreateVirtualMem();
    for (size_t i = 0; i < TwoPow<8>(); ++i) {
        uintptr_t addr_to_load = static_cast<uintptr_t>(std::rand());
        virtual_mem->StoreByteSequence(addr_to_load, arr.data(), arr.size());
        uint16_t two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load);
        ASSERT_EQ(two_bytes, 65535);  // 11111111 11111111
        two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load + 1);
        ASSERT_EQ(two_bytes, 1535);  // 00000101 11111111
        two_bytes = virtual_mem->LoadTwoBytesFast(addr_to_load + 2);
        ASSERT_EQ(two_bytes, 1285);  // 00000101 00000101
        uint32_t four_bytes = virtual_mem->LoadFourBytesFast(addr_to_load);
        ASSERT_EQ(four_bytes, 84279295);  // 00000101 00000101 11111111 11111111
    }
    ASSERT_TRUE(mem::VirtualMem::Destroy(virtual_mem));
}

}  // namespace simulator

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}