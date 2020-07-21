# Configuration of the LAN7430 PCIe GigE Ethernet card

The LAN7430 has the possibility to read its MAC address from a EEPROM. To achieve this the EEPROM content needs to be generated and written to the EEPROM.

# Build
To build the CLI tool and the library, you need a compiler that supports C++17
## Dependencies
CLI tool:
* boost-system > 1.58.0 (system)
* boost-filesystem > 1.58.0 (system)
* spdlog (system/3rdparty)
* cli11 (system/3rdparty)

Library:
* boost-system > 1.58.0 (system)
* boost-filesystem > 1.58.0 (system)
* spdlog (system/3rdparty)

Tests:
* boost-system > 1.58.0 (system)
* boost-filesystem > 1.58.0 (system)
* catch2 (3rdparty)
* spdlog (system/3rdparty)

```
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -H. -Bbuild
cmake --build build
```

# CLI usage
```
Usage: ./lan7430-config [OPTIONS] SUBCOMMAND

Options:
  -h,--help                   Print this help message and exit
  --config TEXT               Read an ini file
  --printconfig               Print an ini file to the console

Subcommands:
  configure
  info  
```


***
## *configure* subcommand
You can build up a *.bin file by using the configure command and it's subcommands, adding more configurations as you go.

When a file is created for the first time a reasonable configuration is used.
```
Usage: ./lan7430-config configure [OPTIONS] [SUBCOMMAND]

Options:
  -h,--help                   Print this help message and exit
  -i,--input TEXT:FILE        Input that the output EEPROM file should be based on
  -o,--output TEXT:DIR=lan7430_config.bin
                              Output path of the EEPROM file
  --memory ENUM:value in {eeprom->165,eepromMac->170,eepromOTP1->243,eepromOTP2->247} OR {165,170,243,247}
                              Determines where this configuration will be stored

Subcommands:
  mac                         Allows configuring the mac address
  led                         Allows configuring the parameters of the LEDs
```

Please note that currently the command line tool is very limited in regards of options that can be configured.

To have access to more configurable options, you can use the lan7430conf library.

#### Example:
```
lan7430-config configure -o lan7430_config.bin --memory eeprom
```

### *mac* subcommand
```
Allows configuring the mac address
Usage: ./lan7430-config configure mac [OPTIONS] mac

Positionals:
  mac TEXT:MAC REQUIRED       MAC address (00:00:00:00:00:00)

Options:
  -h,--help                   Print this help message and exit
  -m,--mac TEXT:MAC REQUIRED  MAC address (00:00:00:00:00:00)
```

#### Example:
```
lan7430-config configure -i lan7430_config.bin -o lan7430_config.bin mac AA:BB:CC:DD:EE:FF
```

### *led* subcommand
#### Example:
```
lan7430-config configure -i lan7430_config.bin -o lan7430_config.bin led --id 0 --on -p ActiveHigh -c LinkActivity -C Enable -b Blink
```


***
## *info* subcommand

```
Usage: ./lan7430-config info [OPTIONS] [input]

Positionals:
  input TEXT:FILE=lan7430_config.bin
                              Dump information about the EEPROM file to console

Options:
  -h,--help                   Print this help message and exit
  -i,--input TEXT:FILE=lan7430_config.bin
                              Dump information about the EEPROM file to console
```

#### Example:
```
lan7430-config info -i lan7430_config.bin
```


# Reading of the EEPROM

```
$ ethtool --eeprom-dump devname raw on > eeprom.bin
```

# Writing of the EEPROM

```
$ ethtool --change-eeprom eth1 magic 0x74A5 < 00-02-01-23-10-58-pad.bin
```

# Content of the EEPROM

This is a HEXDUMP of the EEPROM file for the MAC Address ``00:02:01:23:10:58``

```
hexdump -C doc/bin/00-02-01-23-10-58-pad.bin
00000000  aa 00 02 01 23 10 58 00  00 14 00 00 00 00 00 00  |....#.X.........|
00000010  00 00 00 00 00 05 00 00  00 00 00 b8 00 07 e6 80  |................|
00000020  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
00000200
```

There seems to be no checksum to verify the EEPROM content the only difference between two files is the MAC address

```
--- doc/bin/00-02-01-23-10-58-pad.txt   2020-03-12 09:46:55.116083608 +0100
+++ doc/bin/00-02-01-23-10-57-pad.txt   2020-03-12 09:47:01.884083558 +0100
@@ -1,4 +1,4 @@
-00000000  aa 00 02 01 23 10 58 00  00 14 00 00 00 00 00 00  |....#.X.........|
+00000000  aa 00 02 01 23 10 57 00  00 14 00 00 00 00 00 00  |....#.W.........|
 00000010  00 00 00 00 00 05 00 00  00 00 00 b8 00 07 e6 80  |................|
 00000020  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
 *
```
