#include <stdexcept>
#include "phys_mem.hpp"

namespace simulator::mem {
PhysMem::PhysMem(uint64_t total_size) : total_size_(total_size)
{
    pages_.resize(initial_page_number);
}

/* static */
PhysMem *PhysMem::CreatePhysMem(uint64_t total_size)
{
    assert(total_size != 0);
    return new PhysMem(total_size);
}

/* static */
bool PhysMem::Destroy(PhysMem *phys_mem)
{
    assert(phys_mem != nullptr);
    delete phys_mem;
    return true;
}

template <bool AllocPageIfNeeded>
uint8_t *PhysMem::GetAddr(uint64_t page_id, uint64_t offset)
{
    if constexpr (AllocPageIfNeeded) {
        if (page_id == pages_.size() && (pages_.size() + 4) * Page::SIZE <= total_size_) {
            // TODO(Mirageinvo): introduce more advanced algorithm for page allocation
            pages_.resize(pages_.size() + 4);
        } else if (page_id == pages_.size() && (pages_.size() + 1) * Page::SIZE <= total_size_) {
            // In this case we have few memory left
            pages_.resize(pages_.size() + 1);
        }
    }
#ifndef NDEBUG
    if (page_id >= pages_.size() || offset > Page::SIZE) {
        throw std::runtime_error("Page fault");
    }
#endif
    return ToNativePtr<uint8_t>(pages_[page_id].GetAddrByOffset(offset));
}

Page *PhysMem::GetPage(uint64_t page_id)
{
#ifndef NDEBUG
    if (page_id >= pages_.size()) {
        throw std::runtime_error("Page fault");
    }
#endif
    return &pages_[page_id];
}

std::pair<uint64_t, uint64_t> PhysMem::NextAfterLastOccupiedByte() const
{
    // assuming that free pages are in the back of vector
    for (int i = pages_.size() - 1; i >= 0; --i) {
        if (i == 0 && pages_[i].IsEmpty()) {
            return {0, 0};  // all pages are empty
        } else if (!pages_[i].IsEmpty()) {
            // even if there are no empty pages
            // they will be allocated in GetAddr method
            return {i, pages_[i].GetOccupiedMemorySize() + 1};
        }
    }
    // should be unreachable
    assert(false);
}

template uint8_t *PhysMem::GetAddr<true>(uint64_t page_id, uint64_t offset);
template uint8_t *PhysMem::GetAddr<false>(uint64_t page_id, uint64_t offset);

}  // namespace simulator::mem