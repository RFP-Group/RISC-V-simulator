#ifndef MEMORY_INCLUDES_MMU_HPP
#define MEMORY_INCLUDES_MMU_HPP

#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include "phys_mem.hpp"

namespace simulator::mem {

class MMU final {
public:
    static constexpr size_t PTE_SIZE = 8;
    static constexpr size_t TLB_SIZE = 4096;
    NO_COPY_SEMANTIC(MMU)
    NO_MOVE_SEMANTIC(MMU)

    [[nodiscard]] static MMU *CreateMMU();
    static bool Destroy(MMU *mmu);
    bool StoreByteSequence(uintptr_t addr, uint8_t *chrs, uint64_t length);
    std::vector<uint8_t> LoadByteSequence(uintptr_t addr, uint64_t length);
    void StoreByte(uintptr_t addr, uint8_t chr);
    uint8_t LoadByte(uintptr_t addr);
    void StoreTwoBytesFast(uintptr_t addr, uint16_t value);
    uint16_t LoadTwoBytesFast(uintptr_t addr);
    void StoreFourBytesFast(uintptr_t addr, uint32_t value);
    uint32_t LoadFourBytesFast(uintptr_t addr);
    void StoreEightBytesFast(uintptr_t addr, uint64_t value);
    uint64_t LoadEightBytesFast(uintptr_t addr);
    [[nodiscard]] uintptr_t StoreElfFile(const std::string &name);

private:
    MMU();
    ~MMU();
    inline uint64_t GetPageOffsetByAddress(uintptr_t addr) const;
    inline uint64_t RemoveOffset(uintptr_t addr) const;
    uint64_t PageLookUp(uint32_t vpn0, uint32_t vpn1, uint32_t vpn2, uint32_t vpn3);
    inline uintptr_t CheckInTlb(int64_t id, uintptr_t vaddr);
    inline void PushToTlb(int64_t vpn0, uintptr_t vaddr, uintptr_t paddr);
    uint8_t *GetPhysAddrWithAllocation(uintptr_t vaddr);
    inline bool IsVirtAddrCanonical(uintptr_t vaddr) const;
    uintptr_t GetPointer(uint64_t page_id, uint64_t page_offset) const;
    void ValidateElfHeader(const GElf_Ehdr &ehdr) const;

    std::vector<std::pair<int64_t, uintptr_t>> tlb_;
    PhysMem *ram_ = nullptr;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_MMU_HPP