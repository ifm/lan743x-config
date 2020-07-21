/** @file 021-testReadEeprom.hpp
 *
 *  @brief
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */


#include <catch2/catch.hpp>
#include "shared.hpp"

#include "lan7430conf/byte.hpp"

#include <cstdint>
#include <iostream>
#include <bitset>

TEMPLATE_TEST_CASE("test set individual bits",
                   "[Byte][setBits]",
                   Byte,
                   Byte16,
                   uint8_t,
                   uint16_t,
                   uint32_t,
                   uint64_t,
                   int8_t,
                   int16_t,
                   int32_t,
                   int64_t)
{
    TestType allZeros = 0b0;
    auto bitWidth = sizeof(allZeros) * 8;

    for (auto i = 0; i < bitWidth; ++i)
    {
        DYNAMIC_SECTION("set bit at pos: " << i)
        {
            setBit<TestType>(allZeros, i, 1);
            if (i == bitWidth - 1 && std::is_signed<TestType>())
            {
                REQUIRE(allZeros
                        == std::pow(2, i) * -1);  // as setting the MSB of signed types changes the
                                                  // sign, we need to do the same for the comparator
            }
            else
            {
                REQUIRE(allZeros == std::pow(2, i));
            }
        }
    }
}

TEMPLATE_TEST_CASE("test set all bits",
                   "[Byte][setBits]",
                   Byte,
                   Byte16,
                   uint8_t,
                   uint16_t,
                   uint32_t,
                   uint64_t,
                   int8_t,
                   int16_t,
                   int32_t,
                   int64_t)
{
    TestType allZeros = 0b0;
    TestType allOnes = ~TestType(0);  // flips all the bits to 1
    auto bitWidth = sizeof(allZeros) * 8;

    for (auto i = 0; i < bitWidth; ++i)
    {
        setBit<TestType>(allZeros, i, 1);
    }
    for (auto i = 0; i < bitWidth; ++i)
    {
        REQUIRE(getBit<TestType>(allZeros, i));
    }
    REQUIRE(allZeros == allOnes);
}

