#ifndef CONFIGS_UTILS_HPP
#define CONFIGS_UTILS_HPP

#include <cstdint>

template <uint64_t number>
constexpr uint64_t GetPowerOfTwo()
{
    if constexpr (number == 1) {
        return 0;
    } else {
        static_assert(number % 2 == 0, "The number must be a power of 2");
        return GetPowerOfTwo<number / 2>() + 1;
    }
}

template <uint32_t power>
constexpr uint64_t TenPow()
{
    static_assert(power >= 0, "Power must be non-negative");
    if constexpr (power == 0) {
        return 1;
    } else {
        return TenPow<power - 1>() * 10;
    }
}

template <uint64_t shift, int32_t float_count, uint64_t V>
constexpr uint64_t GetShiftedMemoryValue()
{
    if constexpr (float_count > 0) {
        return static_cast<uint64_t>(V * (1ULL << shift) / TenPow<float_count - 1>());
    } else {
        return static_cast<uint64_t>(V * (1ULL << shift) / TenPow<0>());
    }
}
template <uint64_t shift, int32_t float_count, uint64_t V, char C, char... Cs>
constexpr uint64_t GetShiftedMemoryValue()
{
    static_assert(C == '.' || (C >= '0' && C <= '9'), "Using not a number");
    if constexpr (C == '.') {
        return GetShiftedMemoryValue<shift, 1, V, Cs...>();
    } else if constexpr (float_count > 0) {
        return GetShiftedMemoryValue<shift, float_count + 1, V * 10 + C - '0', Cs...>();
    } else {
        return GetShiftedMemoryValue<shift, float_count, V * 10 + C - '0', Cs...>();
    }
}

#endif  // CONFIGS_UTILS_HPP