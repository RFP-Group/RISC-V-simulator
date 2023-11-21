#ifndef CONFIGS_MEM_HPP
#define CONFIGS_MEM_HPP

#include "utils.hpp"

namespace simulator {
constexpr uint64_t SHIFT_KB = 10ULL;
constexpr uint64_t SHIFT_MB = 20ULL;
constexpr uint64_t SHIFT_GB = 30ULL;

template <char... Cs>
constexpr uint64_t operator"" _KB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_KB, 0, 0, Cs...>();
}
template <char... Cs>
constexpr uint64_t operator"" _MB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_MB, 0, 0, Cs...>();
}
template <char... Cs>
constexpr uint64_t operator"" _GB()
{
    static_assert(sizeof...(Cs) <= 20);
    return GetShiftedMemoryValue<SHIFT_GB, 0, 0, Cs...>();
}

template <typename T>
constexpr uintptr_t ToUintPtr(T *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

constexpr uintptr_t ToUintPtr(std::nullptr_t)
{
    return ToUintPtr(static_cast<void *>(nullptr));
}

template <typename T>
constexpr T *ToNativePtr(uintptr_t ptr)
{
    return reinterpret_cast<T *>(ptr);
}

inline void *ToVoidPtr(uintptr_t ptr)
{
    return reinterpret_cast<void *>(ptr);
}

}  // namespace simulator

#endif  // CONFIGS_MEM_HPP