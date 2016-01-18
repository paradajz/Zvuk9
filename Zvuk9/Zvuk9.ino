//disable internal hardware serial object
#define HardwareSerial_h

#include <Wire.h>

#include "interface/lcd/LCD.h"
#include "interface/pads/Pads.h"
#include "interface/buttons/Buttons.h"
#include "Debug.h"
#include "eeprom/EEPROMsettings.h"
#include "interface/leds/LEDs.h"
#include "interface/encoders/Encoders.h"
#include "hardware/pins/HardwareIDs.h"
#include "Types.h"
#include "hardware/timer/TimerObject.h"
#include "hardware/reset/Reset.h"
#include <util/delay.h>
#include "midi/MIDI.h"

void startUpAnimation() {

    //slow down fading for effect
    leds.setFadeSpeed(1);

    ledIntensity tempLedStateArray[NUMBER_OF_LEDS];

    for (int i=0; i<NUMBER_OF_LEDS; i++)    {

        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledIntensityOff);

    }

    //turn all leds on slowly
    leds.allLEDsOn();
    newDelay(2700);

    //restore led states
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    newDelay(2500);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);

}

void handleTransportControl(uint8_t buttonNumber, buttonState state)  {

    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    transportControl type = transportStop;
    bool displayState = true;

    switch(buttonNumber)    {

        case BUTTON_TRANSPORT_PLAY:
        if (state == buttonPressed) {

            sysExArray[4] = 0x02;
            type = transportPlay;
            #if MODE_SERIAL
            Serial.println("Transport Control Play");
            #endif

        } else return;

        break;

        case BUTTON_TRANSPORT_STOP:
        if (state == buttonReleased)    {

            sysExArray[4] = 0x01;
            type = transportStop;
            #if MODE_SERIAL
            Serial.println("Transport Control Stop");
            #endif

        } else return;
        break;

        case BUTTON_TRANSPORT_RECORD:
        if (state == buttonPressed) {

            ledIntensity recordState = leds.getLEDstate(LED_TRANSPORT_RECORD);
            if (recordState == ledIntensityFull) {

                sysExArray[4] = 0x07;
                recordState = ledIntensityOff;
                #if MODE_SERIAL
                Serial.println("Transport Control Record Stop");
                #endif
                displayState = false;

            }   else if (recordState == ledIntensityOff) {

                sysExArray[4] = 0x06;
                recordState = ledIntensityFull;
                #if MODE_SERIAL
                Serial.println("Transport Control Record Start");
                #endif
                displayState = true;

            }

            type = transportRecord;
            leds.setLEDstate(LED_TRANSPORT_RECORD, recordState);

        } else return;

        break;

    }

    #if MODE_SERIAL < 1
    midi.sendSysEx(sysExArray, SYS_EX_ARRAY_SIZE);
    #endif

    lcDisplay.displayTransportControlMessage(type, displayState);

}

bool checkOctaveUpDownEnabled() {

    //HORRIBLE HACK!!!!!
    //please fix me someday

    //this function will check whether octave up/down is enabled
    //there are two scenarios when those two buttons should be disabled:
    //1) Pad edit mode is activated
    //2) Pad edit mode is disabled

    //in both cases octave/up down functions should be disabled until both buttons
    //have been released

    //check should start when pad edit mode is activated or deactivated and stopped when
    //both buttons are released

    static bool lastPadEditMode = false;
    bool currentPadEditMode = pads.getPadEditMode();
    static bool checkEnabled = false;
    bool returnValue = true;

    if (lastPadEditMode != currentPadEditMode) {

        lastPadEditMode = currentPadEditMode;
        checkEnabled = true;

    }

    if (checkEnabled) {

        //checking has been started
        if (!buttons.getButtonPressed(BUTTON_OCTAVE_DOWN) && !buttons.getButtonPressed(BUTTON_OCTAVE_UP))   {

            //both buttons have been released, stop with the checking
            //use returnValue as return variable since we don't want
            //to enable octave up/down immediately after release,
            //only when one of octave buttons has been pushed again
            if (!checkEnabled) returnValue = true; else returnValue = false;
            checkEnabled = false;

        }

    }

    //if checking is in progress, return false
    if (checkEnabled) return false;

    //if checking has been finished, return returnValue
    return returnValue;

}

