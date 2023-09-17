#ifndef MEMORY_INCLUDES_PHYS_MEM
#define MEMORY_INCLUDES_PHYS_MEM

#include <vector>
#include "macros.hpp"
#include "common/mem.hpp"

namespace simulator::mem {
//TODO(Mirageinvo): remove initial_page_number and introduce algorithm for page number increasing
//                  because it looks weird to alloc all 16_GB at once
constexpr uint32_t initial_page_number = 4;

class PhysMem final {
public:
    NO_DEFAULT_CTOR(PhysMem);
    NO_COPY_SEMANTIC(PhysMem);
    NO_MOVE_SEMANTIC(PhysMem);

    [[nodiscard]] static PhysMem* CreatePhysMem(uint64_t total_size);
    static bool Destroy(PhysMem* phys_mem);

    std::vector<uint32_t>* GetMemory();
private:
    PhysMem(uint64_t size);
    ~PhysMem() = default;

    uint32_t pages_used_ = 0;
    uint64_t total_size_;
    //TODO(Mirageinvo): change uint32_t to Page class. There is no need in using templates
    std::vector<uint32_t> pages_;
};
} // namespace simulator::mem

#endif //MEMORY_INCLUDES_PHYS_MEM