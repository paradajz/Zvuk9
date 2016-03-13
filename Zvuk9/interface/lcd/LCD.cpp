#include "LCD.h"
#include "menu/ServiceMenuStrings.h"

#define X_COORDINATE_START              5
#define Y_COORDINATE_START              10
#define AFTERTOUCH_START                15
#define CC_X_START                      0
#define CC_Y_START                      9

LCD::LCD()  {

    displayMessage = false;
    messageDisplayTime = 0;
    lcdUpdating = false;
    _clearPadData = false;
    keepMessage = false;

    lcd_init();

}

void LCD::init()    {

   lcd_clrscr();

   for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

       lastCharPosition[i] = 0;
       lcdLine[i].reserve(MAX_TEXT_LENGTH);
       lcdLine[i] = emptyLine;
       lcdLineMessage[i].reserve(MAX_TEXT_LENGTH);
       lcdLineMessage[i] = emptyLine;
       lineChange[i] = false;
       lineChangeBuffer[i] = false;
       lastLcdLine[i].reserve(MAX_TEXT_LENGTH);
       lastLcdLine[i] = emptyLine;
       scrollEnabled[i] = false;
       lcdLineScroll[i].reserve(MAX_TEXT_LENGTH);
       lcdLineScroll[i] = emptyLine;
       scrollDirection[i] = true;

   }

   program = -1;
   preset = -1;
   lastDisplayedPad = -1;
   lastScrollTime = 0;
   scrollIndex = 0;

   tempLine1.reserve(MAX_TEXT_LENGTH);
   tempLine2.reserve(MAX_TEXT_LENGTH);

   ccX = -1;
   ccY = -1;

   lcd_set_cursor(0, 0);

}

void LCD::displayHelloMessage() {

    lastLcdLine[0] = helloMessage;

    for (int i=0; i<(int)lastLcdLine[0].length(); i++)  {

        lcd_putc(lastLcdLine[0][i]);
        newDelay(75);

    } newDelay(250); lineChange[0] = true;

}

void LCD::setCCData(uint8_t pad, uint8_t x, uint8_t y)   {

    bool change = false;

    if (x != ccX) { ccX = x; change = true; }
    if (y != ccY) { ccY = y; change = true; }

    if (change) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccIDarray[ccTypeX])));
        tempLine1 = nameBuffer;
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccIDarray[ccTypeY])));
        tempLine2 = nameBuffer;

        tempLine1 += x;
        if (x < 10)       tempLine1 += "  ";
        else if (x < 100) tempLine1 += " ";
        tempLine1 += " ";

        tempLine2 += y;

        lcdLine[XY_ROW] = tempLine1 + tempLine2;
        expandLine(XY_ROW, regularLine);
        lineChange[XY_ROW] = true;

    }   lastLCDupdateTime = newMillis(); _clearPadData = false;

}

void LCD::setProgramAndPreset()    {

    //don't update line until we have both program and preset
    if (!((program != -1) && (preset != -1))) return;

    clearRow(PROGRAM_PRESET_ROW);

    lcdLine[PROGRAM_PRESET_ROW] = "P";
    lcdLine[PROGRAM_PRESET_ROW] += program;
    lcdLine[PROGRAM_PRESET_ROW] += " ";

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        lcdLine[PROGRAM_PRESET_ROW] += nameBuffer;

    }   else {

        lcdLine[PROGRAM_PRESET_ROW] += "User preset ";
        lcdLine[PROGRAM_PRESET_ROW] += (preset - NUMBER_OF_PREDEFINED_SCALES + 1);

    }

    expandLine(PROGRAM_PRESET_ROW, regularLine);
    lineChange[PROGRAM_PRESET_ROW] = true;
    lastLCDupdateTime = newMillis();

}

void LCD::setProgram(uint8_t _program) {

    program = _program;
    setProgramAndPreset();

}

void LCD::setPreset(uint8_t _preset) {

    preset = _preset;
    setProgramAndPreset();

}

