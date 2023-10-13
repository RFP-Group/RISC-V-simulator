#ifndef MEMORY_INCLUDES_VIRTUAL_MEM
#define MEMORY_INCLUDES_VIRTUAL_MEM

#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include "phys_mem.hpp"

namespace simulator::mem {
class VirtualMem final {
public:
    NO_COPY_SEMANTIC(VirtualMem);
    NO_MOVE_SEMANTIC(VirtualMem);

    [[nodiscard]] static VirtualMem *CreateVirtualMem();
    static bool Destroy(VirtualMem *virtual_mem);
    bool StoreByteSequence(uintptr_t addr, uint8_t *chrs, uint64_t length);
    std::vector<uint8_t> LoadByteSequence(uintptr_t addr, uint64_t length);
    void StoreByte(uintptr_t addr, uint8_t chr);
    uint8_t LoadByte(uintptr_t addr) const;
    void StoreTwoBytesFast(uintptr_t addr, uint16_t value);
    uint16_t LoadTwoBytesFast(uintptr_t addr) const;
    void StoreFourBytesFast(uintptr_t addr, uint32_t value);
    uint32_t LoadFourBytesFast(uintptr_t addr) const;
    void StoreEightBytesFast(uintptr_t addr, uint64_t value);
    uint64_t LoadEightBytesFast(uintptr_t addr) const;
    [[nodiscard]] uintptr_t StoreElfFile(const std::string &name);
    [[nodiscard]] uintptr_t GetNextContinuousBlock(uint64_t length);

private:
    VirtualMem();
    ~VirtualMem();
    uint64_t GetPageIdByAddress(uintptr_t addr) const;
    uint64_t GetPageOffsetByAddress(uintptr_t addr) const;
    Page *GetPageByAddress(uintptr_t addr);
    template <bool AllocPageIfNeeded>
    uint8_t *GetPhysAddress(uintptr_t addr) const;
    void IncrementOccupiedValue(uintptr_t addr, uint64_t length);
    uintptr_t GetPointer(uint64_t page_id, uint64_t page_offset) const;
    void validateElfHeader(const GElf_Ehdr &ehdr) const;

    PhysMem *ram_ = nullptr;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_VIRTUAL_MEM