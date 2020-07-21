/** @file lan7430conf.hpp
 *
 *  @brief contains all the eeprom related enums / data structure and functions
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */

#include "lan7430conf/lan7430conf.hpp"

#include "lan7430conf/errors.hpp"

#include <spdlog/spdlog.h>

#include <boost/endian/conversion.hpp>
#include <boost/filesystem.hpp>

#include <cstring>
#include <fstream>
#include <regex>


Mac stringToMac(const std::string& macString) noexcept(false)
{
    if (macString.empty())
    {
        throw ifm::error_type(ifm::MAC_ADDRESS_EMPTY);
    }
    std::regex rx(
        R"(^(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])(?:([0-9A-Fa-f]{2})[:-])([0-9A-Fa-f]{2})$)",
        std::regex::icase);
    std::smatch match;

    if (!std::regex_match(macString, match, rx))
    {
        throw ifm::error_type(ifm::MAC_ADDRESS_INVALID);
    }

    Mac mac;
    for (size_t i = 1; i < match.size(); i++)
    {
        mac[i - 1] = static_cast<Byte>(std::stoul(match[i].str(), nullptr, 16));
    }

    return mac;
}

std::string macToString(const Mac& mac) { return fmt::format("{:02X}", fmt::join(mac, "-")); }

LAN7430_CONFIG_LIB_EXPORT bool validateMAC(const Mac& macArray)
{
    // NOTE(MA): assume macs with all 00 or all FF are invalid
    bool valid = false;
    for (const auto& byte : macArray)
    {
        valid |= (byte > 0x00 && byte < 0xff);
    }
    return valid;
}

void validateEEPROM(const EEPROM& eeprom) noexcept(false)
{
    std::vector<EEPROM_MAGIC> magic{ EEPROM_MAGIC::EEPROM,
                                     EEPROM_MAGIC::EEPROM_MAC,
                                     EEPROM_MAGIC::EEPROM_OTP1,
                                     EEPROM_MAGIC::EEPROM_OTP2 };
    if (std::none_of(magic.begin(), magic.end(), [&](EEPROM_MAGIC m) {
            return m == EEPROM_MAGIC(eeprom.magic);
        }))
    {
        throw ifm::error_type(ifm::EEPROM_INVALID_MAGIC);
    }

    if (!validateMAC(eeprom.mac))
    {
        throw ifm::error_type(ifm::MAC_ADDRESS_INVALID);
    }

    // TODO: add more validation steps e.g. values are in valid range
}

