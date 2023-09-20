#ifndef COMMON_BITOPS_H
#define COMMON_BITOPS_H

#include <cassert>
#include <climits>

namespace simulator {

template <typename Type, Type Mask>
inline Type ApplyMask(Type raw)
{
    return (raw & Mask);
}

template <typename Type, uint8_t Shift>
inline Type ApplyLeftShift(Type raw_inst)
{
    static_assert(Shift < sizeof(Type) * CHAR_BIT);
    return raw_inst << Shift;
}

template <typename Type, uint8_t Shift>
inline Type ApplyRightShift(Type raw_inst)
{
    static_assert(Shift < sizeof(Type) * CHAR_BIT);
    return raw_inst >> Shift;
}

template <uint32_t Mask, int8_t From, int8_t To>
inline uint32_t ApplyMaskAndShift(uint32_t raw_inst)
{
    static_assert(From > 0 && To > 0);
    static_assert(From < sizeof(uint32_t) * CHAR_BIT && To < sizeof(uint32_t) * CHAR_BIT);
    raw_inst = ApplyMask<uint32_t, Mask>(raw_inst);
    constexpr int8_t diff = From - To;
    if constexpr (diff > 0) {
        raw_inst = ApplyRightShift<uint32_t, diff>(raw_inst);
    }
    if constexpr (diff < 0) {
        raw_inst = ApplyLeftShift<uint32_t, -diff>(raw_inst);
    }
    return raw_inst;
}

};  // namespace simulator

#endif  // COMMON_BITOPS_H