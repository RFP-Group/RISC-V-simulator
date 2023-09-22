#include "phys_mem.hpp"
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

namespace simulator::mem {
PhysMem::PhysMem(uint64_t total_size): total_size_(total_size) {
    //TODO(Mirageinvo): change uint32_t's size to Page class's size
    //TODO(Mirageinvo): check that page_size_ is a power of 2
    pages_.resize(initial_page_number * page_size);
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
std::vector<char>* PhysMem::GetMemory() {
    return &pages_;
}

void PhysMem::LoadElfFile(const std::string &name) {
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
    entry_point_ = ehdr.e_entry; // entry point address

    size_t n;
    if (elf_getphdrnum(e, &n) != 0)
        throw std::runtime_error("elf_getphdrnum() failed: " + std::string(elf_errmsg(-1)));

    GElf_Phdr phdr;
    for (int i = 0; i < n; ++i) {
        if (gelf_getphdr(e, i, &phdr) != &phdr)
            throw std::runtime_error("gelf_getphdr() failed: " + std::string(elf_errmsg(-1)));
        if (phdr.p_type != PT_LOAD)
            continue;

        if (phdr.p_vaddr + phdr.p_filesz > pages_.size())
            pages_.resize((phdr.p_vaddr + phdr.p_filesz));
        pread(fd, pages_.data() + phdr.p_vaddr, phdr.p_filesz, phdr.p_offset);
    }

    elf_end(e);
    close(fd);
}

void PhysMem::validateElfHeader(const GElf_Ehdr &ehdr) const {
    #define checkHeaderField(offset, value) \
        if (ehdr.offset != value) \
            throw std::runtime_error(#offset " != " #value " in elf header we don't support it");

    checkHeaderField(e_ident[EI_MAG0], ELFMAG0);
    checkHeaderField(e_ident[EI_MAG1], ELFMAG1);
    checkHeaderField(e_ident[EI_MAG2], ELFMAG2);
    checkHeaderField(e_ident[EI_MAG3], ELFMAG3);
    checkHeaderField(e_ident[EI_CLASS], ELFCLASS64); // 64 bit only supported
    checkHeaderField(e_ident[EI_DATA], ELFDATA2LSB); // little endian only supported
    checkHeaderField(e_machine, EM_RISCV);           // RISCV architecture only supported
}


} // namespace simulator::mem