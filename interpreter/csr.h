#ifndef INTERPRETER_CSR_H
#define INTERPRETER_CSR_H

#include "interpreter/instruction.h"
#include <cstdint>
#include <unordered_map>
#include <stdexcept>
#include <iomanip>

namespace simulator {

using Register = uint64_t;
constexpr std::size_t CSR_COUNT = 4096;

class CSR_file final {
private:
    enum Status_bit_masks : uint64_t {
        MASK_SIE = 1ULL << 1,
        MASK_MIE = 1ULL << 3,
        MASK_SPIE = 1ULL << 5,
        MASK_UBE = 1ULL << 6,
        MASK_MPIE = 1ULL << 7,
        MASK_SPP = 1ULL << 8,
        MASK_VS = 0b11ULL << 9,
        MASK_MPP = 0b11ULL << 11,
        MASK_FS = 0b11ULL << 13,
        MASK_XS = 0b11ULL << 15,
        MASK_MPRV = 1ULL << 17,
        MASK_SUM = 1ULL << 18,
        MASK_MXR = 1ULL << 19,
        MASK_TVM = 1ULL << 20,
        MASK_TW = 1ULL << 21,
        MASK_TSR = 1ULL << 22,
        MASK_UXL = 0b11ULL << 32,
        MASK_SXL = 0b11ULL << 34,
        MASK_SBE = 1ULL << 36,
        MASK_MBE = 1ULL << 37,
        MASK_SD = 1ULL << 63,
        MASK_SSTATUS =
            MASK_SIE | MASK_SPIE | MASK_UBE | MASK_SPP | MASK_FS | MASK_XS | MASK_SUM | MASK_MXR | MASK_UXL | MASK_SD
    };

    // MIP / SIP field mask
    enum MIP_SIP_bit_masks : uint64_t {
        MASK_SSIP = 1ULL << 1,
        MASK_MSIP = 1ULL << 3,
        MASK_STIP = 1ULL << 5,
        MASK_MTIP = 1ULL << 7,
        MASK_SEIP = 1ULL << 9,
        MASK_MEIP = 1ULL << 11
    };

    // std::array<Register, CSR_COUNT> csrs_ {};

    std::unordered_map<uint16_t, uint64_t> csrs_ = {
        {MHARTID, 0},    {MSTATUS, 0},  {MEDELEG, 0}, {MIDELEG, 0},  {MIE, 0},   {MTVEC, 0},
        {MCOUNTEREN, 0}, {MSCRATCH, 0}, {MEPC, 0},    {MCAUSE, 0},   {MTVAL, 0}, {MIP, 0},

        {SSTATUS, 0},    {SIE, 0},      {STVEC, 0},   {SSCRATCH, 0}, {SEPC, 0},  {SCAUSE, 0},
        {STVAL, 0},      {SIP, 0},      {SATP, 0}};

public:
    enum CSR_n : uint16_t {
        // Machine-level CSRs.

        MHARTID = 0xf14,
        MSTATUS = 0x300,
        MEDELEG = 0x302,
        MIDELEG = 0x303,
        MIE = 0x304,
        MTVEC = 0x305,
        MCOUNTEREN = 0x306,
        MSCRATCH = 0x340,
        MEPC = 0x341,
        MCAUSE = 0x342,
        MTVAL = 0x343,
        MIP = 0x344,

        // Supervisor-level CSRs.
        SSTATUS = 0x100,
        SIE = 0x104,
        STVEC = 0x105,
        SSCRATCH = 0x140,
        SEPC = 0x141,
        SCAUSE = 0x142,
        STVAL = 0x143,
        SIP = 0x144,
        SATP = 0x180
    };

    [[nodiscard]] inline Register read(uint16_t addr)
    {
        if (csrs_.find(addr) == csrs_.end()) {
            std::stringstream sstream;
            sstream << std::showbase << std::hex << addr;
            std::string hex_addr = sstream.str();
            std::runtime_error("Unknown csr register with addr: " + hex_addr);
        }

        switch (addr) {
            case SIE:
                return csrs_[MIE] & csrs_[MIDELEG];
            case SIP:
                return csrs_[MIP] & csrs_[MIDELEG];
            case SSTATUS:
                return csrs_[MSTATUS] & MASK_SSTATUS;
            default:
                return csrs_[addr];
        }
    }
    void write(uint16_t addr, Register value)
    {
        if (csrs_.find(addr) == csrs_.end()) {
            std::stringstream sstream;
            sstream << std::showbase << std::hex << addr;
            std::string hex_addr = sstream.str();
            std::runtime_error("Unknown csr register with addr: " + hex_addr);
        }

        switch (addr) {
            case SIE:
                csrs_[MIE] = (csrs_[MIE] & !csrs_[MIDELEG]) | (value & csrs_[MIDELEG]);
                break;
            case SIP:
                csrs_[MIP] = (csrs_[MIE] & !csrs_[MIDELEG]) | (value & csrs_[MIDELEG]);
                break;
            case SSTATUS:
                csrs_[MSTATUS] = (csrs_[MSTATUS] & !MASK_SSTATUS) | (value & MASK_SSTATUS);
                break;
            default:
                csrs_[addr] = value;
                break;
        }
    }
};

}  // namespace simulator

#endif  // INTERPRETER_GPR_H