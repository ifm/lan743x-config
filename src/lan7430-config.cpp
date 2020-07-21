#include "validators.hpp"

#include <lan7430conf/errors.hpp>
#include <lan7430conf/lan7430conf.hpp>

#include <boost/filesystem.hpp>

#if __has_include(<cli11/CLI11.hpp>)
#include <cli11/CLI11.hpp>
#elif __has_include(<CLI/CLI.hpp>)
#include <CLI/CLI.hpp>
#endif
#include <spdlog/spdlog.h>

#include <cstddef>
#include <string>
#include <vector>

struct ConfigureCommandParameters
{
    std::string inputPath;
    std::string outputPath;

    EEPROM_MAGIC magic;
};
struct MacCommandParameters
{
    std::string macAddress;
};
struct LedCommandParameters
{
    int id;
    bool enable;
    LED_POLARITY polarity;
    LED_CONTROL control;
    LED_COMBINE combineFeature;
    LED_BLINK_PULSE_STRETCH blinkPulseStretch;
};
struct InfoCommandParameters
{
    std::string filePath;
};

int main(int argc, char const* argv[])
{
    spdlog::set_pattern("%v");

    CLI::App app;
    app.require_subcommand(1);
    app.fallthrough();
    app.set_config("--config");
    auto printConfigFlag = app.add_flag("--printconfig", "Print an ini file to the console");

    /*****************************************
     **************** CONFIGURE COMMAND ******
     *****************************************/
    ConfigureCommandParameters configParams{};
    configParams.outputPath = "lan7430_config.bin";
    auto configCommand = app.add_subcommand("configure");
    auto cInputOption = configCommand
                            ->add_option("-i,--input",
                                         configParams.inputPath,
                                         "Input that the output EEPROM file should be based on")
                            ->check(CLI::ExistingFile);
    auto cOutputOption
        = configCommand
              ->add_option("-o,--output", configParams.outputPath, "Output path of the EEPROM file")
              ->capture_default_str()
              ->check(ExistingPath);

    auto cMemoryOption = configCommand
                             ->add_option("--memory",
                                          configParams.magic,
                                          "Determines where this configuration will be stored")
                             ->transform(CLI::CheckedTransformer(
                                 std::vector<std::pair<std::string, EEPROM_MAGIC>>{
                                     { "eeprom", EEPROM_MAGIC::EEPROM },
                                     { "eepromMac", EEPROM_MAGIC::EEPROM_MAC },
                                     { "eepromOTP1", EEPROM_MAGIC::EEPROM_OTP1 },
                                     { "eepromOTP2", EEPROM_MAGIC::EEPROM_OTP2 },
                                 },
                                 CLI::ignore_case));

    configCommand->callback([&]() {
        try
        {
            EEPROM_CONFIG config{};
            if (*cInputOption)
            {
                EEPROM eeprom = readEEPROM(configParams.inputPath);
                config = eepromConfigToEEPROM(eeprom);
            }

            if (*cMemoryOption)
            {
                config.magic = configParams.magic;
            }

            // the user can give us different input/output paths.
            // we set the inputPath = outputPath here, so following command callbacks
            // work with the created file from now on...
            if (*cInputOption)
            {
                configParams.inputPath = configParams.outputPath;
            }
            writeEEPROM(configParams.outputPath, config);
        }
        catch (ifm::error_type e)
        {
            SPDLOG_ERROR("Error occured in command configure: {} - {}", e.code(), e.what());
            throw CLI::RuntimeError(e.what(), e.code());
        }
    });


    /*****************************************
     **************** MAC COMMAND ************
     *****************************************/
    MacCommandParameters macParams{};
    CLI::App* macCommand
        = configCommand->add_subcommand("mac", "Allows configuring the mac address");
    auto mMacOption
        = macCommand
              ->add_option("mac,-m,--mac", macParams.macAddress, "MAC address (00:00:00:00:00:00)")
              ->check(ValidMac)
              ->required();

    macCommand->callback([&]() {
        try
        {
            EEPROM_CONFIG config{};
            if (*cInputOption)
            {
                EEPROM eeprom = readEEPROM(configParams.inputPath);
                config = eepromConfigToEEPROM(eeprom);
            }

            if (*mMacOption)
            {
                // remove any whitespace
                macParams.macAddress.erase(
                    std::remove_if(macParams.macAddress.begin(),
                                   macParams.macAddress.end(),
                                   [l = std::locale{}](auto ch) { return std::isspace(ch, l); }),
                    macParams.macAddress.end());

                config.mac = stringToMac(macParams.macAddress);
            }

            // the user can give us different input/output paths.
            // we set the inputPath = outputPath here, so following command callbacks
            // work with the created file from now on...
            if (*cInputOption)
            {
                configParams.inputPath = configParams.outputPath;
            }
            writeEEPROM(configParams.outputPath, config);
        }
        catch (ifm::error_type e)
        {
            SPDLOG_ERROR("Error occured in subcommand mac: {} - {}", e.code(), e.what());
            throw CLI::RuntimeError(e.what(), e.code());
        }
    });


    /*****************************************
     **************** LED COMMAND ************
     *****************************************/
    LedCommandParameters ledParams{};
    CLI::App* ledCommand
        = configCommand->add_subcommand("led", "Allows configuring the parameters of the LEDs");
    ledCommand->add_option("id,--id", ledParams.id, "ID of the LED you want to configure")
        ->check(CLI::Range(0, 3))
        ->required();
    auto lEnableFlag = ledCommand->add_flag("--on,!--off,--enable,!--no-enable,!--disable",
                                            ledParams.enable,
                                            "Enables or disables the LED");
    auto lPolarityOption
        = ledCommand
              ->add_option("polarity,-p,--polarity", ledParams.polarity, "Invert LED polarity")
              ->transform(CLI::CheckedTransformer(
                  std::vector<std::pair<std::string, LED_POLARITY>>{
                      { "ActiveLow", LED_POLARITY::ACTIVE_LOW },
                      { "ActiveHigh", LED_POLARITY::ACTIVE_HIGH },
                  },
                  CLI::ignore_case));
    auto lControlOption
        = ledCommand
              ->add_option("control,-c,--control", ledParams.control, "Selects LED activity output")
              ->transform(CLI::CheckedTransformer(
                  std::vector<std::pair<std::string, LED_CONTROL>>{
                      { "LinkActivity", LED_CONTROL::LINK_ACTIVITY },
                      { "Link1000Activity", LED_CONTROL::LINK_1000_ACTIVITY },
                      { "Link100Activity", LED_CONTROL::LINK_100_ACTIVITY },
                      { "Link10Activity", LED_CONTROL::LINK_10_ACTIVITY },
                      { "Link100_1000Activity", LED_CONTROL::LINK_100_1000_ACTIVITY },
                      { "Link10_1000Activity", LED_CONTROL::LINK_10_1000_ACTIVITY },
                      { "Link10_100Activity", LED_CONTROL::LINK_10_100_ACTIVITY },
                      { "DuplexCollision", LED_CONTROL::DUPLEX_COLLISION },
                      { "Collision", LED_CONTROL::COLLISION },
                      { "Activity", LED_CONTROL::ACTIVITY },
                      { "AutoNegotiationFault", LED_CONTROL::AUTO_NEGOTIATION_FAULT },
                      { "SerialModel", LED_CONTROL::SERIAL_MODE },
                      { "ForceLedOff", LED_CONTROL::FORCE_LED_OFF },
                      { "ForceLedOn", LED_CONTROL::FORCE_LED_ON },
                  },
                  CLI::ignore_case));
    auto lCombineOption = ledCommand
                              ->add_option("combine,-C,--combine",
                                           ledParams.combineFeature,
                                           "Combines link/activity and duplex/collision")
                              ->transform(CLI::CheckedTransformer(
                                  std::vector<std::pair<std::string, LED_COMBINE>>{
                                      { "Enable", LED_COMBINE::ENABLE },
                                      { "Disable", LED_COMBINE::DISABLE },
                                  },
                                  CLI::ignore_case));
    auto lBlinkOption = ledCommand
                            ->add_option("blink,-b,--blink",
                                         ledParams.blinkPulseStretch,
                                         "Configures blink or pulse-stretch")
                            ->transform(CLI::CheckedTransformer(
                                std::vector<std::pair<std::string, LED_BLINK_PULSE_STRETCH>>{
                                    { "Blink", LED_BLINK_PULSE_STRETCH::BLINK },
                                    { "PulseStretch", LED_BLINK_PULSE_STRETCH::PULSE_STRETCH },
                                },
                                CLI::ignore_case));

    ledCommand->callback([&]() {
        try
        {
            EEPROM_CONFIG config{};
            if (*cInputOption)
            {
                EEPROM eeprom = readEEPROM(configParams.inputPath);
                config = eepromConfigToEEPROM(eeprom);
            }

            assert(ledParams.id >= 0);
            assert(ledParams.id < config.ledConfig.size());

            if (*lEnableFlag)
            {
                config.ledConfig[ledParams.id].enable = ledParams.enable;
            }
            if (*lPolarityOption)
            {
                config.ledConfig[ledParams.id].polarity = ledParams.polarity;
            }
            if (*lControlOption)
            {
                config.ledConfig[ledParams.id].control = ledParams.control;
            }
            if (*lCombineOption)
            {
                config.ledConfig[ledParams.id].combineFeature = ledParams.combineFeature;
            }
            if (*lBlinkOption)
            {
                config.ledConfig[ledParams.id].blinkPulseStretch = ledParams.blinkPulseStretch;
            }

            // the user can give us different input/output paths.
            // we set the inputPath = outputPath here, so following command callbacks
            // work with the created file from now on...
            if (*cInputOption)
            {
                configParams.inputPath = configParams.outputPath;
            }
            writeEEPROM(configParams.outputPath, config);
        }
        catch (ifm::error_type e)
        {
            SPDLOG_ERROR("Error occured in subcommand led: {} - {}", e.code(), e.what());
            throw CLI::RuntimeError(e.what(), e.code());
        }
    });


    /*****************************************
     **************** INFO COMMAND ***********
     *****************************************/
    InfoCommandParameters infoParams{};
    auto infoCommand = app.add_subcommand("info");
    infoParams.filePath = "lan7430_config.bin";
    infoCommand
        ->add_option("input,-i,--input",
                     infoParams.filePath,
                     "Dump information about the EEPROM file to console")
        ->capture_default_str()
        ->check(CLI::ExistingFile);
    infoCommand->callback([&]() {
        try
        {
            EEPROM eeprom = readEEPROM(infoParams.filePath);
            EEPROM_CONFIG config = eepromConfigToEEPROM(eeprom);

            switch (EEPROM_MAGIC(eeprom.magic))
            {
                case EEPROM_MAGIC::EEPROM:
                    SPDLOG_INFO("EEPROM magic: {:#04X} <-- load EEPROM", eeprom.magic);
                    break;
                case EEPROM_MAGIC::EEPROM_MAC:
                    SPDLOG_INFO("EEPROM magic: {:#04X} <-- load MAC from EEPROM", eeprom.magic);
                    break;
                case EEPROM_MAGIC::EEPROM_OTP1:
                    SPDLOG_INFO("EEPROM magic: {:#04X} <-- write EEPROM to OTP1 (see datasheet "
                                "EEPROM Controller (EEP))",
                                eeprom.magic);
                    break;
                case EEPROM_MAGIC::EEPROM_OTP2:
                    SPDLOG_INFO("EEPROM magic: {:#04X} <-- write EEPROM to OTP2 (see datasheet "
                                "EEPROM Controller (EEP))",
                                eeprom.magic);
                    break;
                default:
                    SPDLOG_INFO("EEPROM magic: {:#04X} <-- do nothing with EEPROM", eeprom.magic);
                    break;
            }

            SPDLOG_INFO("MAC: {}", macToString(eeprom.mac));

            int ledID = 0;
            for (const auto& ledConfig : config.ledConfig)
            {
                SPDLOG_INFO("LED {}:\tenabled: {}", ledID, ledConfig.enable);
                SPDLOG_INFO("\tpolarity:{:<2} control:{:<2} combine:{:<2} blink:{:<2}",
                            static_cast<uint32_t>(ledConfig.polarity),
                            static_cast<uint32_t>(ledConfig.control),
                            static_cast<uint32_t>(ledConfig.combineFeature),
                            static_cast<uint32_t>(ledConfig.blinkPulseStretch));
                ++ledID;
            }
        }
        catch (ifm::error_type e)
        {
            SPDLOG_ERROR("Error occured in subcommand info: {} - {}", e.code(), e.what());
            throw CLI::RuntimeError(e.what(), e.code());
        }
    });

    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e)
    {
        return app.exit(e);
    }

    if (*printConfigFlag)
    {
        SPDLOG_INFO("----- config.toml begin -----");
        SPDLOG_INFO(app.config_to_str(true, true));
        SPDLOG_INFO("----- config.toml end -----");
    }

    if (*configCommand)
    {
        if (*cInputOption && configParams.inputPath == configParams.outputPath)
        {
            SPDLOG_INFO("Modified EEPROM at: {}", configParams.outputPath);
        }
        else
        {
            SPDLOG_INFO("Created EEPROM at: {}", configParams.outputPath);
        }
    }

    return 0;
}
