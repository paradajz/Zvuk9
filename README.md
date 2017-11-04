# Zvuk9

USB MIDI controller with 9 force and position sensing pads used to play melodic instruments or drums / samples. Pads are sending:

- note
- note velocity
- note aftertouch
- 2 control changes through x and y coordinates (touch position) or pitch bend

Note, aftertouch, x and y sending can be disabled each independently. X and Y have configurable min and max values, curve and MIDI controller that is being sent. All nine pads can use the same X,Y cc/max/min/curve values (useful for melodic instruments) or could be set uniquely (useful for playing drums, samples). Pads are set up to play one of the predetermined scales. Alternatively, they can be set to play custom scale or chords through user defined presets. Controller is capable of sending MIDI messages via USB or via MIDI Out port.

# Compiling

In order to compile Zvuk9, Atmel Studio 7 is needed. `Debug` and `Release` configurations are available. Debug configuration uses LUFA virtual serial mode to print various messages during Zvuk9 usage, while `Release` configuration compiles with USB MIDI support.

Requirements:

- Atmel Studio 7
- Windows 10 with Linux subsytem (Ubuntu) *
- Binary utilities: avr-objcopy, srec_cat *

_Note: While asterisk marked components are optional for compiling, they are needed to correctly write firmware version and CRC to compiled binary._

# Bootloader

Zvuk9 uses LUFA HID bootloader for firmware updating. Bootloader can be programmed either via Atmel Studio or `avrdude`. Using Atmel Sutdio, set correct fuse settings:

* Low fuse: `0xFF`
* High fuse: `0xD2`
* Extended fuse: `0xF8`

After that, select `BootloaderHID.hex` under `Release` directory (`src/BootloaderHID/Release`) or under `/bin` directory and program it.

Alternatively, bootloader can be programmed using `avrdude` using `usbasp` programmer with following command (assuming it's being run from `/bin` or `src/BootloaderHID/Release` directory):

`avrdude -p usb1286  -c usbasp -U lfuse:w:0xff:m -U hfuse:w:0xd2:m -U efuse:w:0xf8:m -Uflash:w:BootloaderHID.hex:i`

# Firmware upload

To upload compiled firmware .hex file, use `hid_bootloader_loader.exe` found under `/bin` directory.

# Licencing

Zvuk9 uses various modules which are licenced under different licenses. 

- LUFA is licenced under modified MIT licence.
- MIDI module is licenced under MIT.
- U8G2 is licenced under Two-clause BSD licence. This project uses modified content which is available under same terms.
- AVR-Core, AVR-Versioning, AVR-DB and strings modules are available under MIT licence.

This project is available under GNU GPLv3 licence. For commerical uses commercial licence is available on demand.