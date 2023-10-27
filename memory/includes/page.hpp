#ifndef MEMORY_INCLUDES_PAGE
#define MEMORY_INCLUDES_PAGE

#include <cstdint>
#include "common/mem.hpp"
#include "macros.hpp"

namespace simulator::mem {
class Page final {
public:
    constexpr static const uint64_t SIZE = 4_KB;  // must be a power of 2
    constexpr static const uint64_t OFFSET_MASK = SIZE - 1;
    constexpr static const uint64_t OFFSET_BIT_LENGTH = GetPowerOfTwo<SIZE>();
    constexpr static const uint64_t ID_MASK = ~OFFSET_MASK;

    Page();
    Page(Page &&);
    ~Page();
    NO_COPY_SEMANTIC(Page)

    bool IsEmpty() const;
    uint64_t GetFreeMemorySize() const;
    uint64_t GetOccupiedMemorySize() const;
    uintptr_t GetMemoryStartPtr() const;
    uintptr_t GetFreeMemoryPtr() const;
    void SetFreeMemoryPtr(uintptr_t addr);
    void IncreaseOccupiedMemorySize(uint64_t size);
    void DecreaseOccupiedMemorySize(uint64_t size);
    uintptr_t GetAddrByOffset(uint64_t offset) const;

private:
    uintptr_t unoccupied_mem_;
    uintptr_t mem_;
};
}  // namespace simulator::mem

#endif  // MEMORY_INCLUDES_PAGE