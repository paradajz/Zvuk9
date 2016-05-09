#include "LCD.h"

#ifdef LCD_H_
#include "../../Scales.h"
#include "../pads/Pads.h"

#define PROGRAM_PRESET_ROW  0
#define PAD_NOTE_ROW        1
#define PAD_V_XY_AT_ROW     2
#define XY_CC_PAD_ROW       3

#define X_COORDINATE_START  0
#define Y_COORDINATE_START  7

#define CC_X_START          0
#define CC_Y_START          7

#define VELOCITY_START      0
#define NOTES_START         5
#define AFTERTOUCH_START    15
#define MIDI_CHANNEL_START  16
#define PAD_NUMBER_START    15

typedef struct {

    uint8_t ccx;
    uint8_t ccy;
    uint8_t xpos;
    uint8_t ypos;
    uint8_t velocity;
    uint8_t aftertouch;
    uint8_t padNumber;
    uint8_t midiChannel;
    uint8_t program;
    uint8_t preset;

} lcdParametersRows;

#define SPACE_CHAR          32

char stringBuffer[MAX_TEXT_SIZE+1];

inline uint8_t getNumberOfDigits(int32_t number)  {

    if (number < 10)            return 1;
    if (number < 100)           return 2;
    if (number < 1000)          return 3;
    if (number < 10000)         return 4;
    if (number < 100000)        return 5;
    if (number < 1000000)       return 6;
    if (number < 10000000)      return 7;
    if (number < 100000000)     return 8;
    if (number < 1000000000)    return 9;

    return 10; //max size

}

inline void addNumberToCharArray(int32_t number, uint8_t &stringSize)  {

    char intToCharArray[7];
    itoa(number, intToCharArray, 10);
    stringSize += getNumberOfDigits(number);
    if (number < 0) stringSize++;
    strcat(stringBuffer, intToCharArray);
    stringBuffer[stringSize] = '\0';

}

inline void addSpaceToCharArray(uint8_t &stringSize, uint8_t numberOfSpaces) {

    memset(stringBuffer+stringSize, SPACE_CHAR, numberOfSpaces);
    stringSize += numberOfSpaces;
    stringBuffer[stringSize] = '\0';

}

void LCD::displayPadAmount(bool singlePad, uint8_t padNumber)  {

    uint8_t size = 0;

    strcpy_P(stringBuffer, singlePad ? padAmountSingle_string : padAmountAll_string);

    size = singlePad ? progmemCharArraySize(padAmountSingle_string) : progmemCharArraySize(padAmountAll_string);

    if (singlePad) { addSpaceToCharArray(size, 1); addNumberToCharArray(padNumber, size); }

    updateDisplay(2, message, 0, true, size);

}

void LCD::displayHelloMessage() {

    strcpy_P(stringBuffer, welcome_string);

    uint8_t charIndex = 0;
    while (stringBuffer[charIndex] != '\0')   {

        //write directly to screen
        lcd_putc(stringBuffer[charIndex]);
        wait(75);
        charIndex++;

    }

    strcpy(lastLCDLine_char[0], stringBuffer);
    wait(250);

}

//messages

void LCD::displayOutOfRange()    {

    strcpy_P(stringBuffer, outOfRange_string);
    updateDisplay(1, message, 0, true, progmemCharArraySize(outOfRange_string));

    strcpy_P(stringBuffer, emptyLine_string);
    updateDisplay(2, message, 0, true, progmemCharArraySize(emptyLine_string));

}

void LCD::displayModifierEnabled()   {

    strcpy_P(stringBuffer, modifierEnabled_string);
    updateDisplay(1, message, 0, true, progmemCharArraySize(modifierEnabled_string));

    strcpy_P(stringBuffer, emptyLine_string);
    updateDisplay(2, message, 0, true, progmemCharArraySize(emptyLine_string));

}

