/** @file 010-testMac.hpp
 *
 *  @brief
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */
#include <catch2/catch.hpp>

#include "lan7430conf/lan7430conf.hpp"
#include "lan7430conf/errors.hpp"

TEST_CASE("valid MAC", "[MAC]")
{
    Mac mac;
    REQUIRE_NOTHROW(mac = stringToMac("02:42:00:cf:36:c1"));
}

TEST_CASE("invalid MAC", "[MAC]")
{
    std::vector<std::string> macStrings{
        "XX:XX:XX:XX:XX:XX", "20:f1", "01", "1:", "20:f1�", "0", "02:36:c1", "02:00:cf", "021",
        "XX;XX*:XX::XXX;",   "20:",   ":"
    };
    for (const auto& macString : macStrings)
    {
        Mac mac;
        REQUIRE_THROWS_WITH(mac = stringToMac(macString),
                            ifm::error_type(ifm::MAC_ADDRESS_INVALID).what());
    }
}

TEST_CASE("empty MAC", "[MAC]")
{
   Mac mac;
   REQUIRE_THROWS_WITH(mac = stringToMac(""), ifm::error_type(ifm::MAC_ADDRESS_EMPTY).what());
}

TEST_CASE("MAC to string", "[MAC]")
{
    Mac mac{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    std::string macString = macToString(mac);
    REQUIRE_THAT(macString, Catch::Equals("01-02-03-04-05-06"));

    Mac mac2{ 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    std::string macString2 = macToString(mac2);
    REQUIRE_THAT(macString2, Catch::Equals("AA-BB-CC-DD-EE-FF"));
}