void LCD::updateNote(uint8_t pad, uint8_t note[], uint8_t octave[], uint8_t numberOfNotes, uint8_t velocity)    {

    if (pad != lastDisplayedPad)    {

        //update notes only if current pad is different from last one

        lastDisplayedPad = pad;
        clearRow(PAD_NOTE_ROW);

        for (int i=0; i<numberOfNotes; i++) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[note[i]])));
            if (!i) lcdLine[PAD_NOTE_ROW] = nameBuffer;
            else lcdLine[PAD_NOTE_ROW] += nameBuffer;
            lcdLine[PAD_NOTE_ROW] += (octave[i]-2);
            lcdLine[PAD_NOTE_ROW] += " ";

        }

        if (lcdLine[PAD_NOTE_ROW].length() > NUMBER_OF_LCD_COLUMNS) {

            scrollEnabled[PAD_NOTE_ROW] = true;
            scrollDirection[PAD_NOTE_ROW] = true;
            scrollIndex = 0;

        }   else scrollEnabled[PAD_NOTE_ROW] = false;

        expandLine(PAD_NOTE_ROW, regularLine);
        lineChange[PAD_NOTE_ROW] = true;

    }

    if ((pad == lastDisplayedPad) || (lastDisplayedPad == -1))  {

        //update velocity only

        tempLine1 = "v";
        tempLine1 += velocity;
        if (velocity < 10)         tempLine1 += "  ";
        else if (velocity < 100)   tempLine1 += " ";

        for (int i=0; i<4; i++) lcdLine[PAD_V_XY_AT_ROW][i] = tempLine1[i];
        lineChange[PAD_V_XY_AT_ROW] = true;

    }

    lastDisplayedPad = pad;
    lastLCDupdateTime = newMillis();
    _clearPadData = false;

}

void LCD::clearPadData()    {

    _clearPadData = true;
    lastPadDataClearTime = newMillis();

}

void LCD::updateAfterTouch(uint8_t afterTouch)  {

    expandLine(PAD_V_XY_AT_ROW, regularLine);

    tempLine1 = "at";
    tempLine1 += afterTouch;
    if (afterTouch < 10)       tempLine1 += "  ";
    else if (afterTouch < 100) tempLine1 += " ";

    for (int i=0; i<5; i++)
        lcdLine[PAD_V_XY_AT_ROW][AFTERTOUCH_START+i] = tempLine1[i];

    lineChange[PAD_V_XY_AT_ROW] = true;
    lastLCDupdateTime = newMillis();
    _clearPadData = false;

}

void LCD::clearRow(uint8_t rowNumber)   {

    lcdLine[rowNumber] = emptyLine;

}

void LCD::setXYData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable)   {

    //lastDisplayedPad = pad;

    expandLine(PAD_V_XY_AT_ROW, regularLine);

    tempLine1 = "x";
    tempLine1 += x;
    if (x < 10)         tempLine1 += "  ";
    else if (x < 100)   tempLine1 += " ";

    tempLine2 = "y";
    tempLine2 += y;
    if (y < 10)         tempLine2 += "  ";
    else if (y < 100)   tempLine2 += " ";

    if (xAvailable)   {

        for (int i=0; i<4; i++)
            lcdLine[PAD_V_XY_AT_ROW][X_COORDINATE_START+i] = tempLine1[i];

    }   else {

        for (int i=0; i<4; i++)
            lcdLine[PAD_V_XY_AT_ROW][X_COORDINATE_START+i] = emptyLine[i];

    }

    if (yAvailable)   {

        for (int i=0; i<4; i++)
            lcdLine[PAD_V_XY_AT_ROW][Y_COORDINATE_START+i] = tempLine2[i];

    }   else {

        for (int i=0; i<4; i++)
            lcdLine[PAD_V_XY_AT_ROW][Y_COORDINATE_START+i] = emptyLine[i];

    }

    lineChange[PAD_V_XY_AT_ROW] = true;
    lastLCDupdateTime = newMillis();
    _clearPadData = false;

}

