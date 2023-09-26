#ifndef CONFIGS_UTILS_HPP
#define CONFIGS_UTILS_HPP

#include <cstdint>

template <uint32_t power>
constexpr uint64_t tenpow()
{
    static_assert(power >= 0, "Power must be non-negative");
    if constexpr (power == 0) {
        return 1;
    } else {
        return tenpow<power - 1>() * 10;
    }
}

template <uint64_t shift, int32_t float_count, uint64_t V>
constexpr uint64_t GetShiftedMemoryValue()
{
    if constexpr (float_count > 0) {
        return static_cast<uint64_t>(V * (1ULL << shift) / tenpow<float_count - 1>());
    } else {
        return static_cast<uint64_t>(V * (1ULL << shift) / tenpow<0>());
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