void LCD::displayMIDIchannelChange(uint8_t channel, bool _splitState, uint8_t padNumber) {

    uint8_t size = 0;
    strcpy_P(stringBuffer, midiChannelChange_string);
    size = progmemCharArraySize(midiChannelChange_string);
    addNumberToCharArray(channel, size);
    updateDisplay(1, message, 0, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCCchange(ccType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)   {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccArray[(uint8_t)type])));
    size = pgm_read_byte(&ccArray_sizes[type]);
    addNumberToCharArray(ccValue, size);
    updateDisplay(1, message, 0, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCurveChange(curveCoordinate_t coordinate, bool _splitState, curveType_t type, uint8_t padNumber)  {

    uint8_t size = 0;
    char tempBuffer[MAX_TEXT_SIZE];
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(curveCoordinateArray[(uint8_t)coordinate])));
    strcpy_P(tempBuffer, (char*)pgm_read_word(&(curveNameArray[type])));

    strcat(stringBuffer, tempBuffer);

    size = pgm_read_byte(&curveCoordinateArray_sizes[(uint8_t)coordinate]) + pgm_read_byte(&curveNameArray_sizes[type]);

    updateDisplay(1, message, 0, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayCClimitChange(ccLimitType_t type, bool _splitState, uint8_t ccValue, uint8_t padNumber)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, (char*)pgm_read_word(&(ccLimitArray[(uint8_t)type])));
    size = pgm_read_byte(&ccLimitArray_sizes[(uint8_t)type]);
    addNumberToCharArray(ccValue, size);

    updateDisplay(1, message, 0, true, size);

    displayPadAmount(_splitState, padNumber);

}

void LCD::displayOnOff(functionsOnOff_t messageType, bool _splitState, uint8_t functionState, uint8_t padNumber)  {

    uint8_t size = 0;

    switch(messageType) {

        case featureNotes:
        case featureAftertouch:
        case featureX:
        case featureY:
        if (!functionState) {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(offArray[messageType])));
            size = pgm_read_byte(&offArray_sizes[messageType]);

        } else {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(onArray[messageType])));
            size = pgm_read_byte(&onArray_sizes[messageType]);

        }

        updateDisplay(1, message, 0, true, size);

        displayPadAmount(_splitState, padNumber);
        break;

        case featureSplit:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(splitArray[_splitState])));
        size = pgm_read_byte(&splitArray_sizes[_splitState]);
        updateDisplay(1, message, 0, true, size);

        strcpy_P(stringBuffer, emptyLine_string);
        size = progmemCharArraySize(emptyLine_string);
        updateDisplay(2, message, 0, true, size);
        break;

        case featureAftertouchType:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(aftertouchTypeArray[functionState])));
        size = pgm_read_byte(&aftertouchTypeArray_sizes[functionState]);
        updateDisplay(1, message, 0, true, size);

        strcpy_P(stringBuffer, emptyLine_string);
        size = progmemCharArraySize(emptyLine_string);
        updateDisplay(2, message, 0, true, size);
        break;

        default:
        break;

    }

}

void LCD::displayNoteChange(changeOutput_t result, noteChangeType_t type, int8_t value) {

    uint8_t size = 0;
    char tempBuffer[MAX_TEXT_SIZE];

    switch(type)    {

        case tonicChange:
        switch(result)  {

            case outputChanged:
            case noChange:
            strcpy_P(stringBuffer, tonic_string);
            size = progmemCharArraySize(tonic_string);
            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[value])));
            size += pgm_read_byte(&noteNameArray_sizes[value]);
            strcat(stringBuffer, tempBuffer);
            updateDisplay(1, message, 0, true, size);
            strcpy_P(stringBuffer, emptyLine_string);
            size = progmemCharArraySize(emptyLine_string);
            updateDisplay(2, message, 0, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(1, message, 0, true, size);
            strcpy_P(stringBuffer, emptyLine_string);
            size = progmemCharArraySize(emptyLine_string);
            updateDisplay(2, message, 0, true, size);
            break;

            default:
            return;

        }
        break;

        case octaveChange:
        //always display active octave, ignore out of range here
        strcpy_P(stringBuffer, octave_string);
        size = progmemCharArraySize(octave_string);
        addNumberToCharArray(value, size);
        updateDisplay(1, message, 0, true, size);
        strcpy_P(stringBuffer, emptyLine_string);
        size = progmemCharArraySize(emptyLine_string);
        updateDisplay(2, message, 0, true, size);
        break;

        case noteShift:
        switch (result) {

            case outputChanged:
            strcpy_P(stringBuffer, value ? noteUp_string : noteDown_string);
            size = value ? progmemCharArraySize(noteUp_string) : progmemCharArraySize(noteDown_string);
            updateDisplay(1, message, 0, true, size);
            strcpy_P(stringBuffer, emptyLine_string);
            size = progmemCharArraySize(emptyLine_string);
            updateDisplay(2, message, 0, true, size);
            break;

            case outOfRange:
            strcpy_P(stringBuffer, outOfRange_string);
            size = progmemCharArraySize(outOfRange_string);
            updateDisplay(1, message, 0, true, size);
            strcpy_P(stringBuffer, emptyLine_string);
            size = progmemCharArraySize(emptyLine_string);
            updateDisplay(2, message, 0, true, size);
            break;

            default:
            return;

        }
        break;

        default:
        break;

    }

}

