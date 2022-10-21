/** @file lan7430conf.hpp
 *
 *  @brief contains all the eeprom related enums / data structure and functions
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */

#ifndef LAN7430CONF_HPP
#define LAN7430CONF_HPP

#include "lan7430conf/byte.hpp"
#include "lan7430conf/lan7430-config-lib_export.h"

#include <array>
#include <string>

enum class EEPROM_MAGIC
{
    EEPROM = 0xA5,
    EEPROM_MAC = 0xAA,
    EEPROM_OTP1 = 0xF3,
    EEPROM_OTP2 = 0xF7,
};

using Mac = std::array<Byte, 6>;

enum class AUX_CURRENT
{
    AC_0 = 0x0,    //= 0b000,
    AC_55 = 0x1,   //= 0b001,
    AC_100 = 0x2,  //= 0b010,
    AC_160 = 0x3,  //= 0b011,
    AC_220 = 0x4,  //= 0b100,
    AC_270 = 0x5,  //= 0b101,
    AC_320 = 0x6,  //= 0b110,
    AC_375 = 0x7,  //= 0b111,
};

enum class PME_SUPPORT
{
    NONE = 0b00000,
    D0 = 0b00001,
    D3_HOT = 0b01000,
    D3_COLD = 0b10000,
};

enum class ASPM_L1_ENTRY_CONTROL
{
    FROM_L0 = 0x0,
    DIRECTLY_AFTER_IDLE = 0x1,
};

enum class OBFF_SUPPORT
{
    NOT_SUPPORTED = 0x0,
    MESSAGE_SIGNALING = 0x1,
    WAKE_SIGNALING = 0x2,
    WAKE_MESSAGE_SIGNALING = 0x3,
};

enum class ASPM_L0_ENTRANCE_LATENCY
{
    MICRO_SECONDS_1 = 0x0,
    MICRO_SECONDS_2 = 0x1,
    MICRO_SECONDS_3 = 0x2,
    MICRO_SECONDS_4 = 0x3,
    MICRO_SECONDS_5 = 0x4,
    MICRO_SECONDS_6 = 0x5,
    MICRO_SECONDS_7 = 0x6,
};
enum class ASPM_L1_ENTRANCE_LATENCY
{
    MICRO_SECONDS_1 = 0x0,
    MICRO_SECONDS_2 = 0x1,
    MICRO_SECONDS_4 = 0x2,
    MICRO_SECONDS_8 = 0x3,
    MICRO_SECONDS_16 = 0x4,
    MICRO_SECONDS_32 = 0x5,
    MICRO_SECONDS_64 = 0x6,
};

enum class MAC_CONFIGURATION
{
    MPBS_10 = 0x0,
    MPBS_100 = 0x1,
    MPBS_1000 = 0x2,
};

enum class DUPLEX_MODE
{
    HALF_DUPLEX = 0x0,
    FULL_DUPLEX = 0x1,
};

enum class AUTOMATIC_DUPLEX_POLARITY
{
    ASSERTED_LOW = 0x0,
    ASSERTED_HIGH = 0x1,
};

enum class LED_POLARITY
{
    ACTIVE_LOW = 0x0,
    ACTIVE_HIGH = 0x1,
};

enum class LED_CONTROL
{
    LINK_ACTIVITY = 0x0,
    LINK_1000_ACTIVITY = 0x1,
    LINK_100_ACTIVITY = 0x2,
    LINK_10_ACTIVITY = 0x3,
    LINK_100_1000_ACTIVITY = 0x4,
    LINK_10_1000_ACTIVITY = 0x5,
    LINK_10_100_ACTIVITY = 0x6,
    // RESERVERD = 0x7,
    DUPLEX_COLLISION = 0x8,
    COLLISION = 0x9,
    ACTIVITY = 0xa,
    // RESERVERD = 0xb,
    AUTO_NEGOTIATION_FAULT = 0xc,
    SERIAL_MODE = 0xd,
    FORCE_LED_OFF = 0xe,
    FORCE_LED_ON = 0xf,
};

enum class LED_COMBINE
{
    ENABLE = 0x0,
    DISABLE = 0x1,
};

enum class LED_BLINK_PULSE_STRETCH
{
    BLINK = 0x0,
    PULSE_STRETCH = 0x1,
};

enum class LED_PULSING
{
    NORMAL_OPERATION = 0x0,
    PULSE_5_KHZ = 0x1,  // Pulse with a 5-kHz, programmable duty cycle when active
};

enum class BLINK_PULSE_STRETCH_RATE
{
    HZ_2_5_400MS = 0x0,
    HZ_5_200MS = 0x1,
    HZ_10_100MS = 0x2,
    HZ_20_50MS = 0x3,
};

