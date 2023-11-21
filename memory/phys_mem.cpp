#include <stdexcept>
#include <cstring>
#include "phys_mem.hpp"

namespace simulator::mem {
PhysMem::PhysMem(uint64_t total_size) : total_size_(total_size)
{
    memory_ = new uint8_t[total_size];
    allocated_pages_.resize(total_size / Page::SIZE, false);
}

PhysMem::~PhysMem()
{
    delete[] memory_;
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

bool PhysMem::Read(uintptr_t paddr, size_t size, void *value)
{
    std::memcpy(value, memory_ + paddr, size);
    return true;
}

bool PhysMem::Write(uintptr_t paddr, size_t size, void *value)
{
    std::memcpy(memory_ + paddr, value, size);
    return true;
}

uint64_t PhysMem::GetEmptyPageNumber() const
{
    for (uint64_t i = 0; i < allocated_pages_.size(); ++i) {
        if (!allocated_pages_[i]) {
            return i + 1;
        }
    }
    throw std::runtime_error("No empty pages");
}

bool PhysMem::InitPage(uint64_t pageNum)
{
    if (pageNum >= total_size_ / Page::SIZE) {
        throw std::runtime_error("Invalid address");
        return false;
    }
    allocated_pages_[pageNum - 1] = true;
    return true;
}

/**
 * Checks that sequence of bytes is located at one Phys Page
 */
bool PhysMem::AtOnePage(uint64_t offset, uint64_t length) const
{
    return Page::SIZE - offset >= length;
}

uint8_t *PhysMem::GetMemPointer() const
{
    return memory_;
}

}  // namespace simulator::mem