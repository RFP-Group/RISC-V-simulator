#ifndef CONFIGS_MEM_HPP
#define CONFIGS_MEM_HPP

#include "utils.hpp"

namespace simulator {
constexpr uint64_t SHIFT_KB = 10ULL;
constexpr uint64_t SHIFT_MB = 20ULL;
constexpr uint64_t SHIFT_GB = 30ULL;

template<char... Cs>
constexpr uint64_t operator"" _KB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_KB, 0, 0, Cs...>();
}
template<char... Cs>
constexpr uint64_t operator"" _MB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_MB, 0, 0, Cs...>();
}
template<char... Cs>
constexpr uint64_t operator"" _GB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_GB, 0, 0, Cs...>();
}

constexpr uint32_t page_size = 4_KB;
} // namespace simulator

#endif //CONFIGS_MEM_HPP