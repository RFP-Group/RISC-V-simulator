#ifndef MEMORY_INCLUDES_PHYS_MEM
#define MEMORY_INCLUDES_PHYS_MEM

#include <vector>
#include <string>
#include <libelf.h>
#include <gelf.h>
#include "macros.hpp"
#include "common/mem.hpp"

namespace simulator::mem {
// TODO(Mirageinvo): remove initial_page_number and introduce algorithm for page number increasing
//                   because it looks weird to alloc all 16_GB at once
constexpr uint32_t initial_page_number = 4;

class PhysMem final {
public:
    NO_DEFAULT_CTOR(PhysMem);
    NO_COPY_SEMANTIC(PhysMem);
    NO_MOVE_SEMANTIC(PhysMem);

    [[nodiscard]] static PhysMem *CreatePhysMem(uint64_t total_size);
    static bool Destroy(PhysMem *phys_mem);

    std::vector<char> *GetMemory();
    void LoadElfFile(const std::string &name);

private:
    PhysMem(uint64_t size);
    ~PhysMem() = default;

    uint32_t pages_used_ = 0;
    uint64_t total_size_;
    uint64_t entry_point_;

    // TODO(Mirageinvo): change uint32_t to Page class. There is no need in using templates
    std::vector<char> pages_;

    void validateElfHeader(const GElf_Ehdr &ehdr) const;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_PHYS_MEM