/** @file 020-testWriteEeprom.hpp
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

#include <filesystem>

#include <fstream>
#include <iomanip>
#include <iostream>

TEST_CASE("validMac", "[WriteEEPROM]")
{
    Mac mac = stringToMac("12:12:12:12:12:12");
    EEPROM_CONFIG config;
    config.magic = EEPROM_MAGIC::EEPROM_MAC;
    config.mac = mac;
    config.clockPowerManagement = true;
    config.auxCurrent = AUX_CURRENT::AC_375;
    config.pmeSupport = PME_SUPPORT::D3_HOT;

    EEPROM eeprom = createEEPROM(config);

    REQUIRE(EEPROM_MAGIC(eeprom.magic) == EEPROM_MAGIC::EEPROM_MAC);
    REQUIRE(eeprom.mac == mac);
    REQUIRE(std::equal(std::begin(base_eeprom) + eeprom_user_defined_size,
                       std::end(base_eeprom),
                       eeprom.base_config.begin()));
}


TEST_CASE("invalidMac", "[WriteEEPROM]")
{
    EEPROM_CONFIG config;
    config.magic = EEPROM_MAGIC::EEPROM_MAC;
    config.mac = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };
    config.clockPowerManagement = true;
    config.auxCurrent = AUX_CURRENT::AC_375;
    config.pmeSupport = PME_SUPPORT::D3_HOT;

    EEPROM eeprom = createEEPROM(config);
    REQUIRE_THROWS_WITH(validateEEPROM(eeprom), ifm::error_type(ifm::MAC_ADDRESS_INVALID).what());

    REQUIRE(EEPROM_MAGIC(eeprom.magic) == EEPROM_MAGIC::EEPROM_MAC);
    REQUIRE(std::equal(std::begin(base_eeprom) + eeprom_user_defined_size,
                       std::end(base_eeprom),
                       eeprom.base_config.begin()));
}

TEST_CASE("emptyFileName", "[WriteEEPROM]")
{
    Mac mac = stringToMac("12:12:12:12:12:12");
    REQUIRE_THROWS_WITH(writeEEPROM("", {}), ifm::error_type(ifm::FILE_CANT_WRITE).what());
}
TEST_CASE("nonExistingPath", "[WriteEEPROM]")
{
    Mac mac = stringToMac("12:12:12:12:12:12");
    REQUIRE_THROWS_WITH(writeEEPROM("/tmp/this/is/some/random/path/to/nowhere.bin", {}),
                        ifm::error_type(ifm::FOLDER_PATH_DOESNT_EXIST).what());
}

TEST_CASE("relativePath", "[WriteEEPROM]")
{
    Mac mac = stringToMac("12:12:12:12:12:12");
    REQUIRE_NOTHROW(writeEEPROM("./relativePathTest.bin", {}));
}
TEST_CASE("relativeNonExistingPath", "[WriteEEPROM]")
{
    Mac mac = stringToMac("12:12:12:12:12:12");
    REQUIRE_THROWS_WITH(writeEEPROM("./path/to/nowhere.bin", {}),
                        ifm::error_type(ifm::FOLDER_PATH_DOESNT_EXIST).what());
}

TEST_CASE("compareAgainstFiles", "[WriteEEPROM]")
{
    std::vector<std::pair<std::string, std::string>> macStrings{
        { "00-02-01-23-10-55", "files/00-02-01-23-10-55-pad.bin" },
        { "00-02-01-23-10-56", "files/00-02-01-23-10-56-pad.bin" },
        { "00-02-01-23-10-57", "files/00-02-01-23-10-57-pad.bin" },
        { "00-02-01-23-10-58", "files/00-02-01-23-10-58-pad.bin" }
    };
    for (const auto& [macString, eepromFilePath] : macStrings)
    {
        std::string tempFilePath
            = std::filesystem::temp_directory_path().append(macString + ".bin").string();
        // std::string tempFilePath = "./" + macString + ".bin";

        EEPROM_CONFIG config;
        config.magic = EEPROM_MAGIC::EEPROM_MAC;
        config.mac = stringToMac(macString);
        config.clockPowerManagement = false;
        config.ltrMechanismSupport = false;
        config.obffSupport = OBFF_SUPPORT::NOT_SUPPORTED;
        config.pciPML12Support = true;
        config.pciPML11Support = false;
        config.aspmL12Support = true;
        config.aspmL11Support = false;
        config.l1PMSubstatesSupported = false;
        config.auxCurrent = AUX_CURRENT::AC_0;
        config.pmeSupport = PME_SUPPORT::NONE;

        REQUIRE_NOTHROW(writeEEPROM(tempFilePath, config));

        // compare the contents of the "files"
        std::ifstream file_orig(eepromFilePath, std::ifstream::binary | std::ifstream::ate);
        std::ifstream file_new(tempFilePath, std::ifstream::binary | std::ifstream::ate);

        REQUIRE(!file_orig.fail());
        REQUIRE(!file_new.fail());
        REQUIRE(file_orig.tellg() == file_new.tellg());
        // seek back to beginning and use std::equal to compare contents
        file_orig.seekg(0, std::ifstream::beg);
        file_new.seekg(0, std::ifstream::beg);
        REQUIRE(std::equal(std::istreambuf_iterator<char>(file_orig.rdbuf()),
                           std::istreambuf_iterator<char>(),
                           std::istreambuf_iterator<char>(file_new.rdbuf())));
    }
}

TEST_CASE("testWriteDefaultFiles", "[WriteEEPROM]")
{
    for (const auto& [eepromFilePath, eepromConfig] : gs_testFilesVector)
    {
        std::string tempFilePath = std::filesystem::temp_directory_path()
                                       .append(macToString(eepromConfig.mac) + ".bin")
                                       .string();
        // std::string tempFilePath = "./" + macToString(eepromConfig.mac) + ".bin";

        REQUIRE_NOTHROW(writeEEPROM(tempFilePath, eepromConfig));

        // compare the contents of the "files"
        std::ifstream file_orig(std::filesystem::absolute(eepromFilePath).string(),
                                std::ifstream::binary | std::ifstream::ate);
        std::ifstream file_new(std::filesystem::absolute(tempFilePath).string(),
                               std::ifstream::binary | std::ifstream::ate);
        std::array<unsigned char, eeprom_user_defined_size> file_content_orig{};
        std::array<unsigned char, eeprom_user_defined_size> file_content_new{};

        REQUIRE(!file_orig.fail());
        REQUIRE(!file_new.fail());
        REQUIRE(file_orig.tellg() >= eeprom_user_defined_size);
        REQUIRE(file_new.tellg() >= eeprom_user_defined_size);

        // seek back to beginning and use std::equal to compare contents
        file_orig.seekg(0, std::ifstream::beg);
        file_new.seekg(0, std::ifstream::beg);

        file_orig.read((char*) &file_content_orig, eeprom_user_defined_size);
        file_new.read((char*) &file_content_new, eeprom_user_defined_size);
#if 0
        std::cout << eepromFilePath << std::endl;
        for (const auto& byte : file_content_orig)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) byte;
        }
        std::cout << std::endl;
        for (const auto& byte : file_content_new)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) byte;
        }
        std::cout << std::endl;
#endif
        REQUIRE(std::equal(
            file_content_orig.begin(), file_content_orig.end(), file_content_new.begin()));
    }
}
