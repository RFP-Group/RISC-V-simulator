#ifndef MEMORY_INCLUDES_PHYS_MEM
#define MEMORY_INCLUDES_PHYS_MEM

#include <vector>
#include <string>
#include "page.hpp"

namespace simulator::mem {

class PhysMem final {
public:
    NO_DEFAULT_CTOR(PhysMem)
    NO_COPY_SEMANTIC(PhysMem)
    NO_MOVE_SEMANTIC(PhysMem)

    [[nodiscard]] static PhysMem *CreatePhysMem(uint64_t total_size);
    static bool Destroy(PhysMem *phys_mem);

    bool Read(uintptr_t paddr, size_t size, void *value);
    bool Write(uintptr_t paddr, size_t size, void *value);
    uint64_t GetEmptyPageNumber() const;
    bool InitPage(uintptr_t paddr);
    bool AtOnePage(uint64_t offset, uint64_t length) const;
    uint8_t *GetMemPointer() const;

private:
    PhysMem(uint64_t size);
    ~PhysMem();

    uint64_t total_size_;
    uint8_t *memory_ = nullptr;
    std::vector<bool> allocated_pages_;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_PHYS_MEM