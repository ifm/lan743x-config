/** @file 021-testReadEeprom.hpp
 *
 *  @brief
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */

#include "lan7430conf/errors.hpp"
#include "lan7430conf/lan7430conf.hpp"
#include "shared.hpp"

#include <catch2/catch.hpp>

#include <fstream>
#include <iostream>

TEST_CASE("testReadDefaultFiles", "[ReadEeprom]")
{
    for (const auto& [eepromFilePath, eepromConfig] : gs_testFilesVector)
    {
        EEPROM eeprom;
        REQUIRE_NOTHROW(eeprom = readEEPROM(eepromFilePath));

        REQUIRE_NOTHROW(validateEEPROM(eeprom));
        REQUIRE(static_cast<EEPROM_MAGIC>(eeprom.magic) == eepromConfig.magic);
        REQUIRE(eeprom.mac == eepromConfig.mac);
    }
}