enum class LED_ACTIVITY_OUTPUT
{
    ACTIVE_LOW = 0x0,
    ACTIVE_HIGH = 0x1,
};

static constexpr int base_eeprom_len = 512;
/* clang-format off */
/**
  * represents a valid EEPROM config used to fill bytes of the file, where no parameters exist for
  */
static constexpr Byte base_eeprom[] {
     static_cast<Byte>(EEPROM_MAGIC::EEPROM_MAC),
          0x00, 0x02, 0x01, 0x23, 0x10, 0x55, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0x00, 0x07, 0xe6, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* clang-format on */

#pragma pack(push, 1)

struct LED_CONFIG
{
    bool enable;
    LED_POLARITY polarity;
    LED_CONTROL control;
    LED_COMBINE combineFeature;
    LED_BLINK_PULSE_STRETCH blinkPulseStretch;
};

struct EEPROM_CONFIG
{
    EEPROM_MAGIC magic{ EEPROM_MAGIC::EEPROM };
    Mac mac{ 0x00, 0x80, 0x0F, 0x74, 0x30, 0x00 };
    bool clockPowerManagement{ false };
    bool ltrMechanismSupport{ false };
    OBFF_SUPPORT obffSupport{ OBFF_SUPPORT::NOT_SUPPORTED };
    bool pciPML12Support{ true };
    bool pciPML11Support{ false };
    bool aspmL12Support{ true };
    bool aspmL11Support{ false };
    bool l1PMSubstatesSupported{ false };
    Byte16 subsystemVendorID{ 0x0 };
    Byte16 subsystemID{ 0x0 };
    AUX_CURRENT auxCurrent{ AUX_CURRENT::AC_0 };
    PME_SUPPORT pmeSupport{ PME_SUPPORT::NONE };
    ASPM_L1_ENTRY_CONTROL aspmL1EntryControl{ ASPM_L1_ENTRY_CONTROL::FROM_L0 };
    ASPM_L0_ENTRANCE_LATENCY aspmL0EntranceLatency{ ASPM_L0_ENTRANCE_LATENCY::MICRO_SECONDS_1 };
    ASPM_L1_ENTRANCE_LATENCY aspmL1EntranceLatency{ ASPM_L1_ENTRANCE_LATENCY::MICRO_SECONDS_1 };
    MAC_CONFIGURATION macConfiguration{ MAC_CONFIGURATION::MPBS_10 };
    DUPLEX_MODE duplexMode{ DUPLEX_MODE::HALF_DUPLEX };
    bool automaticSpeedDetection{ true };
    bool automaticDuplexDetection{ true };
    AUTOMATIC_DUPLEX_POLARITY automaticDuplexPolarity{ AUTOMATIC_DUPLEX_POLARITY::ASSERTED_HIGH };
    bool energyEfficientEthernet{ true };
    bool energyEfficientEthernetTxClockStop{ false };
    std::array<LED_CONFIG, 4> ledConfig{ {
        { true,
          LED_POLARITY::ACTIVE_LOW,
          LED_CONTROL::LINK_ACTIVITY,
          LED_COMBINE::ENABLE,
          LED_BLINK_PULSE_STRETCH::BLINK },
        { true,
          LED_POLARITY::ACTIVE_LOW,
          LED_CONTROL::DUPLEX_COLLISION,
          LED_COMBINE::ENABLE,
          LED_BLINK_PULSE_STRETCH::BLINK },
        { true,
          LED_POLARITY::ACTIVE_LOW,
          LED_CONTROL::LINK_10_100_ACTIVITY,
          LED_COMBINE::ENABLE,
          LED_BLINK_PULSE_STRETCH::BLINK },
        { false,
          LED_POLARITY::ACTIVE_LOW,
          LED_CONTROL::FORCE_LED_OFF,
          LED_COMBINE::ENABLE,
          LED_BLINK_PULSE_STRETCH::BLINK },
    } };
    LED_PULSING ledPulsing{ LED_PULSING::NORMAL_OPERATION };
    BLINK_PULSE_STRETCH_RATE blinkPulseStretchRate{ BLINK_PULSE_STRETCH_RATE::HZ_2_5_400MS };
    LED_ACTIVITY_OUTPUT ledActivityOutput{ LED_ACTIVITY_OUTPUT::ACTIVE_LOW };
};

struct EEPROM
{
    Byte magic;                           // 0x00
    Mac mac;                              // 0x01 - 0x06
    Byte byte7;                           // 0x07
    Byte deviceCapabilitiesEnable_1_2;    // 0x08
    Byte l1PMSubstatesCapabilitesEnable;  // 0x09
    Byte aspmConfigEnable;                // 0x0a
    Byte16 subsystemVendorID;             // 0x0b - 0x0c
    Byte16 subsystemID;                   // 0x0d - 0x0e
    Byte powerManagementCapabilities;     // 0x0f
    Byte byte16;                          // 0x10 unused
    Byte byte17;                          // 0x11 unused
    Byte deviceCapabilities_1;            // 0x12
    Byte byte19;                          // 0x13 unused
    Byte deviceCapabilities_2;            // 0x14
    Byte l1PMSubstatesCapabilites;        // 0x15
    Byte byte22;                          // 0x16 unused
    Byte byte23;                          // 0x17 unused
    Byte aspmConfig;                      // 0x18
    Byte byte25;                          // 0x19 unused
    Byte byte26;                          // 0x1a unused
    Byte macConfig1;                      // 0x1b
    Byte macConfig2;                      // 0x1c
    Byte ledConfig1;                      // 0x1d
    Byte16 ledConfig2;                    // 0x1e - 0x1f
    Byte16 ledConfig3;                    // 0x20 - 0x21

    std::array<
        Byte,
        base_eeprom_len
            - (sizeof(magic) + sizeof(mac) + sizeof(byte7) + sizeof(deviceCapabilitiesEnable_1_2)
               + sizeof(l1PMSubstatesCapabilitesEnable) + sizeof(aspmConfigEnable)
               + sizeof(subsystemVendorID) + sizeof(subsystemID)
               + sizeof(powerManagementCapabilities) + sizeof(byte16) + sizeof(byte17)
               + sizeof(deviceCapabilities_1) + sizeof(byte19) + sizeof(deviceCapabilities_2)
               + sizeof(l1PMSubstatesCapabilites) + sizeof(byte22) + sizeof(byte23)
               + sizeof(aspmConfig) + sizeof(byte25) + sizeof(byte26) + sizeof(macConfig1)
               + sizeof(macConfig2) + sizeof(ledConfig1) + sizeof(ledConfig2) + sizeof(ledConfig3))>
        base_config;
};
#pragma pack(pop)
static constexpr int eeprom_user_defined_size = base_eeprom_len - sizeof(EEPROM::base_config);

/**
 * \brief parses a MAC address string
 * takes strings of format XX:XX:XX:XX:XX:XX or XX-XX-XX-XX-XX-XX and returns it as \typedef Mac
 * \param macString
 * \param error
 * \return Mac
 */
LAN7430_CONFIG_LIB_EXPORT Mac stringToMac(const std::string& macString) noexcept(false);
/**
 * \brief creates a string representation of \typedef Mac
 * \param Mac mac
 * \return std::string
 */
LAN7430_CONFIG_LIB_EXPORT std::string macToString(const Mac& mac);
/**
 * @brief validates that a Mac address is not 00:00:00:00:00:00 or FF:FF:FF:FF:FF:FF
 * @param macArray
 * @return bool
 */
LAN7430_CONFIG_LIB_EXPORT bool validateMAC(const Mac& macArray);
/**
 * @brief validates that the given EEPROM does not contain informations that aren't specifieds
 * @param eeprom
 * @return
 */
LAN7430_CONFIG_LIB_EXPORT void validateEEPROM(const EEPROM& eeprom) noexcept(false);

/**
 * @brief creates a EEPROM_CONFIG from the given EEPROM
 * @param eeprom
 * @return
 */
LAN7430_CONFIG_LIB_EXPORT EEPROM_CONFIG eepromConfigToEEPROM(EEPROM eeprom) noexcept(false);
/**
 * @brief creates a EEPROM as byte representation from the given EEPROM_CONFIG
 * @param conf
 * @return
 */
LAN7430_CONFIG_LIB_EXPORT EEPROM createEEPROM(const EEPROM_CONFIG& conf) noexcept(false);
/**
 * @brief creates a byte representation of the given EEPROM_CONFIG and writes it to the specified
 * path
 * @param filePath
 * @param config
 * @param error
 */
LAN7430_CONFIG_LIB_EXPORT void writeEEPROM(const std::string& filePath,
                                           const EEPROM_CONFIG& config) noexcept(false);
/**
 * @brief reads the file content into a EEPROM and validates it
 * @param filePath
 * @param error
 * @return
 */
LAN7430_CONFIG_LIB_EXPORT EEPROM readEEPROM(const std::string& filePath) noexcept(false);

#endif /* LAN7430CONF_HPP */
