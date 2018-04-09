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

#include "interface/buttons/Buttons.h"
#include "interface/encoders/Encoders.h"
#include "interface/lcd/LCD.h"
#include "interface/lcd/menu/Menu.h"
#include "interface/leds/LEDs.h"
#include "interface/pads/Pads.h"
#include "database/Database.h"

#ifdef DEBUG
#include "vserial/Serial.h"
#endif

MIDI midi;

void startUpAnimation()
{
    //slow down fading for effect
    leds.setFadeSpeed(3);

    ledState_t tempLedStateArray[MAX_NUMBER_OF_LEDS];

    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
    {
        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledStateOff);
    }

    //turn all leds on slowly
    leds.setAllOn();

    sei();

    display.displayWelcomeMessage();

    wait_ms(1500);

    //restore led states
    for (int i=0; i<MAX_NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    wait_ms(1000);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);
}

int main()
{
    //do not change order of initialization!

    #ifdef DEBUG
    vserial.init();
    #endif

    board.init();
    display.init();

    database.init();

    // if (!database.signatureValid())
    // {
    //     display.setDirectWriteState(true);
    //     stringBuffer.appendText_P(dbInit_string);
    //     display.updateText(1, lcdtext_still, display.getTextCenter(ARRAY_SIZE(dbInit_string)));
    //     stringBuffer.appendText_P(pleaseWait_string);
    //     display.updateText(2, lcdtext_still, display.getTextCenter(ARRAY_SIZE(pleaseWait_string)));
    //     database.factoryReset(initFull);
    //     display.setDirectWriteState(false);
    // }

    midi.setInputChannel(1);
    midi.setNoteOffMode((noteOffType_t)database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_NOTE_OFF_TYPE_ID));
    bool runningStatus = database.read(DB_BLOCK_GLOBAL_SETTINGS, globalSettingsMIDI, MIDI_SETTING_RUNNING_STATUS_ID);
    midi.setRunningStatusState(runningStatus);

    leds.init();
    pads.init();
    encoders.init();

    leds.displayActiveNoteLEDs();

    #ifdef START_UP_ANIMATION
    startUpAnimation();
    #else
    sei();
    #endif

    display.setupHomeScreen();

    buttons.init();

    #ifndef DEBUG
    if (board.checkNewRevision())
        display.displayFirmwareUpdated();
    #endif

    //flush all data from encoders
    encoders.flush();

    while (1)
    {
        pads.update();
        buttons.update();
        encoders.update();
        display.update();

        #ifdef DEBUG
        vserial.update();
        #endif
    }

    return 0;
}