#include "page.hpp"
#include "mmu.hpp"

namespace simulator::mem {
Page::Page()
{
    mem_ = ToUintPtr(new char[SIZE]);
    unoccupied_mem_ = mem_;
}

Page::Page(Page &&another) : unoccupied_mem_(another.unoccupied_mem_), mem_(std::move(another.mem_))
{
    another.mem_ = 0;
}

Page::~Page()
{
    delete[] ToNativePtr<char>(mem_);
}

bool Page::IsEmpty() const
{
    return unoccupied_mem_ == mem_;
}

uint64_t Page::GetOccupiedMemorySize() const
{
    return unoccupied_mem_ - mem_;
}

uint64_t Page::GetFreeMemorySize() const
{
    return SIZE - GetOccupiedMemorySize();
}

uintptr_t Page::GetMemoryStartPtr() const
{
    return mem_;
}

uintptr_t Page::GetFreeMemoryPtr() const
{
    return unoccupied_mem_;
}

void Page::SetFreeMemoryPtr(uintptr_t addr)
{
    unoccupied_mem_ = addr;
}

void Page::IncreaseOccupiedMemorySize(uint64_t size)
{
    assert(size <= GetFreeMemorySize());
    unoccupied_mem_ += size;
}

void Page::DecreaseOccupiedMemorySize(uint64_t size)
{
    assert(size <= GetOccupiedMemorySize());
    unoccupied_mem_ -= size;
}

uintptr_t Page::GetAddrByOffset(uint64_t offset) const
{
    assert(offset <= SIZE);
    return mem_ + offset;
}

}  // namespace simulator::mem