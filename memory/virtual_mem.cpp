#include <iostream>
#include "virtual_mem.hpp"

namespace simulator::mem {
VirtualMem::VirtualMem()
{
    ram_ = PhysMem::CreatePhysMem(16_GB);
    assert(ram_ != nullptr);
}

VirtualMem::~VirtualMem()
{
    assert(ram_ != nullptr);
    PhysMem::Destroy(ram_);
}

/* static */
VirtualMem *VirtualMem::CreateVirtualMem()
{
    return new VirtualMem();
}

/* static */
bool VirtualMem::Destroy(VirtualMem *virtual_mem)
{
    assert(virtual_mem != nullptr);
    delete virtual_mem;
    return true;
}

void VirtualMem::LoadByte(uintptr_t addr, uint8_t chr)
{
    uint8_t *phys_addr;
    // TODO(Mirageinvo): maybe create derived page_fault exception class?
    // handling possible page fault exception
    try {
        phys_addr = GetPhysAddress<true>(addr);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
    }
    assert(phys_addr != nullptr);
    *phys_addr = chr;
}

uint8_t VirtualMem::ReadByte(uintptr_t addr) const
{
    uint8_t *phys_addr;
    // TODO(Mirageinvo): maybe create derived page_fault exception class?
    // handling possible page fault exception
    try {
        phys_addr = GetPhysAddress<false>(addr);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
    }
    assert(phys_addr != nullptr);
    return *phys_addr;
}

bool VirtualMem::LoadByteSequence(uintptr_t addr, uint8_t *chrs, uint64_t length)
{
    assert(chrs != nullptr);
    for (uint64_t i = 0; i < length; ++i) {
        LoadByte(addr + i, *(chrs + i));
    }
    IncrementOccupiedValue(addr, length);
    return true;
}

void VirtualMem::IncrementOccupiedValue(uintptr_t addr, uint64_t length)
{
    while (length != 0) {
        Page *page = GetPageByAddress(addr);
        [[maybe_unused]] uintptr_t page_start = addr & Page::ID_MASK;
        assert(page_start <= addr);
        uintptr_t unoccupied_mem = page->GetFreeMemoryPtr();
        if (addr < unoccupied_mem) {
            length -= std::min(length, unoccupied_mem - addr);
            addr = unoccupied_mem;
        } else if (addr > unoccupied_mem) {
            // TODO(mirageinvo): maybe it is better to fail here?
            page->SetFreeMemoryPtr(addr);
            continue;
        }
        uint64_t free_mem_size = page->GetFreeMemorySize();
        if (length > free_mem_size) {
            // registering that this page is full
            page->SetFreeMemoryPtr(unoccupied_mem + free_mem_size);
            assert(page->GetFreeMemorySize() == 0);
            assert(page->GetOccupiedMemorySize() == Page::SIZE);
            length -= free_mem_size;
            addr += free_mem_size;
        } else {
            page->SetFreeMemoryPtr(unoccupied_mem + length);
            length -= length;
            addr += length;
        }
    }
}

uintptr_t VirtualMem::GetNextContinuousBlock(uint64_t length)
{
    // TODO(Mirageinvo): introduce less memory-consuming algorithm
    std::pair<uint64_t, uint64_t> ind_and_offset = ram_->NextAfterLastOccupiedByte();
    return GetPointer(ind_and_offset.first, ind_and_offset.second);
}

uintptr_t VirtualMem::GetPointer(uint64_t page_id, uint64_t page_offset) const
{
    uintptr_t addr = 0;
    addr += (page_id << Page::OFFSET_BIT_LENGTH) + page_offset;
    return addr;
}

// TODO(Mirageinvo): refactor returned value after discussion
std::vector<uint8_t> VirtualMem::ReadByteSequence(uintptr_t addr, uint64_t length)
{
    std::vector<uint8_t> arr(length);
    for (uint64_t i = 0; i < length; ++i) {
        arr[i] = ReadByte(addr + i);
    }
    return arr;
}

template <bool AllocPageIfNeeded>
uint8_t *VirtualMem::GetPhysAddress(uintptr_t addr) const
{
    uint64_t page_id = GetPageIdByAddress(addr);
    uint64_t offset = GetPageOffsetByAddress(addr);
    uint8_t *phys_addr = ram_->GetAddr<AllocPageIfNeeded>(page_id, offset);
    assert(phys_addr != nullptr);
    return phys_addr;
}

Page *VirtualMem::GetPageByAddress(uintptr_t addr)
{
    uint64_t page_id = GetPageIdByAddress(addr);
    return ram_->GetPage(page_id);
}

uint64_t VirtualMem::GetPageIdByAddress(uintptr_t addr) const
{
    return (addr & Page::ID_MASK) >> Page::OFFSET_BIT_LENGTH;
}

uint64_t VirtualMem::GetPageOffsetByAddress(uintptr_t addr) const
{
    return addr & Page::OFFSET_MASK;
}

void VirtualMem::LoadTwoBytesFast(uintptr_t addr, uint16_t value)
{
    uint8_t upper_value = (value & ((TwoPow<8>() - 1) << 8)) >> 8;
    uint8_t lower_value = value & (TwoPow<8>() - 1);
    LoadByte(addr, upper_value);
    LoadByte(addr + 1, lower_value);
}

uint16_t VirtualMem::ReadTwoBytesFast(uintptr_t addr) const
{
    uint16_t value = 0;
    value |= ReadByte(addr);
    value = value << 8;
    value |= ReadByte(addr + 1);
    return value;
}

void VirtualMem::LoadFourBytesFast(uintptr_t addr, uint32_t value)
{
    uint16_t upper_value = (value & ((TwoPow<16>() - 1) << 16)) >> 16;
    uint16_t lower_value = value & (TwoPow<16>() - 1);
    LoadTwoBytesFast(addr, upper_value);
    LoadTwoBytesFast(addr + 2, lower_value);
}

uint32_t VirtualMem::ReadFourBytesFast(uintptr_t addr) const
{
    uint32_t value = 0;
    for (size_t i = 0; i < 3; ++i) {
        value |= ReadByte(addr + i);
        value = value << 8;
    }
    value |= ReadByte(addr + 3);
    return value;
}

void VirtualMem::LoadEightBytesFast(uintptr_t addr, uint64_t value)
{
    uint32_t upper_value = (value & ((TwoPow<32>() - 1) << 32)) >> 32;
    uint32_t lower_value = value & (TwoPow<32>() - 1);
    LoadFourBytesFast(addr, upper_value);
    LoadFourBytesFast(addr + 4, lower_value);
}

uint64_t VirtualMem::ReadEightBytesFast(uintptr_t addr) const
{
    uint64_t value = 0;
    value |= ReadFourBytesFast(addr);
    value = value << 4;
    value |= ReadFourBytesFast(addr + 4);
    return value;
}

uintptr_t VirtualMem::LoadElfFile(const std::string &name)
{
    int fd;
    if ((fd = open(name.c_str(), O_RDONLY, 0777)) < 0)
        throw std::runtime_error("cannot open file failed " + name);
    if (elf_version(EV_CURRENT) == EV_NONE)
        throw std::runtime_error("ELF library initialization failed: " + std::string(elf_errmsg(-1)));

    Elf *e;
    if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        throw std::runtime_error("elf_begin() failed " + std::string(elf_errmsg(-1)));

    GElf_Ehdr ehdr;
    if (gelf_getehdr(e, &ehdr) == NULL)
        throw std::runtime_error("gelf_getehdr() failed " + std::string(elf_errmsg(-1)));

    validateElfHeader(ehdr);

    size_t n;
    if (elf_getphdrnum(e, &n) != 0)
        throw std::runtime_error("elf_getphdrnum() failed: " + std::string(elf_errmsg(-1)));

    GElf_Phdr phdr;
    for (int i = 0; i < n; ++i) {
        if (gelf_getphdr(e, i, &phdr) != &phdr)
            throw std::runtime_error("gelf_getphdr() failed: " + std::string(elf_errmsg(-1)));
        if (phdr.p_type != PT_LOAD)
            continue;

        // TODO(Mirageinvo): remove temporary buffer
        std::vector<uint8_t> buff(phdr.p_vaddr + phdr.p_filesz);
        pread(fd, buff.data() + phdr.p_vaddr, phdr.p_filesz, phdr.p_offset);
        uintptr_t addr_to_load = GetNextContinuousBlock(buff.size());
        LoadByteSequence(addr_to_load, buff.data(), buff.size());
    }

    elf_end(e);
    close(fd);

    return ehdr.e_entry;
}

void VirtualMem::validateElfHeader(const GElf_Ehdr &ehdr) const
{
#define checkHeaderField(offset, value) \
    if (ehdr.offset != value)           \
        throw std::runtime_error(#offset " != " #value " in elf header we don't support it");

    checkHeaderField(e_ident[EI_MAG0], ELFMAG0);
    checkHeaderField(e_ident[EI_MAG1], ELFMAG1);
    checkHeaderField(e_ident[EI_MAG2], ELFMAG2);
    checkHeaderField(e_ident[EI_MAG3], ELFMAG3);
    checkHeaderField(e_ident[EI_CLASS], ELFCLASS64);  // 64 bit only supported
    checkHeaderField(e_ident[EI_DATA], ELFDATA2LSB);  // little endian only supported
    checkHeaderField(e_machine, EM_RISCV);            // RISCV architecture only supported
}

}  // namespace simulator::mem