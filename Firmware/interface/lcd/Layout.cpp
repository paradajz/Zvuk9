#include "LCD.h"

#ifdef LCD_H_

void LCD::setupLCDlayout()  {

    {
        lcdElements.programAndScale.row = 0;
        lcdElements.programAndScale.startIndex = 0;
    }

    {
        lcdElements.padNumber.row = 1;
        lcdElements.padNumber.startIndex = 0;

        lcdElements.notes.row = 1;
        lcdElements.notes.startIndex = 5;

        lcdElements.noteShiftLevel.row = 1;
        lcdElements.noteShiftLevel.startIndex = 17;
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

        //pad edit mode only
        lcdElements.activeOctave.row = 2;
        lcdElements.activeOctave.startIndex = 0;
    }

    {
        lcdElements.ccx.row = 3;
        lcdElements.ccx.startIndex = 0;

        lcdElements.ccy.row = 3;
        lcdElements.ccy.startIndex = 7;

        lcdElements.midiChannel.row = 3;
        lcdElements.midiChannel.startIndex = 16;
    }

    {
        lcdElements.messageText1.row = 1;
        lcdElements.messageText1.startIndex = 0;

        lcdElements.messageText2.row = 2;
        lcdElements.messageText2.startIndex = 0;
    }

    {
        //used during pad calibration
        lcdElements.padCalibration.row = 0;
        lcdElements.padCalibration.startIndex = 0;
    }

}

#endif