#include <iostream>
#include "bitops.h"
#include "mmu.hpp"

namespace simulator::mem {
MMU::MMU()
{
    ram_ = PhysMem::CreatePhysMem(1_GB);
    tlb_.resize(TLB_SIZE, {-1, 0});
    assert(ram_ != nullptr);
}

MMU::~MMU()
{
    assert(ram_ != nullptr);
    PhysMem::Destroy(ram_);
}

/* static */
MMU *MMU::CreateMMU()
{
    return new MMU();
}

/* static */
bool MMU::Destroy(MMU *mmu)
{
    assert(mmu != nullptr);
    delete mmu;
    return true;
}

void MMU::StoreByte(uintptr_t addr, uint8_t chr)
{
    uint8_t *phys_addr;
    // TODO(Mirageinvo): maybe create derived page_fault exception class?
    // handling possible page fault exception
    try {
        phys_addr = GetPhysAddrWithAllocation(addr);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        PhysMem::Destroy(ram_);
        std::abort();
    }
    assert(phys_addr != nullptr);
    *phys_addr = chr;
}

uint8_t MMU::LoadByte(uintptr_t addr)
{
    uint8_t *phys_addr;
    // TODO(Mirageinvo): maybe create derived page_fault exception class?
    // handling possible page fault exception
    try {
        phys_addr = GetPhysAddrWithAllocation(addr);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what();
        PhysMem::Destroy(ram_);
        std::abort();
    }
    assert(phys_addr != nullptr);
    return *phys_addr;
}

bool MMU::StoreByteSequence(uintptr_t addr, uint8_t *chrs, uint64_t length)
{
    assert(chrs != nullptr);
    for (uint64_t i = 0; i < length; ++i) {
        StoreByte(addr + i, *(chrs + i));
    }
    return true;
}

std::vector<uint8_t> MMU::LoadByteSequence(uintptr_t addr, uint64_t length)
{
    std::vector<uint8_t> arr(length);
    for (uint64_t i = 0; i < length; ++i) {
        arr[i] = LoadByte(addr + i);
    }
    return arr;
}

uint64_t MMU::PageLookUp(uint32_t vpn0, uint32_t vpn1, uint32_t vpn2, uint32_t vpn3)
{
    auto mem_ptr = ram_->GetMemPointer();
    uintptr_t paddr0;
    uintptr_t paddr1;
    uintptr_t paddr2;
    uintptr_t paddr3;
    // Assume that transation table root is on the first page
    paddr3 = PTE_SIZE * vpn3;
    uint64_t *paddr3_ptr = reinterpret_cast<uint64_t *>(mem_ptr + paddr3);
    if (*paddr3_ptr == 0) {
        uint64_t pageNum = ram_->GetEmptyPageNumber();
        ram_->InitPage(pageNum);
        *paddr3_ptr = pageNum;
    }

    paddr2 = ((*paddr3_ptr) - 1) * Page::SIZE;
    paddr2 += PTE_SIZE * vpn2;
    uint64_t *paddr2_ptr = reinterpret_cast<uint64_t *>(mem_ptr + paddr2);
    if (*paddr2_ptr == 0) {
        uint64_t pageNum = ram_->GetEmptyPageNumber();
        ram_->InitPage(pageNum);
        *paddr2_ptr = pageNum;
    }

    paddr1 = ((*paddr2_ptr) - 1) * Page::SIZE;
    paddr1 += PTE_SIZE * vpn1;
    uint64_t *paddr1_ptr = reinterpret_cast<uint64_t *>(mem_ptr + paddr1);
    if (*paddr1_ptr == 0) {
        uint64_t pageNum = ram_->GetEmptyPageNumber();
        ram_->InitPage(pageNum);
        *paddr1_ptr = pageNum;
    }

    paddr0 = ((*paddr1_ptr - 1) * Page::SIZE);
    paddr0 += PTE_SIZE * vpn0;
    uint64_t *paddr0_ptr = reinterpret_cast<uint64_t *>(mem_ptr + paddr0);
    if (*paddr0_ptr == 0) {
        uint64_t pageNum = ram_->GetEmptyPageNumber();
        ram_->InitPage(pageNum);
        *paddr0_ptr = pageNum;
    }

    return ((*paddr0_ptr) - 1) * Page::SIZE;
}

uintptr_t MMU::CheckInTlb(int64_t id, uintptr_t vaddr)
{
    id = id % TLB_SIZE;
    auto pair = tlb_[id];
    [[likely]] if (pair.first != -1 && pair.second == vaddr)
    {
        return pair.first;
    }
    return 0;
}

void MMU::PushToTlb(int64_t id, uintptr_t vaddr, uintptr_t paddr)
{
    id = id % TLB_SIZE;
    tlb_[id] = std::make_pair(paddr, vaddr);
}

uint8_t *MMU::GetPhysAddrWithAllocation(uintptr_t vaddr)
{
    // TRANSLATION MODE IS SV48
    [[unlikely]] if (!IsVirtAddrCanonical(vaddr))
    {
        throw std::runtime_error("Noncanonical address");
    }

    uint32_t vpn0 = GetPartialBitsShifted<12, 20>(vaddr);
    uint64_t tlb_ind = vpn0;
    uint64_t vaddr_without_offset = RemoveOffset(vaddr);
    uintptr_t paddr = CheckInTlb(tlb_ind, vaddr_without_offset);
    [[unlikely]] if (paddr == 0)
    {
        uint32_t vpn1 = GetPartialBitsShifted<21, 29>(vaddr);
        uint32_t vpn2 = GetPartialBitsShifted<30, 38>(vaddr);
        uint32_t vpn3 = GetPartialBitsShifted<39, 47>(vaddr);
        paddr = PageLookUp(vpn0, vpn1, vpn2, vpn3);
        PushToTlb(tlb_ind, vaddr_without_offset, paddr);
    }

    paddr += GetPageOffsetByAddress(vaddr);
    return ToNativePtr<uint8_t>(ToUintPtr<uint8_t>(ram_->GetMemPointer()) + paddr);
}

bool MMU::IsVirtAddrCanonical(uintptr_t vaddr) const
{
    static constexpr uint64_t ADDRESS_UPPER_BITS_MASK_SV48 = 0xFFFF800000000000;
    uint64_t val = vaddr & ADDRESS_UPPER_BITS_MASK_SV48;
    if (val == 0 || val == ADDRESS_UPPER_BITS_MASK_SV48) {
        return true;
    }
    return false;
}

uint64_t MMU::GetPageOffsetByAddress(uintptr_t addr) const
{
    return addr & Page::OFFSET_MASK;
}

uint64_t MMU::RemoveOffset(uintptr_t addr) const
{
    return addr & (~Page::OFFSET_MASK);
}

void MMU::StoreTwoBytesFast(uintptr_t addr, uint16_t value)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 2));
    *reinterpret_cast<uint16_t *>(GetPhysAddrWithAllocation(addr)) = value;
}