EEPROM_CONFIG eepromConfigToEEPROM(EEPROM eeprom) noexcept(false)
{
    EEPROM_CONFIG config{};

    config.magic = EEPROM_MAGIC(eeprom.magic);
    config.mac = eeprom.mac;

    config.clockPowerManagement = getBit<bool>(eeprom.deviceCapabilitiesEnable_1_2, 3);
    config.ltrMechanismSupport = getBit<bool>(eeprom.deviceCapabilitiesEnable_1_2, 7);

    config.aspmL1EntryControl = getBit<ASPM_L1_ENTRY_CONTROL>(eeprom.aspmConfigEnable, 4);

    config.subsystemVendorID = eeprom.subsystemVendorID;
    config.subsystemID = eeprom.subsystemID;

    config.auxCurrent = getBitmask<AUX_CURRENT>(eeprom.powerManagementCapabilities, 0, 2);
    config.pmeSupport = getBitmask<PME_SUPPORT>(eeprom.powerManagementCapabilities, 3, 7);

    config.clockPowerManagement = getBit<bool>(eeprom.deviceCapabilities_1, 7);

    config.ltrMechanismSupport = getBit<bool>(eeprom.deviceCapabilities_2, 1);
    config.obffSupport = getBitmask<OBFF_SUPPORT>(eeprom.powerManagementCapabilities, 2, 3);

    config.pciPML12Support = getBit<bool>(eeprom.l1PMSubstatesCapabilitesEnable, 2);
    config.pciPML11Support = getBit<bool>(eeprom.l1PMSubstatesCapabilitesEnable, 3);
    config.aspmL12Support = getBit<bool>(eeprom.l1PMSubstatesCapabilitesEnable, 4);
    config.aspmL11Support = getBit<bool>(eeprom.l1PMSubstatesCapabilitesEnable, 5);
    config.l1PMSubstatesSupported = getBit<bool>(eeprom.l1PMSubstatesCapabilitesEnable, 6);

    config.aspmL0EntranceLatency = getBitmask<ASPM_L0_ENTRANCE_LATENCY>(eeprom.aspmConfig, 0, 2);
    config.aspmL1EntranceLatency = getBitmask<ASPM_L1_ENTRANCE_LATENCY>(eeprom.aspmConfig, 4, 6);
    config.aspmL1EntryControl = getBit<ASPM_L1_ENTRY_CONTROL>(eeprom.aspmConfig, 7);

    config.macConfiguration = getBitmask<MAC_CONFIGURATION>(eeprom.macConfig1, 0, 1);
    config.duplexMode = getBit<DUPLEX_MODE>(eeprom.macConfig1, 2);
    config.automaticSpeedDetection = getBit<bool>(eeprom.macConfig1, 3);
    config.automaticDuplexDetection = getBit<bool>(eeprom.macConfig1, 4);
    config.automaticDuplexPolarity = getBit<AUTOMATIC_DUPLEX_POLARITY>(eeprom.macConfig1, 5);
    config.energyEfficientEthernet = getBit<bool>(eeprom.macConfig1, 7);

    config.energyEfficientEthernetTxClockStop = getBit<bool>(eeprom.macConfig1, 0);

    config.ledConfig[0].enable = getBit<bool>(eeprom.ledConfig1, 0);
    config.ledConfig[0].polarity = getBit<LED_POLARITY>(eeprom.ledConfig1, 4);

    config.ledConfig[1].enable = getBit<bool>(eeprom.ledConfig1, 1);
    config.ledConfig[1].polarity = getBit<LED_POLARITY>(eeprom.ledConfig1, 5);

    config.ledConfig[2].enable = getBit<bool>(eeprom.ledConfig1, 2);
    config.ledConfig[2].polarity = getBit<LED_POLARITY>(eeprom.ledConfig1, 6);

    config.ledConfig[3].enable = getBit<bool>(eeprom.ledConfig1, 3);
    config.ledConfig[3].polarity = getBit<LED_POLARITY>(eeprom.ledConfig1, 7);

    eeprom.ledConfig2 = boost::endian::big_to_native(eeprom.ledConfig2);
    config.ledConfig[0].control = getBitmask<LED_CONTROL>(eeprom.ledConfig2, 0, 3);
    config.ledConfig[1].control = getBitmask<LED_CONTROL>(eeprom.ledConfig2, 4, 7);
    config.ledConfig[2].control = getBitmask<LED_CONTROL>(eeprom.ledConfig2, 8, 11);
    config.ledConfig[3].control = getBitmask<LED_CONTROL>(eeprom.ledConfig2, 12, 15);

    eeprom.ledConfig3 = boost::endian::big_to_native(eeprom.ledConfig3);
    config.ledConfig[0].combineFeature = getBit<LED_COMBINE>(eeprom.ledConfig3, 0);
    config.ledConfig[1].combineFeature = getBit<LED_COMBINE>(eeprom.ledConfig3, 1);
    config.ledConfig[2].combineFeature = getBit<LED_COMBINE>(eeprom.ledConfig3, 2);
    config.ledConfig[3].combineFeature = getBit<LED_COMBINE>(eeprom.ledConfig3, 3);

    config.ledConfig[0].blinkPulseStretch = getBit<LED_BLINK_PULSE_STRETCH>(eeprom.ledConfig3, 5);
    config.ledConfig[1].blinkPulseStretch = getBit<LED_BLINK_PULSE_STRETCH>(eeprom.ledConfig3, 6);
    config.ledConfig[2].blinkPulseStretch = getBit<LED_BLINK_PULSE_STRETCH>(eeprom.ledConfig3, 7);
    config.ledConfig[3].blinkPulseStretch = getBit<LED_BLINK_PULSE_STRETCH>(eeprom.ledConfig3, 8);

    config.blinkPulseStretchRate = getBitmask<BLINK_PULSE_STRETCH_RATE>(eeprom.ledConfig3, 10, 11);
    config.ledPulsing = getBit<LED_PULSING>(eeprom.ledConfig3, 12);
    config.ledActivityOutput = getBit<LED_ACTIVITY_OUTPUT>(eeprom.ledConfig3, 14);

    return config;
}