void LCD::displayEditModeNotAllowed(padEditModeResult_t errorType)   {

    uint8_t size = 0;

    switch (errorType)  {

        case notUserPreset:
        strcpy_P(stringBuffer, editModeEnterError0_string);
        size = progmemCharArraySize(editModeEnterError0_string);
        updateDisplay(1, message, 0, true, size);

        strcpy_P(stringBuffer, editModeEnterError1_string);
        size = progmemCharArraySize(editModeEnterError1_string);
        updateDisplay(2, message, 0, true, size);
        break;

        case padNotReleased:
        displayPadReleaseError(enterPadEditMode);
        break;

        default:
        return;

    }

}

void LCD::displayTransportControl(transportControl_t type, bool state)  {

    uint8_t size = 0;

    switch(type)    {

        case transportRecord:
        if (state) {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[2])));
            size = pgm_read_byte(&transportControlChangeArray_sizes[2]);

        } else {

            strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[3])));
            size = pgm_read_byte(&transportControlChangeArray_sizes[3]);

        }
        break;

        case transportPlay:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[0])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[0]);
        break;

        case transportStop:
        strcpy_P(stringBuffer, (char*)pgm_read_word(&(transportControlChangeArray[1])));
        size = pgm_read_byte(&transportControlChangeArray_sizes[1]);
        break;

    }

    updateDisplay(1, message, 0, true, size);
    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);
    updateDisplay(2, message, 0, true, size);

}

void LCD::displayMaxNotesSet()   {

    uint8_t size = 0;

    strcpy_P(stringBuffer, maxNotesSet0_string);
    size = progmemCharArraySize(maxNotesSet0_string);
    updateDisplay(1, message, 0, true, size);

    strcpy_P(stringBuffer, maxNotesSet1_string);
    size = progmemCharArraySize(maxNotesSet1_string);
    updateDisplay(2, message, 0, true, size);

}

void LCD::displayPadReleaseError(padReleaseError_t error)    {

    uint8_t size = 0;
    strcpy_P(stringBuffer, relasePad_string);
    size = progmemCharArraySize(relasePad_string);
    updateDisplay(1, message, 0, true, size);

    switch(error)   {

        case changeProgram:
        strcpy_P(stringBuffer, changeProgram_string);
        size = progmemCharArraySize(changeProgram_string);
        break;

        case changePreset:
        strcpy_P(stringBuffer, changePreset_string);
        size = progmemCharArraySize(changePreset_string);
        break;

        case changeCCnumber:
        strcpy_P(stringBuffer, changeCC_string);
        size = progmemCharArraySize(changeCC_string);
        break;

        case changeCClimit:
        strcpy_P(stringBuffer, changeCClimit_string);
        size = progmemCharArraySize(changeCClimit_string);
        break;

        case changeCurve:
        strcpy_P(stringBuffer, changeCurve_string);
        size = progmemCharArraySize(changeCurve_string);
        break;

        case enterPadEditMode:
        strcpy_P(stringBuffer, enterPadEditMode_string);
        size = progmemCharArraySize(enterPadEditMode_string);
        break;

        default:
        return;

    }   updateDisplay(2, message, 0, true, size);

}