void LCD::displayOnOffMessage(functionsOnOff_t messageType, splitState_t _splitState, bool functionState, uint8_t padNumber)  {

    clearMessage();

    switch(messageType) {

        case featureNotes:
        if (_splitState == splitXYFunctions) {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offLocalArray[0])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onLocalArray[0])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "pad ";
            lcdLineMessage[2] += padNumber;

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[0])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[0])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "all pads";

        }
        break;

        case featureAftertouch:
        if (_splitState == splitXYFunctions) {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offLocalArray[1])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onLocalArray[1])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "pad ";
            lcdLineMessage[2] += padNumber;

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[1])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[1])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "all pads";

        }
        break;

        case featureX:
        if (_splitState == splitXYFunctions) {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offLocalArray[2])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onLocalArray[2])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "pad ";
            lcdLineMessage[2] += padNumber;

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[2])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[2])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "all pads";

        }
        break;

        case featureY:
        if (_splitState == splitXYFunctions) {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offLocalArray[3])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onLocalArray[3])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "pad ";
            lcdLineMessage[2] += padNumber;

        }

        else {

            if (!functionState) strcpy_P(nameBuffer, (char*)pgm_read_word(&(offGlobalArray[3])));
            else                strcpy_P(nameBuffer, (char*)pgm_read_word(&(onGlobalArray[3])));
            lcdLineMessage[1] = nameBuffer;
            lcdLineMessage[2] = "all pads";

        }
        break;

        case featureSplit:
        if (_splitState == splitOff)  {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[2])));
            lcdLineMessage[1] = nameBuffer;

        }   else if (_splitState == splitXY) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[0])));
            lcdLineMessage[1] = nameBuffer;

        }   else {

                strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[0])));
                lcdLineMessage[1] = nameBuffer;

                strcpy_P(nameBuffer, (char*)pgm_read_word(&(splitArray[1])));
                lcdLineMessage[2] = nameBuffer;

            }

        break;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayTransportControlMessage(transportControl_t type, bool state)  {

    clearMessage();

    switch(type)    {

        case transportRecord:
        if (state) strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[2])));
        else strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[3])));
        lcdLineMessage[1] = nameBuffer;
        break;

        case transportPlay:
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[0])));
        lcdLineMessage[1] = nameBuffer;
        break;

        case transportStop:
        strcpy_P(nameBuffer, (char*)pgm_read_word(&(transportControlChangeArray[1])));
        lcdLineMessage[1] = nameBuffer;
        break;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayCCchangeMessage(ccType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)   {

    clearMessage();

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));

    lcdLineMessage[1] = nameBuffer;
    lcdLineMessage[1] += ccValue;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    lcdLineMessage[2] = nameBuffer;

    if (!(_splitState == splitOff))
        lcdLineMessage[2] += padNumber; //local change

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayCClimitChangeMessage(ccLimitType_t type, splitState_t _splitState, uint8_t ccValue, uint8_t padNumber)  {

    clearMessage();

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));

    lcdLineMessage[1] = nameBuffer;
    lcdLineMessage[1] += ccValue;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    lcdLineMessage[2] = nameBuffer;

    if (!(_splitState == splitOff))
        lcdLineMessage[2] += padNumber; //local change

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayCurveChangeMessage(curveCoordinate_t coordinate, splitState_t _splitState, curveType_t type, uint8_t padNumber)  {

    clearMessage();

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));

    lcdLineMessage[1] = nameBuffer;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(curveNameArray[type])));
    lcdLineMessage[1] += nameBuffer;

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padAmountArray[(uint8_t)_splitState])));
    lcdLineMessage[2] = nameBuffer;

    if (!(_splitState == splitOff))
        lcdLineMessage[2] += padNumber; //local change

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayNoteChange(changeOutput_t result, changeType_t type, int8_t value) {

    clearMessage();

    if (type == noteUpOrDown)  {

        if (result == outputChanged)
            lcdLineMessage[1] = value ? "One note up" : "One note down";
        else if (result == outOfRange)  {

            lcdLineMessage[1] = "Out of range!";

        }   else if (result == notAllowed)  {

            lcdLineMessage[1] = "Switch to";
            lcdLineMessage[2] = "predefined preset";

        }

    }   else {

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lcdLineMessage[i] = emptyLine;

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[type])));
        lcdLineMessage[1] = nameBuffer;
        if (type == octaveChange) lcdLineMessage[1] += (value - 2);
        else if (type == noteChange) {

            strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            lcdLineMessage[1] += nameBuffer;

        }

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayMIDIchannelChange(uint8_t channel) {

    clearMessage();

    strcpy_P(nameBuffer, (char*)pgm_read_word(&(changeTypeArray[5])));
    lcdLineMessage[1] = nameBuffer;
    lcdLineMessage[1] += channel;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    expandLine(i, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayOctaveChange(uint8_t octave)   {

    clearMessage();

    lcdLineMessage[1] = "Octave set to " + octave;

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::update()  {

    if (bitRead(ADCSRA, ADSC)) return;  //don't mess with LCD while ADC conversion is in progress

    checkClearScreen();
    if ((newMillis() - lastLCDupdateTime < LCD_REFRESH_TIME) && !_clearPadData) return;

        if (displayMessage)   {

            for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

                for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                    if (lcdLineMessage[i][j] != lastLcdLine[i][j])  {

                        lcd_set_cursor(j, i);
                        lcd_putc(lcdLineMessage[i][j]);

                    }

                }

                lastLcdLine[i] = lcdLineMessage[i];

            }   displayMessage = false; messageActivated = true;

        }

        if (messageActivated)   {

            if ((!((newMillis() - messageDisplayTime) > LCD_MESSAGE_DURATION)) || keepMessage) return;
            for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lineChange[i] = true;

            clearMessage();

        }

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

            if (scrollEnabled[i])   {

                if ((newMillis() - lastScrollTime) > LCD_SCROLL_TIME)    {

                    lcdLineScroll[i] = lcdLine[i].substring(scrollIndex, MAX_TEXT_LENGTH);
                    expandLine(i, scrollLine);

                    for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                        if (lastLcdLine[i][j] != lcdLineScroll[i][j])   {

                            lcd_set_cursor(j, i);
                            lcd_putc(lcdLineScroll[i][j]);

                        }

                    }

                    lastScrollTime = newMillis();

                    if (scrollDirection[i])    {

                        if (((lcdLine[i].length()-1) - scrollIndex) > NUMBER_OF_LCD_COLUMNS) {

                            scrollIndex++;

                        }   else scrollDirection[i] = false;

                    }   else {

                        scrollIndex--;
                        if (scrollIndex < 0) { scrollDirection[i] = true; scrollIndex = 0; }

                    }

                    lastLcdLine[i] = lcdLineScroll[i];

                }

            }

            else if (lineChange[i])  {

                for (int j=0; j<NUMBER_OF_LCD_COLUMNS; j++) {

                    if (lcdLine[i][j] != lastLcdLine[i][j]) {

                        lcd_set_cursor(j, i);
                        lcd_putc(lcdLine[i][j]);

                    }

                }

                lastLcdLine[i] = lcdLine[i];

            }

            lineChange[i] = false;

        }   lastRefreshTime = newMillis();

}