EEPROM createEEPROM(const EEPROM_CONFIG& conf) noexcept(false)
{
    std::array<Byte, base_eeprom_len> eeprom_bin{};
    std::copy(std::begin(base_eeprom), std::end(base_eeprom), eeprom_bin.begin());

    EEPROM eeprom{};


    eeprom.magic = static_cast<Byte>(conf.magic);
    eeprom.mac = conf.mac;

    setBit<Byte>(eeprom.byte7, 2, conf.auxCurrent != AUX_CURRENT::AC_0);
    setBit<Byte>(eeprom.byte7, 3, conf.pmeSupport != PME_SUPPORT::NONE);

    setBit<Byte>(eeprom.deviceCapabilitiesEnable_1_2, 3, conf.clockPowerManagement);
    setBit<Byte>(eeprom.deviceCapabilitiesEnable_1_2, 7, conf.ltrMechanismSupport);

    setBit<Byte>(
        eeprom.l1PMSubstatesCapabilitesEnable, 0, conf.obffSupport != OBFF_SUPPORT::NOT_SUPPORTED);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilitesEnable, 2, conf.pciPML12Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilitesEnable, 3, conf.pciPML11Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilitesEnable, 4, conf.aspmL12Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilitesEnable, 5, conf.aspmL11Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilitesEnable, 6, conf.l1PMSubstatesSupported);

    setBit<Byte>(eeprom.byte7, 0, conf.subsystemVendorID != 0x0);
    setBit<Byte>(eeprom.byte7, 1, conf.subsystemID != 0x0);

    setBit<Byte>(eeprom.aspmConfigEnable,
                 2,
                 conf.aspmL0EntranceLatency != ASPM_L0_ENTRANCE_LATENCY::MICRO_SECONDS_1);
    setBit<Byte>(eeprom.aspmConfigEnable,
                 3,
                 conf.aspmL1EntranceLatency != ASPM_L1_ENTRANCE_LATENCY::MICRO_SECONDS_1);

    setBit<Byte>(eeprom.aspmConfigEnable, 4, static_cast<Byte>(conf.aspmL1EntryControl));

    eeprom.subsystemVendorID = conf.subsystemVendorID;
    eeprom.subsystemID = conf.subsystemID;

    setBitmask<Byte>(eeprom.powerManagementCapabilities, 0, 2, static_cast<Byte>(conf.auxCurrent));
    setBitmask<Byte>(eeprom.powerManagementCapabilities, 3, 7, static_cast<Byte>(conf.pmeSupport));

    setBit<Byte>(eeprom.deviceCapabilities_1, 7, conf.clockPowerManagement);

    setBit<Byte>(eeprom.deviceCapabilities_2, 1, conf.ltrMechanismSupport);
    setBitmask<Byte>(eeprom.deviceCapabilities_2, 2, 3, static_cast<Byte>(conf.obffSupport));

    setBit<Byte>(eeprom.l1PMSubstatesCapabilites, 0, conf.pciPML12Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilites, 1, conf.pciPML11Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilites, 2, conf.aspmL12Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilites, 3, conf.aspmL11Support);
    setBit<Byte>(eeprom.l1PMSubstatesCapabilites, 4, conf.l1PMSubstatesSupported);

    setBitmask<Byte>(eeprom.aspmConfig, 0, 2, static_cast<Byte>(conf.aspmL0EntranceLatency));
    setBitmask<Byte>(eeprom.aspmConfig, 4, 6, static_cast<Byte>(conf.aspmL1EntranceLatency));
    setBit<Byte>(eeprom.aspmConfig, 7, static_cast<Byte>(conf.aspmL1EntryControl));

    setBitmask<Byte>(eeprom.macConfig1, 0, 1, static_cast<Byte>(conf.macConfiguration));
    setBit<Byte>(eeprom.macConfig1, 2, static_cast<Byte>(conf.duplexMode));
    setBit<Byte>(eeprom.macConfig1, 3, conf.automaticSpeedDetection);
    setBit<Byte>(eeprom.macConfig1, 4, conf.automaticDuplexDetection);
    setBit<Byte>(eeprom.macConfig1, 5, static_cast<Byte>(conf.automaticDuplexPolarity));
    setBit<Byte>(eeprom.macConfig1, 7, conf.energyEfficientEthernet);

    setBit<Byte>(eeprom.macConfig2, 0, conf.energyEfficientEthernetTxClockStop);

    setBit<Byte>(eeprom.ledConfig1, 0, conf.ledConfig[0].enable);
    setBit<Byte>(eeprom.ledConfig1, 4, static_cast<Byte>(conf.ledConfig[0].polarity));

    setBit<Byte>(eeprom.ledConfig1, 1, conf.ledConfig[1].enable);
    setBit<Byte>(eeprom.ledConfig1, 5, static_cast<Byte>(conf.ledConfig[1].polarity));

    setBit<Byte>(eeprom.ledConfig1, 2, conf.ledConfig[2].enable);
    setBit<Byte>(eeprom.ledConfig1, 6, static_cast<Byte>(conf.ledConfig[2].polarity));

    setBit<Byte>(eeprom.ledConfig1, 3, conf.ledConfig[3].enable);
    setBit<Byte>(eeprom.ledConfig1, 7, static_cast<Byte>(conf.ledConfig[3].polarity));

    setBitmask<Byte16>(eeprom.ledConfig2, 0, 3, static_cast<Byte>(conf.ledConfig[0].control));
    setBitmask<Byte16>(eeprom.ledConfig2, 4, 7, static_cast<Byte>(conf.ledConfig[1].control));
    setBitmask<Byte16>(eeprom.ledConfig2, 8, 11, static_cast<Byte>(conf.ledConfig[2].control));
    setBitmask<Byte16>(eeprom.ledConfig2, 12, 15, static_cast<Byte>(conf.ledConfig[3].control));

    eeprom.ledConfig2 = boost::endian::native_to_big(eeprom.ledConfig2);

    setBit<Byte16>(eeprom.ledConfig3, 0, static_cast<Byte>(conf.ledConfig[0].combineFeature));
    setBit<Byte16>(eeprom.ledConfig3, 1, static_cast<Byte>(conf.ledConfig[1].combineFeature));
    setBit<Byte16>(eeprom.ledConfig3, 2, static_cast<Byte>(conf.ledConfig[2].combineFeature));
    setBit<Byte16>(eeprom.ledConfig3, 3, static_cast<Byte>(conf.ledConfig[3].combineFeature));

    setBit<Byte16>(eeprom.ledConfig3, 5, static_cast<Byte>(conf.ledConfig[0].blinkPulseStretch));
    setBit<Byte16>(eeprom.ledConfig3, 6, static_cast<Byte>(conf.ledConfig[1].blinkPulseStretch));
    setBit<Byte16>(eeprom.ledConfig3, 7, static_cast<Byte>(conf.ledConfig[2].blinkPulseStretch));
    setBit<Byte16>(eeprom.ledConfig3, 8, static_cast<Byte>(conf.ledConfig[3].blinkPulseStretch));

    setBitmask<Byte16>(eeprom.ledConfig3, 10, 11, static_cast<Byte>(conf.blinkPulseStretchRate));
    setBit<Byte16>(eeprom.ledConfig3, 12, static_cast<Byte16>(conf.ledPulsing));
    setBit<Byte16>(eeprom.ledConfig3, 14, static_cast<Byte16>(conf.ledActivityOutput));

    eeprom.ledConfig3 = boost::endian::native_to_big(eeprom.ledConfig3);

    std::copy(std::begin(base_eeprom) + eeprom_user_defined_size,
              std::end(base_eeprom),
              eeprom.base_config.begin());  // copy the remaining base eeprom to the array

    return eeprom;
}