TEMPLATE_TEST_CASE("test set bitmask",
                   "[Byte][setBitmask]",
                   Byte,
                   Byte16,
                   uint8_t,
                   uint16_t,
                   uint32_t,
                   uint64_t,
                   int8_t,
                   int16_t,
                   int32_t,
                   int64_t)
{
    TestType allZeros = 0b0;

    SECTION("set bitmask [0:0]")
    {
        setBitmask<TestType>(allZeros, 0, 0, 0b1);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 0) == TestType(0b1));
        REQUIRE(allZeros == TestType(0b1));
    }
    SECTION("set bitmask [0:1]")
    {
        setBitmask<TestType>(allZeros, 0, 1, 0b11);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 1) == TestType(0b11));
        REQUIRE(allZeros == TestType(0b11));
    }
    SECTION("set bitmask [0:2]")
    {
        setBitmask<TestType>(allZeros, 0, 2, 0b111);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 2) == TestType(0b111));
        REQUIRE(allZeros == TestType(0b111));
    }
    SECTION("set bitmask [0:3]")
    {
        setBitmask<TestType>(allZeros, 0, 3, 0b1111);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 3) == TestType(0b1111));
        REQUIRE(allZeros == TestType(0b1111));
    }
    SECTION("set bitmask [0:4]")
    {
        setBitmask<TestType>(allZeros, 0, 4, 0b11111);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 4) == TestType(0b11111));
        REQUIRE(allZeros == TestType(0b11111));
    }
    SECTION("set bitmask [0:5]")
    {
        setBitmask<TestType>(allZeros, 0, 5, 0b111111);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 5) == TestType(0b111111));
        REQUIRE(allZeros == TestType(0b111111));
    }
    SECTION("set bitmask [0:6]")
    {
        setBitmask<TestType>(allZeros, 0, 6, 0b1111111);
        REQUIRE(getBitmask<TestType>(allZeros, 0, 6) == TestType(0b1111111));
        REQUIRE(allZeros == TestType(0b1111111));
    }
    SECTION("set bitmask [0:7]")
    {
        setBitmask<TestType>(allZeros, 0, 7, TestType(0b11111111));
        REQUIRE(getBitmask<TestType>(allZeros, 0, 7) == TestType(0b11111111));
        REQUIRE(allZeros == TestType(0b11111111));
    }
    SECTION("set bitmask [1:7]")
    {
        setBitmask<TestType>(allZeros, 1, 7, 0b1111111);
        REQUIRE(getBitmask<TestType>(allZeros, 1, 7) == TestType(0b1111111));
        REQUIRE(allZeros == TestType(0b11111110));
    }
    SECTION("set bitmask [2:7]")
    {
        setBitmask<TestType>(allZeros, 2, 7, 0b111111);
        REQUIRE(getBitmask<TestType>(allZeros, 2, 7) == TestType(0b111111));
        REQUIRE(allZeros == TestType(0b11111100));
    }
    SECTION("set bitmask [3:7]")
    {
        setBitmask<TestType>(allZeros, 3, 7, 0b11111);
        REQUIRE(getBitmask<TestType>(allZeros, 3, 7) == TestType(0b11111));
        REQUIRE(allZeros == TestType(0b11111000));
    }
    SECTION("set bitmask [4:7]")
    {
        setBitmask<TestType>(allZeros, 4, 7, 0b1111);
        REQUIRE(getBitmask<TestType>(allZeros, 4, 7) == TestType(0b1111));
        REQUIRE(allZeros == TestType(0b11110000));
    }
    SECTION("set bitmask [5:7]")
    {
        setBitmask<TestType>(allZeros, 5, 7, 0b111);
        REQUIRE(getBitmask<TestType>(allZeros, 5, 7) == TestType(0b111));
        REQUIRE(allZeros == TestType(0b11100000));
    }
    SECTION("set bitmask [6:7]")
    {
        setBitmask<TestType>(allZeros, 6, 7, 0b11);
        REQUIRE(getBitmask<TestType>(allZeros, 6, 7) == TestType(0b11));
        REQUIRE(allZeros == TestType(0b11000000));
    }
    SECTION("set bitmask [7:7]")
    {
        setBitmask<TestType>(allZeros, 7, 7, 0b1);
        REQUIRE(getBitmask<TestType>(allZeros, 7, 7) == 1);
        REQUIRE(allZeros == TestType(0b10000000));
    }

    SECTION("set bitmask random [4:7]")
    {
        setBitmask<TestType>(allZeros, 4, 7, 0b1010);
        REQUIRE(getBitmask<TestType>(allZeros, 4,7) == TestType(0b1010));
        REQUIRE(allZeros == TestType(0b10100000));
    }
    SECTION("set bitmask random [1:5]")
    {
        setBitmask<TestType>(allZeros, 1, 5, 0b1010);
        REQUIRE(getBitmask<TestType>(allZeros, 1,5) == TestType(0b1010));
        REQUIRE(allZeros == TestType(0b00010100));
    }
    SECTION("set bitmask random [3:6]")
    {
        setBitmask<TestType>(allZeros, 3, 5, 0b111);
        REQUIRE(getBitmask<TestType>(allZeros, 3, 5) == TestType(0b111));
        REQUIRE(allZeros == TestType(0b00111000));
    }
}

TEMPLATE_TEST_CASE("test set bitmask random",
                   "[Byte][setBitmask]",
                   uint16_t,
                   uint32_t,
                   uint64_t,
                   int16_t,
                   int32_t,
                   int64_t)
{
    // NOTE(MA): (u)int8_t etc... are not allowed for random generator
    TestType allZeros = 0b0;

    SECTION("set bitmask random") {
        uint32_t bitWidth = sizeof(allZeros) * 8;
        TestType rand = GENERATE(take(
            2500,
            random(std::numeric_limits<TestType>::min(), std::numeric_limits<TestType>::max())));
        //        TestType rand = GENERATE(take(100, random(, intmax_t(564))));
        setBitmask<TestType>(allZeros, 0, bitWidth - 1, rand);
        REQUIRE(getBitmask<TestType>(allZeros, 0, bitWidth - 1) == rand);
        REQUIRE(allZeros == rand);
    }

    SECTION("set bitmask random [n:m]")
    {
        uint32_t start = GENERATE_COPY(take(20, random<uint32_t>(0, (sizeof(TestType) * 8) - 1)));
        uint32_t end = GENERATE_COPY(take(20,
                                          filter([start](uint32_t end) { return end >= start; },
                                                 random<uint32_t>(0, (sizeof(TestType) * 8) - 1))));

        uint64_t mask = ((uint64_t(1) << (end - start + 1)) - 1) << start;
        uint64_t maskShifted = mask >> start;
        uint64_t min = 0;
        uint64_t max = mask;
        uint64_t rand = GENERATE_COPY(take(5, random(min, max)));
        uint64_t randOred = rand & mask;
        uint64_t randShifted = randOred >> start;

        setBitmask<TestType>(allZeros, start, end, static_cast<TestType>(randShifted));

        REQUIRE(getBitmask<TestType>(allZeros, start, end) == static_cast<TestType>(randShifted));
        REQUIRE(allZeros == static_cast<TestType>(randOred));
    }
}