void setLEDTonicStateEditMode(uint8_t pad) {

    uint8_t *notes = pads.getPadNotes(pad);

    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD];

    for (int i=0; i<NOTES_PER_PAD; i++) {

        tonicArray[i] = pads.getTonicFromNote(notes[i]);
        octaveArray[i] = pads.getOctaveFromNote(notes[i]);

    }

    //turn off all LEDs
    for (int i=0; i<MIDI_OCTAVE_RANGE; i++)
        leds.setTonicLEDstate((tonic)i, ledIntensityOff);

    //set dim led state for assigned notes on current pad
    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (tonicArray[i] != MIDI_OCTAVE_RANGE) {

            leds.setTonicLEDstate((tonic)i, ledIntensityDim);

        }

    }

    //set full led state for assigned notes on current pad if note matches current octave
    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (tonicArray[i] != MIDI_OCTAVE_RANGE) {

            if (octaveArray[i] == pads.getActiveOctave())
                leds.setTonicLEDstate((tonic)i, ledIntensityFull);

        }

    }

}

void padsFunctionOnOff(uint8_t buttonNumber, buttonState state)    {

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    functionsOnOff lcdMessageType;
    ledIntensity ledState = ledIntensityOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        pads.notesOnOff();
        lcdMessageType = featureNotes;
        ledNumber = LED_ON_OFF_NOTES;
        if (pads.getNoteSendState(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        pads.afterTouchOnOff();
        lcdMessageType = featureAftertouch;
        ledNumber = LED_ON_OFF_AFTERTOUCH;
        if (pads.getAfterTouchSendState(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_X:
        pads.xOnOff();
        lcdMessageType = featureX;
        ledNumber = LED_ON_OFF_X;
        if (pads.getXSendState(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_Y:
        pads.yOnOff();
        lcdMessageType = featureY;
        ledNumber = LED_ON_OFF_Y;
        if (pads.getYSendState(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        pads.splitFunctions();
        lcdMessageType = featureSplit;
        ledNumber = LED_ON_OFF_SPLIT;
        ledState = pads.getSplitStateLEDvalue();
        break;

        default:
        return;

    }

    leds.setLEDstate(ledNumber, ledState);
    lcDisplay.displayOnOffMessage(lcdMessageType, pads.getSplitState(), (ledState == ledIntensityFull), lastTouchedPad+1);

}

void padsOctaveUpDown(uint8_t direction, bool buttonState)    {

    if (buttons.getButtonPressed(BUTTON_OCTAVE_DOWN) && buttons.getButtonPressed(BUTTON_OCTAVE_UP))   {

        //enter pad edit mode

        if (pads.getActivePreset() >= NUMBER_OF_PREDEFINED_SCALES)    {

            pads.setPadEditMode(!pads.getPadEditMode());

            if (pads.getPadEditMode())  {

                //check if last touched pad is pressed
                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    checkOctaveUpDownEnabled();
                    pads.setPadEditMode(!pads.getPadEditMode());

                }   else {

                    //normally, this is called by callback from Pads,
                    //but on first occasion call it manually
                    setUpPadEditMode(pads.getLastTouchedPad());

                }

            }   else   clearPadEditMode();

        }   else {

                //used to "cheat" checkOctaveUpDownEnabled function
                //this will cause reset of function counters so that buttons are disabled
                //after LCD shows message that pad editing isn't allowed
                pads.setPadEditMode(!pads.getPadEditMode());
                checkOctaveUpDownEnabled();
                pads.setPadEditMode(!pads.getPadEditMode());
                lcDisplay.displayEditModeNotAllowed(noUserPreset);

            }

    }

    if (!pads.getPadEditMode() && checkOctaveUpDownEnabled())    {

        if (!buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))   {

            //shift entire octave

            //shift all notes up or down
            if (buttonState)    {

                //#if defined(KONTROLA_PCB) && defined (MODULE_LED)
                //if (direction)  leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull);
                //else            leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);
                //#endif

            }   else {

                changeOutput shiftResult = pads.shiftOctave(direction);
                int8_t activeOctave = pads.getActiveOctave();
                lcDisplay.displayNoteChange(shiftResult, octaveChange, activeOctave);

                #if defined(KONTROLA_PCB)
                    //if (direction) leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff);
                    //else           leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);
                #endif

            }

        }   else {

            //shift single note
            if (buttonState)    {

                if (pads.getPadPressed(pads.getLastTouchedPad()))   {

                    lcDisplay.displayEditModeNotAllowed(padNotReleased);
                    return;

                }

                //stop button is modifier
                //disable it on release
                buttons.pauseCallback(BUTTON_TRANSPORT_STOP);
                changeOutput shiftResult = pads.shiftNote(direction);
                lcDisplay.displayNoteChange(shiftResult, noteUpOrDown, direction);

            }

        }

    }   else if (pads.getPadEditMode() && checkOctaveUpDownEnabled()) {

        //do not shift octaves while pad is pressed
        if (pads.getPadPressed(pads.getLastTouchedPad()))   {

            lcDisplay.displayEditModeNotAllowed(padNotReleased);

        }   else if (!buttonState)   {

            pads.changeActiveOctave(direction);
            setLEDTonicStateEditMode(pads.getLastTouchedPad());
            lcDisplay.displayActiveOctave(pads.getActiveOctave());

        }

    }

}

void handleTonicPress(tonic _tonic) {

    if (!pads.getPadEditMode())   {

        //change tonic
        changeOutput result = pads.setTonic(_tonic);

        if (result == outputChanged)    {

            //tonic is changed, set correct LED states

            leds.tonicLEDsOff();    //turn off all tonic leds

            for (int i=0; i<tonicInvalid; i++)  {   //iterate over all tonics

                if (pads.noteActive((tonic)i))
                    leds.setTonicLEDstate((tonic)i, ledIntensityDim);

            }

        }

        tonic activeTonic = pads.getActiveTonic();
        lcDisplay.displayNoteChange(result, noteChange, activeTonic);

    }   else {

        //add note to pad
        lcDisplay.displayPadEditResult(pads.changePadNote(_tonic));
        displayActivePadNotes(pads.getLastTouchedPad());

    }


}

void displayActivePadNotes(uint8_t pad) {   //pad edit mode

    uint8_t *notes = pads.getPadNotes(pad);

    uint8_t tonicArray[NOTES_PER_PAD],
            octaveArray[NOTES_PER_PAD];

    for (int i=0; i<NOTES_PER_PAD; i++) {

        tonicArray[i] = pads.getTonicFromNote(notes[i]);
        octaveArray[i] = pads.getOctaveFromNote(notes[i]);

    }

    lcDisplay.displayActivePadNotes(tonicArray, octaveArray);
    setLEDTonicStateEditMode(pad);

}

void setUpPadEditMode(uint8_t pad) {

    lcDisplay.displayPadEditMode(pad + 1);
    displayActivePadNotes(pad);
    lcDisplay.displayActiveOctave(pads.getActiveOctave());

}

void clearPadEditMode() {

    lcDisplay.clearPadEditMode();
    pads.setPadEditMode(false);

    //set all active tonic leds from full to dim intensity
    for (int i=0; i<tonicInvalid; i++)  {

        if (leds.getTonicLEDstate((tonic)i) == ledIntensityFull)
            leds.setTonicLEDstate((tonic)i, ledIntensityDim);

    }

}

void handlePadPress(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY)  {

    //pad is pressed

    if (pads.getNoteSendEnabled(pad))   {

        uint8_t tonicArray[NOTES_PER_PAD],
        octaveArray[NOTES_PER_PAD];

        for (int i=0; i<numberOfNotes; i++) {

            tonicArray[i] = (uint8_t)pads.getTonicFromNote(notes[i]);
            leds.setTonicLEDstate((tonic)tonicArray[i], ledIntensityFull);
            octaveArray[i] = pads.getOctaveFromNote(notes[i]);

        }

        lcDisplay.updateNote(pad, tonicArray, octaveArray, numberOfNotes, velocity);

    }

    //always display ccx/ccy
    lcDisplay.setCCData(pad, ccX, ccY);

}

void handlePadRelease(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes)  {

    //we need to set LEDs back to dim states for released pad, but only if
    //some other pad with same active note isn't already pressed

    uint8_t *tempNotes;
    bool noteActive = false;

    for (int z=0; z<numberOfNotes; z++) {

        //iterate over every note on released pad

        noteActive = false;

        for (int i=0; i<NUMBER_OF_PADS; i++)    {

            if (!pads.getPadPressed(i)) continue; //skip released pad
            if (i == pad) continue; //skip current pad

            tempNotes = pads.getPadNotes(i);

            for (int j=0; j<NOTES_PER_PAD; j++) {

                if (pads.getTonicFromNote(tempNotes[j]) == pads.getTonicFromNote(notes[z])) {

                    noteActive = true;

                 }

            }

        }   if (!noteActive) leds.setTonicLEDstate(pads.getTonicFromNote((tonic)notes[z]), ledIntensityDim);

    }

} 

void setLCDAfterTouch(uint8_t pressure) {

    lcDisplay.updateAfterTouch(pressure);

}

void setLCDxyData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable) {

    if (xAvailable || yAvailable)
        lcDisplay.setXYData(pad, x, y, xAvailable, yAvailable);

}

void clearPadData(uint8_t pad) {

    lcDisplay.clearPadData();

}

void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps)   {

    if (pads.getPadEditMode()) return; //don't allow changing settings using encoders during pad edit mode
    for (int i=0; i<NUMBER_OF_PADS; i++)
        if (pads.getPadPressed(i)) {

            //disable encoders while pads are pressed
            return;

        }

    uint8_t lastTouchedPad = pads.getLastTouchedPad();
    splitState _splitState = pads.getSplitState();
    changeOutput result;
    curveType activeCurve = curveTypeInvalid;
    uint8_t value;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        if (buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))    {

            //stop button is modifier
            //disable it on release
            buttons.pauseCallback(BUTTON_TRANSPORT_STOP);
            //change midi channel in case it's pressed
            pads.changeMIDIchannel(direction);

            lcDisplay.displayMIDIchannelChange(pads.getMIDIchannel());

        } else {

            //preset is contained within program
            //save current preset and check if it's changed after program change
            bool programChanged = pads.changeActiveProgram(direction);

            if (programChanged)    {

                //program is changed
                uint8_t program = pads.getActiveProgram();
                lcDisplay.setProgram(program+1);

                //get last active preset on current program
                uint8_t currentPreset = pads.getActivePreset();

                //if (currentPreset != lastActivePreset)  {

                    bool presetChanged = pads.changeActivePreset_direct(currentPreset);

                    if (presetChanged) {

                        //preset is changed
                        //turn off all tonic leds
                        leds.tonicLEDsOff();
                        //turn on tonic leds
                        for (int i=0; i<tonicInvalid; i++)  {

                            if (pads.noteActive((tonic)i))
                            leds.setTonicLEDstate((tonic)i, ledIntensityDim);

                        }

                        //display preset on display
                        lcDisplay.setPreset(currentPreset);

                    }

                //}

            }

        }
        break;

        case PRESET_ENCODER:
        if (pads.changeActivePreset_incDec(direction)) {

            //preset is changed
            //turn off all tonic leds
            leds.tonicLEDsOff();
            //turn on tonic leds
            for (int i=0; i<tonicInvalid; i++)  {

                if (pads.noteActive((tonic)i))
                    leds.setTonicLEDstate((tonic)i, ledIntensityDim);

            }

            //display preset on display
            uint8_t _preset = pads.getActivePreset();
            lcDisplay.setPreset(_preset);

        }
        break;

        case X_CC_ENCODER:
        result = pads.changeCC(direction, ccTypeX, steps);
        lcDisplay.displayCCchangeMessage(ccTypeX, result, _splitState, pads.getPadCCvalue(ccTypeX, lastTouchedPad), lastTouchedPad+1);
        break;

        case Y_CC_ENCODER:
        result = pads.changeCC(direction, ccTypeY, steps);
        lcDisplay.displayCCchangeMessage(ccTypeY, result, _splitState, pads.getPadCCvalue(ccTypeY, lastTouchedPad), lastTouchedPad+1);
        break;

        case X_MAX_ENCODER:
        result = pads.changeXYlimits(direction, ccLimitTypeXmax, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmax, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeXmax, result, _splitState, value, lastTouchedPad+1);
        break;

        case X_MIN_ENCODER:
        result = pads.changeXYlimits(direction, ccLimitTypeXmin, steps);
        value = pads.getPadCClimitValue(ccTypeX, ccLimitTypeXmin, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeXmin, result, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MAX_ENCODER:
        result = pads.changeXYlimits(direction, ccLimitTypeYmax, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmax, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeYmax, result, _splitState, value, lastTouchedPad+1);
        break;

        case Y_MIN_ENCODER:
        result = pads.changeXYlimits(direction, ccLimitTypeYmin, steps);
        value = pads.getPadCClimitValue(ccTypeY, ccLimitTypeYmin, lastTouchedPad);
        lcDisplay.displayCClimitChangeMessage(ccLimitTypeYmin, result, _splitState, value, lastTouchedPad+1);
        break;

        case X_CURVE_ENCODER:
        result = pads.changeCurve(direction, curveCoordinateX);
        activeCurve = pads.getPadCurve(curveCoordinateX, lastTouchedPad);
        lcDisplay.displayCurveChangeMessage(curveCoordinateX, result, _splitState, activeCurve, lastTouchedPad+1);
        break;

        case Y_CURVE_ENCODER:
        result = pads.changeCurve(direction, curveCoordinateY);
        activeCurve = pads.getPadCurve(curveCoordinateY, lastTouchedPad);
        lcDisplay.displayCurveChangeMessage(curveCoordinateY, result, _splitState, activeCurve, lastTouchedPad+1);
        break;

    }

}

void setLEDstate(uint8_t ledNumber, ledIntensity state)   {

    leds.setLEDstate(ledNumber, state);

}

//callbacks

void configurePadCallbacks()    {

    pads.setHandleLEDstateCallback(setLEDstate);
    pads.setHandlePadPressCallback(handlePadPress);
    pads.setHandlePadReleaseCallback(handlePadRelease);
    pads.setHandleLCDxyCallback(setLCDxyData);
    pads.setHandleClearPadDataCallback(clearPadData);
    pads.setHandleLCDAfterTouchCallback(setLCDAfterTouch);
    pads.setHandlePadEditModeCallback(setUpPadEditMode);

}

void configureButtonCallbacks() {

    buttons.setHandleOnOffPress(padsFunctionOnOff);
    buttons.setHandleOctaveUpDownPress(padsOctaveUpDown);
    buttons.setHandleTonic(handleTonicPress);
    buttons.setHandleTransportControlCallback(handleTransportControl);

}

void configureEncoderCallbacks()    {

    encoders.setHandleEncoderCallback(handleEncoder);

}

void configureCallbacks()   {

    configurePadCallbacks();
    configureButtonCallbacks();
    configureEncoderCallbacks();

}


//init

void initHardware() {

    //do not change order of initialization!

    configuration.init();

    timers.init();

    midi.init();
    lcDisplay.init();
    leds.init();
    pads.init();
    buttons.init();
    encoders.init();

    //setup program/preset on display
    uint8_t activeProgram = pads.getActiveProgram();
    uint8_t activePreset = pads.getActivePreset();

    lcDisplay.setProgram(activeProgram+1);
    lcDisplay.setPreset(activePreset);

    //tonic leds
    leds.tonicLEDsOff();
    for (int i=0; i<tonicInvalid; i++)  {

        if (pads.noteActive((tonic)i))
            leds.setTonicLEDstate((tonic)i, ledIntensityDim);

    }

    startUpAnimation();

}

void setup()    {

    #if MODE_SERIAL
        Serial.begin(38400);
    #endif

    configureCallbacks();
    initHardware();

    //calibration
    #ifdef KONTROLA_BREADBOARD
        pads.setLowerXLimit(0, 422);
        pads.setLowerXLimit(1, 422);
        pads.setLowerXLimit(2, 422);
        pads.setLowerXLimit(3, 460);
        pads.setLowerXLimit(4, 422);
        pads.setLowerXLimit(5, 422);
        pads.setLowerXLimit(6, 422);
        pads.setLowerXLimit(7, 422);
        pads.setLowerXLimit(8, 422);

        pads.setUpperXLimit(0, 570);
        pads.setUpperXLimit(1, 570);
        pads.setUpperXLimit(2, 570);
        pads.setUpperXLimit(3, 600);
        pads.setUpperXLimit(4, 570);
        pads.setUpperXLimit(5, 570);
        pads.setUpperXLimit(6, 570);
        pads.setUpperXLimit(7, 565);
        pads.setUpperXLimit(8, 570);

        pads.setLowerYLimit(0, 450);
        pads.setLowerYLimit(1, 450);
        pads.setLowerYLimit(2, 450);
        pads.setLowerYLimit(3, 462);
        pads.setLowerYLimit(4, 450);
        pads.setLowerYLimit(5, 450);
        pads.setLowerYLimit(6, 450);
        pads.setLowerYLimit(7, 450);
        pads.setLowerYLimit(8, 450);

        pads.setUpperYLimit(0, 612);
        pads.setUpperYLimit(1, 612);
        pads.setUpperYLimit(2, 612);
        pads.setUpperYLimit(3, 615);
        pads.setUpperYLimit(4, 575);
        pads.setUpperYLimit(5, 612);
        pads.setUpperYLimit(6, 612);
        pads.setUpperYLimit(7, 610);
        pads.setUpperYLimit(8, 610);
    #elif defined KONTROLA_PCB
        pads.setLowerXLimit(0, 438);
        pads.setLowerXLimit(1, 442);
        pads.setLowerXLimit(2, 438);
        pads.setLowerXLimit(3, 438);
        pads.setLowerXLimit(4, 442);
        pads.setLowerXLimit(5, 438);
        pads.setLowerXLimit(6, 438);
        pads.setLowerXLimit(7, 438);
        pads.setLowerXLimit(8, 442);

        pads.setUpperXLimit(0, 604);
        pads.setUpperXLimit(1, 600);
        pads.setUpperXLimit(2, 604);
        pads.setUpperXLimit(3, 604);
        pads.setUpperXLimit(4, 604);
        pads.setUpperXLimit(5, 604);
        pads.setUpperXLimit(6, 604);
        pads.setUpperXLimit(7, 602);
        pads.setUpperXLimit(8, 604);

        pads.setLowerYLimit(0, 445);
        pads.setLowerYLimit(1, 445);
        pads.setLowerYLimit(2, 445);
        pads.setLowerYLimit(3, 445);
        pads.setLowerYLimit(4, 445);
        pads.setLowerYLimit(5, 445);
        pads.setLowerYLimit(6, 445);
        pads.setLowerYLimit(7, 445);
        pads.setLowerYLimit(8, 445);

        pads.setUpperYLimit(0, 595);
        pads.setUpperYLimit(1, 595);
        pads.setUpperYLimit(2, 595);
        pads.setUpperYLimit(3, 595);
        pads.setUpperYLimit(4, 595);
        pads.setUpperYLimit(5, 595);
        pads.setUpperYLimit(6, 595);
        pads.setUpperYLimit(7, 595);
        pads.setUpperYLimit(8, 590);
    #endif

}

void loop()     {

    pads.update();
    buttons.update();
    encoders.update();
    lcDisplay.update();

    if (checkReset(buttons.getButtonPressed(BUTTON_TRANSPORT_PLAY)))    {

        lcDisplay.displayUserMessage("Press STOP to reset");
        leds.allLEDsOn();
        lcDisplay.update();

        do {

            buttons.update();

        } while (!buttons.getButtonPressed(BUTTON_TRANSPORT_STOP));

        leds.allLEDsOff();
        newDelay(50);
        _reboot_Teensyduino_();

    }

}