uint16_t MMU::LoadTwoBytesFast(uintptr_t addr)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 2));
    return *reinterpret_cast<uint16_t *>(GetPhysAddrWithAllocation(addr));
}

void MMU::StoreFourBytesFast(uintptr_t addr, uint32_t value)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 4));
    *reinterpret_cast<uint32_t *>(GetPhysAddrWithAllocation(addr)) = value;
}

uint32_t MMU::LoadFourBytesFast(uintptr_t addr)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 4));
    return *reinterpret_cast<uint32_t *>(GetPhysAddrWithAllocation(addr));
}

void MMU::StoreEightBytesFast(uintptr_t addr, uint64_t value)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 8));
    *reinterpret_cast<uint64_t *>(GetPhysAddrWithAllocation(addr)) = value;
}

uint64_t MMU::LoadEightBytesFast(uintptr_t addr)
{
    assert(ram_->AtOnePage(GetPageOffsetByAddress(addr), 8));
    return *reinterpret_cast<uint64_t *>(GetPhysAddrWithAllocation(addr));
}

uintptr_t MMU::StoreElfFile(const std::string &name)
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

    ValidateElfHeader(ehdr);

    size_t n;
    if (elf_getphdrnum(e, &n) != 0)
        throw std::runtime_error("elf_getphdrnum() failed: " + std::string(elf_errmsg(-1)));

    GElf_Phdr phdr;
    std::vector<uint8_t> buff;
    for (size_t i = 0; i < n; ++i) {
        if (gelf_getphdr(e, i, &phdr) != &phdr)
            throw std::runtime_error("gelf_getphdr() failed: " + std::string(elf_errmsg(-1)));
        if (phdr.p_type != PT_LOAD)
            continue;

        // TODO(Mirageinvo): remove temporary buffer
        if (buff.capacity() < phdr.p_filesz) {
            buff.resize(phdr.p_filesz);
        }
        if (pread(fd, buff.data(), phdr.p_filesz, phdr.p_offset) == -1) {
            std::abort();
        }
        StoreByteSequence(phdr.p_vaddr, buff.data(), phdr.p_filesz);
    }

    elf_end(e);
    close(fd);

    return ehdr.e_entry;
}

void MMU::ValidateElfHeader(const GElf_Ehdr &ehdr) const
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