/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#include "board/Board.h"

// class StorageAccess : public LESSDB::StorageAccess
// {
//     public:
//     StorageAccess() = default;

//     bool init() override
//     {
//         return Board::NVM::init();
//     }

//     uint32_t size() override
//     {
//         return Board::NVM::size();
//     }

//     bool clear() override
//     {
//         return Board::NVM::clear(0, Board::NVM::size());
//     }

//     bool read(uint32_t address, int32_t& value, LESSDB::sectionParameterType_t type) override
//     {
//         switch (type)
//         {
//         case LESSDB::sectionParameterType_t::word:
//             return Board::NVM::read(address, value, Board::NVM::parameterType_t::word);

//         case LESSDB::sectionParameterType_t::dword:
//             return Board::NVM::read(address, value, Board::NVM::parameterType_t::dword);

//         default:
//             return Board::NVM::read(address, value, Board::NVM::parameterType_t::byte);
//         }
//     }

//     bool write(uint32_t address, int32_t value, LESSDB::sectionParameterType_t type) override
//     {
//         switch (type)
//         {
//         case LESSDB::sectionParameterType_t::word:
//             return Board::NVM::write(address, value, Board::NVM::parameterType_t::word);

//         case LESSDB::sectionParameterType_t::dword:
//             return Board::NVM::write(address, value, Board::NVM::parameterType_t::dword);

//         default:
//             return Board::NVM::write(address, value, Board::NVM::parameterType_t::byte);
//         }
//     }

//     size_t paramUsage(LESSDB::sectionParameterType_t type) override
//     {
//         switch (type)
//         {
//         case LESSDB::sectionParameterType_t::word:
//             return Board::NVM::paramUsage(Board::NVM::parameterType_t::word);

//         case LESSDB::sectionParameterType_t::dword:
//             return Board::NVM::paramUsage(Board::NVM::parameterType_t::dword);

//         default:
//             return Board::NVM::paramUsage(Board::NVM::parameterType_t::byte);
//         }
//     }
// } storageAccess;

// Database database(storageAccess, false);

// class HWAMIDI : public MIDI::HWA
// {
//     public:
//     HWAMIDI() = default;

//     bool init() override
//     {
//         return true;
//     }

//     bool dinRead(uint8_t& data) override
//     {
// #ifdef DIN_MIDI_SUPPORTED
//         return Board::UART::read(UART_CHANNEL_DIN, data);
// #else
//         return false;
// #endif
//     }

//     bool dinWrite(uint8_t data) override
//     {
// #ifdef DIN_MIDI_SUPPORTED
//         return Board::UART::write(UART_CHANNEL_DIN, data);
// #else
//         return false;
// #endif
//     }

//     bool usbRead(MIDI::USBMIDIpacket_t& USBMIDIpacket) override
//     {
//         return Board::USB::readMIDI(USBMIDIpacket);
//     }

//     bool usbWrite(MIDI::USBMIDIpacket_t& USBMIDIpacket) override
//     {
//         return Board::USB::writeMIDI(USBMIDIpacket);
//     }
// } hwaMIDI;

// //buttons and encoders have the same data source which is digital input
// //this helper class pulls the latest data from board and then feeds it into HWAButtons and HWAEncoders
// class HWADigitalIn
// {
//     public:
//     HWADigitalIn() = default;

//     bool buttonState(size_t index, uint8_t& numberOfReadings, uint32_t& states)
//     {
//         //if encoder under this index is enabled, just return false state each time
//         //side note: don't bother with references to dependencies here, just use global database object
//         if (database.read(Database::Section::encoder_t::enable, Board::io::encoderIndex(index)))
//             return false;

//         if (!Board::io::digitalInState(index, dInReadA))
//             return false;

//         numberOfReadings = dInReadA.count;
//         states           = dInReadA.readings;

//         return true;
//     }

//     bool encoderState(size_t index, uint8_t& numberOfReadings, uint32_t& states)
//     {
//         if (!Board::io::digitalInState(Board::io::encoderSignalIndex(index, Board::io::encoderIndex_t::a), dInReadA))
//             return false;

//         if (!Board::io::digitalInState(Board::io::encoderSignalIndex(index, Board::io::encoderIndex_t::b), dInReadB))
//             return false;

//         numberOfReadings = dInReadA.count > dInReadB.count ? dInReadA.count : dInReadB.count;

//         //construct encoder pair readings
//         //encoder signal is made of A and B signals
//         //take each bit of A signal and B signal and append it to states variable in order
//         //latest encoder readings should be in LSB bits