void LCD::displayPadEditChangeParametersError()  {

    uint8_t size = 0;

    strcpy_P(stringBuffer, exitPadMode_string);
    size = progmemCharArraySize(exitPadMode_string);
    updateDisplay(1, message, 0, true, size);

    strcpy_P(stringBuffer, changeParameters_string);
    size = progmemCharArraySize(changeParameters_string);
    updateDisplay(2, message, 0, true, size);

}

//text

void LCD::displayProgramAndPreset(uint8_t program, uint8_t preset)   {

    //program and preset are displayed in single row
    uint8_t size = 0;
    char tempBuffer[MAX_TEXT_SIZE];
    strcpy_P(stringBuffer, program_string);
    size = progmemCharArraySize(program_string);

    addNumberToCharArray(program, size);
    addSpaceToCharArray(size, 1);

    if ((preset >= 0) && (preset < NUMBER_OF_PREDEFINED_SCALES))  {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[preset])));
        size += pgm_read_byte(&presetNameArray_sizes[preset]);
        strncat(stringBuffer, tempBuffer, pgm_read_byte(&presetNameArray_sizes[preset]));

    }   else {

        strcpy_P(tempBuffer, (char*)pgm_read_word(&(presetNameArray[NUMBER_OF_PREDEFINED_SCALES])));
        size += pgm_read_byte(&presetNameArray_sizes[NUMBER_OF_PREDEFINED_SCALES]);
        strcat(stringBuffer, tempBuffer);
        addSpaceToCharArray(size, 1);
        addNumberToCharArray((preset - NUMBER_OF_PREDEFINED_SCALES + 1), size);

    }

    updateDisplay(PROGRAM_PRESET_ROW, text, 0, true, size);

}

void LCD::displayVelocity(uint8_t velocity)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, velocity_string);
    size = progmemCharArraySize(velocity_string);
    addNumberToCharArray(velocity, size);

    if (velocity < 10)         addSpaceToCharArray(size, 2);
    else if (velocity < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(1, text, VELOCITY_START, false, size);

}

void LCD::displayAftertouch(uint8_t afterTouch)  {

    uint8_t size = 0;
    strcpy_P(stringBuffer, aftertouch_string);
    size = progmemCharArraySize(aftertouch_string);
    addNumberToCharArray(afterTouch, size);

    if (afterTouch < 10)       addSpaceToCharArray(size, 2);
    else if (afterTouch < 100) addSpaceToCharArray(size, 1);

    updateDisplay(1, text, AFTERTOUCH_START, false, size);

}

