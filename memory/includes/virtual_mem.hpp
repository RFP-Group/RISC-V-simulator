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
    bool LoadByteSequence(uintptr_t addr, uint8_t *chrs, uint64_t length);
    std::vector<uint8_t> ReadByteSequence(uintptr_t addr, uint64_t length);
    uint8_t ReadByte(uintptr_t addr) const;
    uint16_t ReadTwoBytesFast(uintptr_t addr) const;
    uint32_t ReadFourBytesFast(uintptr_t addr) const;
    void LoadElfFile(const std::string &name);
    [[nodiscard]] uintptr_t GetNextContinuousBlock(uint64_t length);

private:
    VirtualMem();
    ~VirtualMem();
    uint64_t GetPageIdByAddress(uintptr_t addr) const;
    uint64_t GetPageOffsetByAddress(uintptr_t addr) const;
    Page *GetPageByAddress(uintptr_t addr);
    void LoadByte(uintptr_t addr, uint8_t chr);
    template <bool AllocPageIfNeeded>
    uint8_t *GetPhysAddress(uintptr_t addr) const;
    void IncrementOccupiedValue(uintptr_t addr, uint64_t length);
    uintptr_t GetPointer(uint64_t page_id, uint64_t page_offset) const;
    void validateElfHeader(const GElf_Ehdr &ehdr) const;

    PhysMem *ram_ = nullptr;
    uintptr_t entry_point_;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_VIRTUAL_MEM