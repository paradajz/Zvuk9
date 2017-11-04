# Zvuk9

Zvuk9 is an expressive USB MIDI controller used to play melodic instruments or drums / samples.

Zvuk9 features:

- 9 pressure and position sensitive pads
- Velocity sensitivity (3 levels of sensitivity and 3 velocity response curves)
- X and Y MIDI assignable to CC (CCs can be the same for all pads or unique per pad)
- X or Y can be set to send pitch bend instead of CC
- Aftertouch (channel or polyphonic)
- 15 programs for storing settings (scale, tonic, split status, X & Y controls, on/off status for notes, aftertouch, and X and Y)
- 7 predefined scales available: natural minor, harmonic minor, melodic minor, major, harmonic major, pentatonic minor, and pentatonic major
- 10 user scales available so you can set which pad will play which note or notes
- Chords can be assigned through user scales, a maximum of 7 notes can be assigned to a pad
- Tonic can be set with Notes buttons
- Notes buttons are illuminated, showing which notes are currently available and which note is currently playing
- By default, tonic goes on Pad1; by using Notes up/down function, all notes can be moved
- Encoder controls for X & Y: CC/Pitch bend, min, max, curve
- Total of 13 response curves available for X and Y
- 4 on/off buttons: notes, X, Y, and aftertouch
- In Split mode, all controls for X and Y, state of 4 on/off buttons, and MIDI channel are unique per pad
- Transport controls: play, stop, and record can be set to MCC, CC, or MCC+CC
- Note off type choice (off channel, on channel/0 velocity)
- All settings saved during reboot
- MIDI over USB and MIDI through 3.5mm mini jack out (cable mini jack to MIDI DIN included)
- Configurable MIDI running status for DIN MIDI
- OLED display

# Compiling

In order to compile Zvuk9, Atmel Studio 7 is needed. `Debug` and `Release` configurations are available. `Debug` configuration uses LUFA virtual serial mode to print various messages during Zvuk9 usage, while `Release` configuration compiles with USB MIDI support.

Requirements:

- Atmel Studio 7
- Windows 10 with Linux subsytem (Ubuntu) *
- Binary utilities: avr-objcopy, srec_cat *

_Note: While asterisk marked components are optional for compiling, they are needed to correctly write firmware version and CRC to compiled binary._

# Bootloader

Zvuk9 uses LUFA HID bootloader for firmware updating. Bootloader can be programmed either via Atmel Studio or `avrdude`. Using Atmel Studio, set correct fuse settings:

* Low fuse: `0xFF`
* High fuse: `0xD2`
* Extended fuse: `0xF8`

After that, select `BootloaderHID.hex` under `Release` directory (`src/BootloaderHID/Release`) or under `/bin` directory and program it.

Alternatively, bootloader can be programmed using `avrdude` and `usbasp` programmer with following command (assuming it's being run from `/bin` or `src/BootloaderHID/Release` directory):

`avrdude -p usb1286  -c usbasp -U lfuse:w:0xff:m -U hfuse:w:0xd2:m -U efuse:w:0xf8:m -Uflash:w:BootloaderHID.hex:i`

# Firmware upload

Before uploading Zvuk9 firmware, controller must be put into firmware update mode. To do this, unplug controller from USB, press and hold small button located next to MIDI connector and connect USB cable to PC. After that, PC will recognize Zvuk9 as HID device. Once the bootloader is active, run `hid_bootloader_loader.exe` found under `/bin` directory. This tool will automatically write firwmare to Zvuk9. Make sure `FLASH.hex` is present in directory. Controller will automatically load new firmware once updating is finished.

# Licencing

Zvuk9 uses various modules which are licenced under different licenses. 

- LUFA is licenced under modified MIT licence.
- MIDI module is licenced under MIT.
- U8G2 is licenced under Two-clause BSD licence. This project uses modified content which is available under same terms.
- AVR-Core, AVR-Versioning, AVR-DB and strings modules are available under MIT licence.

This project is available under GNU GPLv3 licence. For commerical uses commercial licence is available on demand.