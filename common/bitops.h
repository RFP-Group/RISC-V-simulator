#ifndef COMMON_BITOPS_H
#define COMMON_BITOPS_H

#include <cassert>
#include <climits>
#include <cstdint>
#include <type_traits>

namespace simulator {

template <typename Type, uint8_t Sign>
inline Type GetSignedExtension(Type val)
{
    static_assert(sizeof(Type) * 8 > Sign);
    Type mask = (static_cast<Type>(1) << Sign) - 1;
    Type sign_bit = static_cast<Type>(1) << (Sign - 1);
    return ((val & mask) ^ sign_bit) - sign_bit;
}

template <typename Type, Type Mask>
inline constexpr Type ApplyMask(Type raw)
{
    return (raw & Mask);
}

template <typename Type, uint8_t Shift>
inline constexpr Type ApplyLeftShift(Type raw_inst)
{
    static_assert(Shift < sizeof(Type) * CHAR_BIT);
    return raw_inst << Shift;
}

template <typename Type, uint8_t Shift>
inline constexpr Type ApplyRightShift(Type raw_inst)
{
    static_assert(Shift < sizeof(Type) * CHAR_BIT);
    return raw_inst >> Shift;
}

template <uint32_t Mask, int8_t From, int8_t To>
inline uint32_t ApplyMaskAndShift(uint32_t raw_inst)
{
    static_assert(From >= 0 && To >= 0);
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

template <uint8_t low, uint8_t high, typename T = uint32_t>
inline constexpr T GetPartialBitsShifted(const T val)
{
    static_assert(low <= high);
    constexpr T mask = ((static_cast<T>(1) << (high - low + 1)) - 1) << low;
    return ApplyRightShift<T, low>(val & mask);
}

template <uint8_t low, uint8_t high, typename T = uint32_t>
inline constexpr T MakePartialBits(const T val)
{
    static_assert(low <= high);
    constexpr const T mask = ((static_cast<T>(1) << (high - low + 1)) - 1);
    return ((val & mask) << low);
}

template <typename Type>
inline std::make_signed_t<Type> GetSignedForm(Type val)
{
    return std::make_signed_t<Type>(val);
}

template <typename Type>
inline std::make_unsigned_t<Type> GetUnsignedForm(Type val)
{
    return std::make_unsigned_t<Type>(val);
}

}  // namespace simulator

#endif  // COMMON_BITOPS_H