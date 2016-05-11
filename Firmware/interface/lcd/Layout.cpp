#include "LCD.h"

#ifdef LCD_H_

void LCD::setupLCDlayout()  {

    {
        lcdElements.programAndPreset.row = 0;
        lcdElements.programAndPreset.startIndex = 0;
    }

    {
        lcdElements.padNumber.row = 1;
        lcdElements.padNumber.startIndex = 0;

        lcdElements.notes.row = 1;
        lcdElements.notes.startIndex = 5;
    }

    {
        lcdElements.velocity.row = 2;
        lcdElements.velocity.startIndex = 0;

        lcdElements.xpos.row = 2;
        lcdElements.xpos.startIndex = 5;

        lcdElements.ypos.row = 2;
        lcdElements.ypos.startIndex = 10;

        lcdElements.aftertouch.row = 2;
        lcdElements.aftertouch.startIndex = 15;
    }

    {
        lcdElements.ccx.row = 3;
        lcdElements.ccx.startIndex = 0;

        lcdElements.ccy.row = 3;
        lcdElements.ccy.startIndex = 7;

        lcdElements.midiChannel.row = 3;
        lcdElements.midiChannel.startIndex = 16;
    }

}

#endif