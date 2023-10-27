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
    uint64_t max_num_of_pages = total_size_ / Page::SIZE;
    if (page_id >= max_num_of_pages || offset > Page::SIZE) {
        throw std::runtime_error("Page fault");
    }
    if constexpr (AllocPageIfNeeded) {
        while (page_id >= pages_.size()) {
            size_t new_size = std::min(max_num_of_pages, pages_.size() * 2);
            pages_.resize(new_size);
        }
    }
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

/**
 * Checks that sequence of bytes is located at one Phys Page
 */
bool PhysMem::AtOnePage(uint64_t offset, uint64_t length) const
{
    return Page::SIZE - offset >= length;
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