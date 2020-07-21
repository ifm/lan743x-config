/** @file byte.hpp
 *
 *  @brief contains functions etc... for manipulating bits
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */

#ifndef BYTE_HPP
#define BYTE_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <utility>


using Byte = uint8_t;
using Byte16 = uint16_t;

template<typename T, typename S>
constexpr T getBit(S source, uint32_t pos)
{
    assert(pos <= (sizeof(S) * 8) - 1);
    assert(pos >= 0);
    pos = std::max<uint32_t>(0, pos);                    // limit to allowed range
    pos = std::min<uint32_t>((sizeof(S) * 8) - 1, pos);  // limit to allowed range

    return static_cast<T>((source >> pos) & 1);
}
template<typename T, typename S>
constexpr T getBitmask(S source, uint32_t start, uint32_t end)
{
    assert(start >= 0);
    assert(end <= (sizeof(S) * 8) - 1);
    assert(start <= end);
    // start must be smaller or equal to end
    if (start >= end)
    {
        std::swap(start, end);
    }
    start = std::max<uint32_t>(0, start);                // limit to allowed range
    end = std::min<uint32_t>((sizeof(S) * 8) - 1, end);  // limit to allowed range


    // NOTE(MA): shift operation has undefined behavior
    //           if you shift by values greater than or equal to the bit width
    uint64_t mask = ((uint64_t(1) << (end - start + 1)) - 1) << start;
    if (end - start + 1 >= sizeof(S) * 8)
    {
        mask = ~uint64_t(0);
    }

    // NOTE(MA): right shift on uint64_t fills with 0, which is what we expect here
    mask = mask >> start;
    return static_cast<T>((source >> start) & mask);
}

template<typename T>
constexpr void setBit(T& target, uint32_t pos, T value)
{
    assert(pos <= (sizeof(T) * 8) - 1);
    assert(pos >= 0);
    pos = std::max<uint32_t>(0, pos);                    // limit to allowed range
    pos = std::min<uint32_t>((sizeof(T) * 8) - 1, pos);  // limit to allowed range

    target |= value << pos;
}
template<typename T>
constexpr void setBitmask(T& target, uint32_t start, uint32_t end, T value)
{
    assert(start >= 0);
    assert(end <= (sizeof(T) * 8) - 1);
    assert(start <= end);
    // start must be smaller or equal to end
    if (start >= end)
    {
        std::swap(start, end);
    }
    start = std::max<uint32_t>(0, start);                // limit to allowed range
    end = std::min<uint32_t>((sizeof(T) * 8) - 1, end);  // limit to allowed range

    // NOTE(MA): shift operation has undefined behavior
    //           if you shift by values greater than or equal to the bit width
    uint64_t mask = ((uint64_t(1) << (end - start + 1)) - 1) << start;
    if (end - start + 1 >= sizeof(T) * 8)
    {
        mask = ~uint64_t(0);
    }

    target |= (value << start) & static_cast<T>(mask);
}

#endif  // BYTE_HPP