//         for (uint8_t i = 0; i < numberOfReadings; i++)
//         {
//             BIT_WRITE(states, (i * 2) + 1, (dInReadA.readings >> i & 0x01));
//             BIT_WRITE(states, i * 2, (dInReadB.readings >> i & 0x01));
//         }

//         return true;
//     }

//     private:
//     Board::io::dInReadings_t dInReadA;
// #ifdef ENCODERS_SUPPORTED
//     Board::io::dInReadings_t dInReadB;
// #endif
// } hwaDigitalIn;
// #endif

// class HWAButtons : public IO::Buttons::HWA
// {
//     public:
//     HWAButtons() = default;

//     bool state(IO::Buttons::button_t index, bool& state) override
//     {
//         return hwaDigitalIn.buttonState(buttonBoardIndex[static_cast<uint8_t>(index)], numberOfReadings, states);
//     }

//     private:
//     uint8_t buttonBoardIndex[static_cast<uint8_t>(IO::Buttons::button_t::AMOUNT)] = {
//         BUTTON_NOTE_C_SHARP,
//         BUTTON_NOTE_D_SHARP,
//         BUTTON_NOTE_F_SHARP,
//         BUTTON_NOTE_G_SHARP,
//         BUTTON_NOTE_A_SHARP,
//         BUTTON_NOTE_C,
//         BUTTON_NOTE_D,
//         BUTTON_NOTE_E,
//         BUTTON_NOTE_F,
//         BUTTON_NOTE_G,
//         BUTTON_NOTE_A,
//         BUTTON_NOTE_B,
//         BUTTON_ON_OFF_SPLIT,
//         BUTTON_ON_OFF_X,
//         BUTTON_ON_OFF_Y,
//         BUTTON_ON_OFF_AFTERTOUCH,
//         BUTTON_ON_OFF_NOTES,
//         BUTTON_OCTAVE_DOWN,
//         BUTTON_OCTAVE_UP,
//         BUTTON_TRANSPORT_PLAY,
//         BUTTON_TRANSPORT_STOP,
//         BUTTON_TRANSPORT_RECORD,
//         BUTTON_PROGRAM_ENC,
//         BUTTON_PRESET_ENC,
//     };
// } hwaButtons;

// void startUpAnimation()
// {
//     ledState_t tempLedStateArray[MAX_NUMBER_OF_LEDS];

//     for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
//     {
//         //copy ledstates to temp field
//         tempLedStateArray[i] = leds.getLEDstate(i);
//         //turn all leds off
//         leds.setLEDstate(i, ledStateOff);
//     }

//     //turn all leds
//     leds.setAllOn();

//     //enable global interrupts so that leds can actually turn on
//     sei();

//     display.displayWelcomeMessage();

//     wait_ms(1500);

//     //restore led states
//     for (int i = 0; i < MAX_NUMBER_OF_LEDS; i++)
//         leds.setLEDstate(i, tempLedStateArray[i]);

//     wait_ms(1000);
// }

int main()
{
    //do not change order of initialization!

    //     board.init();
    //     display.init();
    //     database.init();

    //     if (!database.signatureValid())
    //     {
    //         display.setDirectWriteState(true);
    //         display.clearAll();

    //         stringBuffer.startLine();
    //         stringBuffer.appendText_P(dbInit_string);
    //         stringBuffer.endLine();
    //         display.updateText(1, displayText_still, display.getTextCenter(ARRAY_SIZE(dbInit_string)));

    //         stringBuffer.startLine();
    //         stringBuffer.appendText_P(pleaseWait_string);
    //         stringBuffer.endLine();
    //         display.updateText(2, displayText_still, display.getTextCenter(ARRAY_SIZE(pleaseWait_string)));

    //         database.factoryReset(LESSDB::factoryResetType_t::full);
    //         display.setDirectWriteState(false);
    //     }

    //     midi.setInputChannel(1);
    //     midi.setNoteOffMode((noteOffType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID));
    //     midi.setRunningStatusState(database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID));
    //     pads.init();
    //     encoders.init();

    // #ifdef START_UP_ANIMATION
    //     startUpAnimation();
    // #else
    //     sei();
    // #endif

    //     display.setupHomeScreen();
    //     buttons.init();

    //     //flush all data from encoders
    //     encoders.update(false);

    //     //start first conversion manually
    //     startADCconversion();

    //     while (1)
    //     {
    // #ifdef DEBUG
    //         CDC_Update();
    // #endif

    //         pads.update();
    //         digitalInput.update();
    //         display.update();
    //         leds.update();
    //     }

    while (1)
    {
    }

    return 0;
}