void writeEEPROM(const std::string& filePath, const EEPROM_CONFIG& config) noexcept(false)
{
    EEPROM eeprom = createEEPROM(config);

    boost::filesystem::path path(filePath);
    if (path.has_parent_path() && !boost::filesystem::exists(path.remove_filename()))
    {
        throw ifm::error_type(ifm::FOLDER_PATH_DOESNT_EXIST);
    }

    std::ofstream out(filePath, std::ios::binary | std::ios::out);
    if (!out)
    {
        throw ifm::error_type(ifm::FILE_CANT_WRITE);
    }

    out.write(reinterpret_cast<char*>(&eeprom), sizeof(EEPROM));
    out.close();
    if (!out)
    {
        throw ifm::error_type(ifm::FILE_CANT_WRITE);
    }
}

EEPROM readEEPROM(const std::string& filePath) noexcept(false)
{
    if (filePath.empty() || !boost::filesystem::exists(filePath))
    {
        throw ifm::error_type(ifm::FILE_PATH_DOESNT_EXIST);
    }

    if (boost::filesystem::file_size(filePath) != 255
        && boost::filesystem::file_size(filePath)
               != 512)  // NOTE(MA): the gui (MPLAB Connect) seems to create only files of size
                        // 255... even if specified to use 512
    {
        throw ifm::error_type(ifm::EEPROM_WRONG_SIZE);
    }

    std::ifstream in(filePath, std::ios::binary | std::ios::in);

    if (!in)
    {
        throw ifm::error_type(ifm::FILE_CANT_READ);
    }

    EEPROM eeprom;
    in.read((char*) &eeprom, sizeof(EEPROM));
    in.close();

    validateEEPROM(eeprom);

    return eeprom;
}