void LCD::expandLine(uint8_t lineNumber, lcdLineType_t lineType)    {

    switch (lineType)   {

        case regularLine:
        if (lcdLine[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)
        do {

            lcdLine[lineNumber] += " ";

        }   while (lcdLine[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

        case messageLine:
        if (lcdLineMessage[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)

        do {

            lcdLineMessage[lineNumber] += " ";

        }   while (lcdLineMessage[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

        case scrollLine:
        if (lcdLineScroll[lineNumber].length() < NUMBER_OF_LCD_COLUMNS)

        do {

            lcdLineScroll[lineNumber] += " ";

        }   while (lcdLineScroll[lineNumber].length() < NUMBER_OF_LCD_COLUMNS);
        break;

    }

}

void LCD::displayPadEditMode(uint8_t padNumber)  {

    clearRow(0);
    clearRow(1);
    clearRow(2);
    clearRow(3);

    lcdLine[0] = "Editing pad ";
    lcdLine[0] += padNumber;
    expandLine(0, regularLine);

    expandLine(1, regularLine);
    expandLine(2, regularLine);
    expandLine(3, regularLine);
    lineChange[0] = true;
    lineChange[1] = true;
    lineChange[2] = true;
    lineChange[3] = true;

}

void LCD::clearPadEditMode()    {

    setProgramAndPreset();
    clearRow(1);
    clearRow(2);
    clearRow(3);

    scrollIndex = 0;

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) {

        scrollDirection[i] = true;
        scrollEnabled[i] = false;

    }

    lineChange[1] = true;
    lineChange[2] = true;
    lineChange[3] = true;

}

void LCD::displayEditModeNotAllowed(padEditError_t errorType)   {

    uint8_t errorIndex = 0;

    switch (errorType)  {

        case noUserPreset:
        errorIndex = 0;
        break;

        case padNotReleased:
        errorIndex = 2;
        break;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lcdLineMessage[i] = emptyLine;
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex])));
    lcdLineMessage[1] = nameBuffer; 
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(padEditErrorArray[errorIndex+1])));
    lcdLineMessage[2] = nameBuffer;

    expandLine(1, messageLine);
    expandLine(2, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;

}

void LCD::displayActiveOctave(int8_t octave)   {

    lcdLine[2] = "Active octave: ";
    lcdLine[2] += octave;

    lineChange[2] = true;
    expandLine(2, regularLine);

}

void LCD::displayActivePadNotes(uint8_t notes[], uint8_t octaves[], uint8_t numberOfNotes)  {

    clearRow(1);

    for (int i=0; i<numberOfNotes; i++) {

        strcpy_P(nameBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
        if (!i) lcdLine[1] = nameBuffer;
        else lcdLine[1] += nameBuffer;
        lcdLine[1] += (octaves[i]-2);
        lcdLine[1] += " ";

    }

    if (lcdLine[PAD_NOTE_ROW].length() > NUMBER_OF_LCD_COLUMNS)   {

        //scrolling logic
        scrollEnabled[1] = true;

    }   else {

        scrollEnabled[1] = false;
        scrollIndex = 0;
        scrollDirection[1] = true;

    }

    lineChange[1] = true;
    expandLine(1, regularLine);

}

void LCD::displayPadEditResult(changeOutput_t result)    {

    if (result == outOfRange)   {

        for (int i=0; i<NUMBER_OF_LCD_ROWS; i++) lcdLineMessage[i] = emptyLine;

        lcdLineMessage[1] = "Maximum number of";
        lcdLineMessage[2] = "notes set!";

        messageDisplayTime = newMillis();
        displayMessage = true;

    }

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        expandLine(i, messageLine);

}

void LCD::displayUserMessage(String message, bool stayOn)  {

    lcdLineMessage[1] = message;
    expandLine(1, messageLine);

    messageDisplayTime = newMillis();
    displayMessage = true;
    keepMessage = stayOn;

}

void LCD::clearMessage()    {

    messageActivated = false;
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)    {

        lcdLineMessage[i] = emptyLine;

    }

    keepMessage = false;

}

bool LCD::checkClearScreen()    {

    bool clearScreen = false;

    if (_clearPadData)  {

        if (lastPadDataClearTime - lastLCDupdateTime > LCD_PAD_DATA_CLEAR_DELAY) {

            clearScreen = true;

        }   else

        if (newMillis() - lastPadDataClearTime > LCD_PAD_DATA_CLEAR_DELAY) {

            clearScreen = true;

        }

    }

    if (clearScreen)    {

        clearRow(PAD_NOTE_ROW);
        clearRow(PAD_V_XY_AT_ROW);
        clearRow(XY_ROW);

        lineChange[PAD_NOTE_ROW] = true;
        lineChange[PAD_V_XY_AT_ROW] = true;
        lineChange[XY_ROW] = true;
        scrollEnabled[PAD_NOTE_ROW] = false;
        scrollDirection[PAD_NOTE_ROW] = true;
        scrollIndex = 0;

        ccX = -1;
        ccY = -1;
        lastDisplayedPad = -1;

        _clearPadData = false;
        return true;

    }   return false;

}

void LCD::displayServiceMenu()	{

    lcdLine[0] = "Service menu";
    lcdLine[1] = ">";
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[0])));
    lcdLine[1] += nameBuffer;
    strcpy_P(nameBuffer, (char*)pgm_read_word(&(service_menu_options[1])));
    lcdLine[2] = nameBuffer;
    lcdLine[3] = emptyLine;

    expandLine(0, regularLine);
    expandLine(1, regularLine);
    expandLine(2, regularLine);

    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        lineChange[i] = true;

}

LCD lcDisplay;
