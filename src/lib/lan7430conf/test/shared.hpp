/** @file shared.hpp
 *
 *  @brief
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */
#ifndef SHARED_HPP
#define SHARED_HPP

#include <lan7430conf/lan7430conf.hpp>

#include <vector>

static std::vector<std::pair<std::string, EEPROM_CONFIG>> gs_testFilesVector{
    { "files/00-80-0F-74-30-01-default_DevId.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ false,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ false,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
      } },
    { "files/00-80-0F-74-30-01-default_DevId_OTP.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM_OTP1,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ false,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ false,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
      } },
    { "files/00-80-0F-74-30-01-default_MAC.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM_MAC,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ false,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ false,
          /*subsystemVendorID = */ 0x0,
          /*subsystemID = */ 0x0,
      } },
    { "files/00-80-0F-74-30-01-L1_substates_off.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ false,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ false,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
      } },
    { "files/00-80-0F-74-30-01-L1_substates_on.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ false,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ true,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
      } },
    { "files/00-80-0F-74-30-01-L1_substates_all_on.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ false,
          /*ltrMechanismSupport = */ false,
          /*obffSupport = */ OBFF_SUPPORT::NOT_SUPPORTED,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ true,
          /*aspmL12Support = */ true,
          /*aspmL11Support = */ true,
          /*l1PMSubstatesSupported = */ true,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
      } },
    { "files/00-80-0F-74-30-01-random.bin",
      {
          /*magic = */ EEPROM_MAGIC::EEPROM,
          /*mac = */ stringToMac("00-80-0F-74-30-01"),
          /*clockPowerManagement = */ true,
          /*ltrMechanismSupport = */ true,
          /*obffSupport = */ OBFF_SUPPORT::WAKE_MESSAGE_SIGNALING,
          /*pciPML12Support = */ true,
          /*pciPML11Support = */ true,
          /*aspmL12Support = */ false,
          /*aspmL11Support = */ false,
          /*l1PMSubstatesSupported = */ true,
          /*subsystemVendorID = */ 0x1234,
          /*subsystemID = */ 0x5678,
          /*auxCurrent = */ AUX_CURRENT::AC_375,
          /*pmeSupport = */ PME_SUPPORT::D3_COLD,
          /*aspmL1EntryControl*/ ASPM_L1_ENTRY_CONTROL::DIRECTLY_AFTER_IDLE,
          /*aspmL0EntranceLatency*/ ASPM_L0_ENTRANCE_LATENCY::MICRO_SECONDS_6,
          /*aspmL1EntranceLatency*/ ASPM_L1_ENTRANCE_LATENCY::MICRO_SECONDS_32,
          /*macConfiguration = */ MAC_CONFIGURATION::MPBS_1000,
          /*duplexMode = */ DUPLEX_MODE::FULL_DUPLEX,
          /*automaticSpeedDetection = */ false,
          /*automaticDuplexDetection = */ false,
          /*automaticDuplexPolarity = */ AUTOMATIC_DUPLEX_POLARITY::ASSERTED_LOW,
          /*energyEfficientEthernet = */ false,
          /*energyEfficientEthernetTxClockStop = */ true,
          { {
              /*ledConfig[0] = */
              {
                  /*enable = */ true,
                  /*polarity = */ LED_POLARITY::ACTIVE_HIGH,
                  /*control = */ LED_CONTROL::ACTIVITY,
                  /*combineFeature = */ LED_COMBINE::DISABLE,
                  /*blinkPulseStretch = */ LED_BLINK_PULSE_STRETCH::PULSE_STRETCH,
              },
              /*ledConfig[1] = */
              {
                  /*enable = */ false,
                  /*polarity = */ LED_POLARITY::ACTIVE_HIGH,
                  /*control = */ LED_CONTROL::DUPLEX_COLLISION,
                  /*combineFeature = */ LED_COMBINE::ENABLE,
                  /*blinkPulseStretch = */ LED_BLINK_PULSE_STRETCH::PULSE_STRETCH,
              },
              /*ledConfig[2] = */
              {
                  /*enable = */ true,
                  /*polarity = */ LED_POLARITY::ACTIVE_HIGH,
                  /*control = */ LED_CONTROL::ACTIVITY,
                  /*combineFeature = */ LED_COMBINE::ENABLE,
                  /*blinkPulseStretch = */ LED_BLINK_PULSE_STRETCH::BLINK,
              },
              /*ledConfig[3] = */
              {
                  /*enable = */ true,
                  /*polarity = */ LED_POLARITY::ACTIVE_HIGH,
                  /*control = */ LED_CONTROL::LINK_1000_ACTIVITY,
                  /*combineFeature = */ LED_COMBINE::DISABLE,
                  /*blinkPulseStretch = */ LED_BLINK_PULSE_STRETCH::PULSE_STRETCH,
              },
          } },
          /*ledPulsing = */ LED_PULSING::PULSE_5_KHZ,
          /*blinkPulseStretchRate = */ BLINK_PULSE_STRETCH_RATE::HZ_10_100MS,
          /*ledActivityOutput = */ LED_ACTIVITY_OUTPUT::ACTIVE_HIGH,
      } }
};

#endif  // SHARED_HPP
