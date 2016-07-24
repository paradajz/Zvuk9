/*

    Zvuk9 MIDI controller firmware
    Author: Igor Petrovic
    Ad Bit LLC 2014-2016

*/

#include "init/Init.h"

int main()    {

    globalInit();

    while(1) {

        pads.update();

        #ifdef MODULE_BUTTONS
        buttons.update();
        #endif

        #ifdef MODULE_ENCODERS
        encoders.update();
        #endif

        #ifdef MODULE_LCD
        display.update();
        #endif

        #if MODE_SERIAL > 0
        vserial.update();
        #endif

        #ifdef ENABLE_ASYNC_UPDATE
        //write to eeprom when all pads are released
        if (pads.allPadsReleased())
            configuration.update();
        #endif

    }

    return 0;

}