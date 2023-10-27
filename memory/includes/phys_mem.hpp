#ifndef MEMORY_INCLUDES_PHYS_MEM
#define MEMORY_INCLUDES_PHYS_MEM

#include <vector>
#include <string>
#include "page.hpp"

namespace simulator::mem {
// TODO(Mirageinvo): remove initial_page_number and introduce algorithm for page number increasing
//                   because it looks weird to alloc all 16_GB at once
constexpr const uint32_t initial_page_number = 4;

class PhysMem final {
public:
    NO_DEFAULT_CTOR(PhysMem)
    NO_COPY_SEMANTIC(PhysMem)
    NO_MOVE_SEMANTIC(PhysMem)

    [[nodiscard]] static PhysMem *CreatePhysMem(uint64_t total_size);
    static bool Destroy(PhysMem *phys_mem);

    template <bool AllocPageIfNeeded>
    uint8_t *GetAddr(uint64_t page_id, uint64_t offset);
    Page *GetPage(uint64_t page_id);
    bool AtOnePage(uint64_t offset, uint64_t length) const;
    std::pair<uint64_t, uint64_t> NextAfterLastOccupiedByte() const;

private:
    PhysMem(uint64_t size);
    ~PhysMem() = default;

    uint64_t total_size_;
    std::vector<Page> pages_;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_PHYS_MEM