void LCD::displayXYposition(uint8_t position, ccType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0;

    switch(type)    {

        case ccTypeX:
        lcdCoordinate = X_COORDINATE_START;
        break;

        case ccTypeY:
        lcdCoordinate = Y_COORDINATE_START;
        break;

    }

    strcpy_P(stringBuffer, xyPosition_string);
    size = progmemCharArraySize(xyPosition_string);

    addNumberToCharArray(position, size);

    if (position < 10)         addSpaceToCharArray(size, 2);
    else if (position < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(3, text, lcdCoordinate, false, size);

}

void LCD::displayXYcc(uint8_t ccXY, ccType_t type)   {

    uint8_t lcdCoordinate = 0, size = 0;

    switch(type)    {

        case ccTypeX:
        strcpy_P(stringBuffer, xCCid_string);
        size = progmemCharArraySize(xCCid_string);
        lcdCoordinate = CC_X_START;
        break;

        case ccTypeY:
        strcpy_P(stringBuffer, yCCid_string);
        size = progmemCharArraySize(yCCid_string);
        lcdCoordinate = CC_Y_START;
        break;

    }

    addNumberToCharArray(ccXY, size);

    if (ccXY < 10)         addSpaceToCharArray(size, 2);
    else if (ccXY < 100)   addSpaceToCharArray(size, 1);

    updateDisplay(2, text, lcdCoordinate, false, size);

}

void LCD::displayActivePadNotes(uint8_t notes[], int8_t octaves[], uint8_t numberOfNotes, bool padEditMode)  {

    uint8_t size = 0;
    char tempBuffer[MAX_TEXT_SIZE];

    if (!numberOfNotes) {

        strcpy_P(stringBuffer, emptyLine_string);
        size = progmemCharArraySize(emptyLine_string);
        updateDisplay(1, text, 0, true, size);
        //we should also clear velocity if we aren't in pad edit mode
        if (!pads.editModeActive()) {

            strcpy_P(stringBuffer, velocityClear_string);
            size = progmemCharArraySize(velocityClear_string);
            if (padEditMode)    updateDisplay(2, text, 0, true, size);
            else                updateDisplay(2, text, NOTES_START, false, size);
        }

    }   else {

        for (int i=0; i<numberOfNotes; i++) {

            strcpy_P(tempBuffer, (char*)pgm_read_word(&(noteNameArray[notes[i]])));
            //start appending after first note (don't clear string)
            if (i) {

                strcat(stringBuffer, tempBuffer);
                size += pgm_read_byte(&noteNameArray_sizes[notes[i]]);

            } else {

                strcpy(stringBuffer, tempBuffer);
                size = pgm_read_byte(&noteNameArray_sizes[notes[i]]);

            }

            addNumberToCharArray(octaves[i], size);
            addSpaceToCharArray(size, 1);

        }

        if (padEditMode)    updateDisplay(1, text, 0, true, size);
        else                updateDisplay(1, text, NOTES_START, false, size);

    }

}

void LCD::clearVelocity()   {

    strcpy_P(stringBuffer, velocityClear_string);
    uint8_t size = progmemCharArraySize(velocityClear_string);
    updateDisplay(1, text, VELOCITY_START, false, size);

}

void LCD::clearAftertouch() {

    strcpy_P(stringBuffer, aftertouchClear_string);
    uint8_t size = progmemCharArraySize(aftertouchClear_string);
    updateDisplay(1, text, AFTERTOUCH_START, false, size);

}

void LCD::clearXYposition(ccType_t type)    {

    strcpy_P(stringBuffer, xyPositionClear_string);
    uint8_t size = progmemCharArraySize(xyPositionClear_string);

    switch(type)    {

        case ccTypeX:
        updateDisplay(3, text, X_COORDINATE_START, false, size);
        break;

        case ccTypeY:
        updateDisplay(3, text, Y_COORDINATE_START, false, size);
        break;

    }

}

void LCD::clearXYcc(ccType_t type)  {

    strcpy_P(stringBuffer, xyCCclear_string);
    uint8_t size = progmemCharArraySize(xyCCclear_string);

    switch(type)    {

        case ccTypeX:
        updateDisplay(2, text, CC_X_START, false, size);
        break;

        case ccTypeY:
        updateDisplay(2, text, CC_Y_START, false, size);
        break;

    }

}

void LCD::displayActiveOctave(int8_t octave)   {

    //used only in pad edit mode
    strcpy_P(stringBuffer, activeOctave_string);
    uint8_t size = progmemCharArraySize(activeOctave_string);
    addNumberToCharArray(octave, size);

    updateDisplay(2, text, 0, true, size);

}

void LCD::displayDFUmode()   {

    //this function writes directly to screen
    lcd_clrscr();

    strcpy_P(stringBuffer, dfu_string);
    lcd_puts(stringBuffer);

}

void LCD::displayPadEditMode(uint8_t padNumber)  {

    strcpy_P(stringBuffer, editingPad_string);
    uint8_t size = progmemCharArraySize(editingPad_string);
    addNumberToCharArray(padNumber, size);
    updateDisplay(0, text, 0, true, size);
    strcpy_P(stringBuffer, emptyLine_string);
    size = progmemCharArraySize(emptyLine_string);
    updateDisplay(1, text, 0, true, size);
    updateDisplay(2, text, 0, true, size);
    updateDisplay(3, text, 0, true, size);

}

void LCD::displayPad(uint8_t pad)   {

    strcpy_P(stringBuffer, padAmountSingle_string);
    uint8_t size = progmemCharArraySize(padAmountSingle_string);
    addSpaceToCharArray(size, 1);
    addNumberToCharArray(pad, size);

    updateDisplay(XY_CC_PAD_ROW, text, PAD_NUMBER_START, false, size);

}

void LCD::clearPad()    {

    strcpy_P(stringBuffer, padClear_string);
    updateDisplay(XY_CC_PAD_ROW, text, PAD_NUMBER_START, false, progmemCharArraySize(padClear_string));

}

void LCD::displayMIDIchannel(uint8_t channel)   {

    strcpy_P(stringBuffer, midiChannel_string);
    uint8_t size = progmemCharArraySize(midiChannel_string);
    addNumberToCharArray(channel, size);

    updateDisplay(2, text, MIDI_CHANNEL_START, false, size);

}

void LCD::clearPadEditMode()    {

    strcpy_P(stringBuffer, emptyLine_string);
    for (int i=0; i<NUMBER_OF_LCD_ROWS; i++)
        updateDisplay(i, text, 0, true, progmemCharArraySize(emptyLine_string));

}

void LCD::clearMIDIchannel()    {

    strcpy_P(stringBuffer, midiChannelClear_string);
    updateDisplay(2, text, MIDI_CHANNEL_START, false, progmemCharArraySize(midiChannelClear_string));

}

//menu
void LCD::displayServiceMenu(uint8_t startPosition)  {

    char tempBuffer[MAX_TEXT_SIZE];
    uint8_t size;

    strcpy_P(stringBuffer, (char*)pgm_read_word(&(menu_types[serviceMenu])));
    size = pgm_read_byte(&menu_types_sizes[serviceMenu]);
    updateDisplay(0, text, 0, true, size);

    for (int i=0; i<(NUMBER_OF_LCD_ROWS-1); i++)    {

        (!i) ? stringBuffer[0] = '>' : stringBuffer[0] = SPACE_CHAR;
        stringBuffer[1] = '\0';
        strcpy_P(tempBuffer, (char*)pgm_read_word(&(service_menu_options[i])));
        strcat(stringBuffer, tempBuffer);
        size = 1 + pgm_read_byte(&service_menu_options_sizes[i]);
        updateDisplay(i+1, text, 0, true, size);

    }

}

void LCD::changeMenuOption(menuType_t type, uint8_t option, uint8_t subOption) {

    //we can display up to three options/suboptions at the time
    uint8_t markerOption = (option > (NUMBER_OF_LCD_ROWS-2)) ? (NUMBER_OF_LCD_ROWS-2) : option;

    char tempBuffer[MAX_TEXT_SIZE];
    uint8_t size;

    uint8_t startPosition = (option > (NUMBER_OF_LCD_ROWS-2)) ? option-(NUMBER_OF_LCD_ROWS-2) : 0;

    switch(type)    {

        case serviceMenu:
        for (int i=0; i<(NUMBER_OF_LCD_ROWS-1); i++)    {

            if (i == markerOption)  stringBuffer[0] = '>';
            else                    stringBuffer[0] = SPACE_CHAR;

            stringBuffer[1] = '\0';
            strcpy_P(tempBuffer, (char*)pgm_read_word(&(service_menu_options[i+startPosition])));
            strcat(stringBuffer, tempBuffer);
            size = 1 + pgm_read_byte(&service_menu_options_sizes[i+startPosition]);
            updateDisplay(i+1, text, 0, true, size);

        }
        break;

        case userMenu:
        break;

        case noMenu:
        break;

    }

}

void LCD::selectMenuOption(menuType_t type, uint8_t option, uint8_t suboption)  {



}


//lcd update

void LCD::updateDisplay(uint8_t row, lcdTextType type, uint8_t startIndex, bool overwrite, uint8_t size)    {

    stringBuffer[size] = '\0'; //just a precaution

    switch(type)    {

        case text:
        display.displayText(row, stringBuffer, startIndex, overwrite);
        break;

        case message:
        display.displayMessage(row, stringBuffer);
        break;

        default:
        break;

    }

}

#endif