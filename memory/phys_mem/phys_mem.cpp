#include "phys_mem.hpp"

namespace simulator::mem {
PhysMem::PhysMem(uint64_t total_size): total_size_(total_size) {
    //TODO(Mirageinvo): change uint32_t's size to Page class's size
    //TODO(Mirageinvo): check that page_size_ is a power of 2
    pages_.resize(initial_page_number * page_size / sizeof(uint32_t));
}

/* static */
PhysMem* PhysMem::CreatePhysMem(uint64_t total_size) {
    assert(total_size != 0);
    return new PhysMem(total_size);
}

/* static */
bool PhysMem::Destroy(PhysMem* phys_mem) {
    //TODO(Mirageinvo): pages will be freed here
    assert(phys_mem != nullptr);
    delete phys_mem;
    return true;
}

//TODO(Mirageinvo): refactor this terrible method after getting more info about elf loading
//                  and after creating Page class
std::vector<uint32_t>* PhysMem::GetMemory() {
    return &pages_;
}

} // namespace simulator::mem