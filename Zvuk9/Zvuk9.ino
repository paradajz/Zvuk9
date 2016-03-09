//disable internal hardware serial object
#define HardwareSerial_h

#include "interface/lcd/LCD.h"
#include "interface/pads/Pads.h"
#include "interface/buttons/Buttons.h"
#include "Debug.h"
#include "eeprom/EEPROMsettings.h"
#include "interface/leds/LEDs.h"
#include "interface/encoders/Encoders.h"
#include "hardware/pins/Pins.h"
#include "Types.h"
#include "hardware/timer/TimerObject.h"
#include "hardware/reset/Reset.h"
#include <util/delay.h>
#include "midi/MIDI.h"

//define function prototypes
void startUpAnimation();
void setTonicLEDs();
void handleTransportControl(uint8_t buttonNumber, buttonState_t state);
bool checkOctaveUpDownEnabled();
void setLEDTonicStateEditMode(uint8_t pad);
void padsFunctionOnOff(uint8_t buttonNumber, buttonState_t state);
void padsOctaveUpDown(uint8_t direction, bool buttonState);
void handleTonicPress(tonic_t _tonic);
void displayActivePadNotes(uint8_t pad);
void setUpPadEditMode(uint8_t pad);
void clearPadEditMode();
void handlePadPress(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY);
void handlePadRelease(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes);
void setLCDAfterTouch(uint8_t pressure);
void setLCDxyData(uint8_t pad, uint8_t x, uint8_t y, bool xAvailable, bool yAvailable);
void clearPadData(uint8_t pad);
void handleEncoder(uint8_t encoderNumber, bool direction, uint8_t steps);
void setLEDstate(uint8_t ledNumber, ledIntensity_t state);

//implementation
void startUpAnimation() {

    //slow down fading for effect
    leds.setFadeSpeed(1);

    ledIntensity_t tempLedStateArray[NUMBER_OF_LEDS];

    for (int i=0; i<NUMBER_OF_LEDS; i++)    {

        //copy ledstates to temp field
        tempLedStateArray[i] = leds.getLEDstate(i);
        //turn all leds off
        leds.setLEDstate(i, ledIntensityOff);

    }

    //turn all leds on slowly
    leds.allLEDsOn();

    sei();

    lcDisplay.displayHelloMessage();
    newDelay(600);

    //restore led states
    for (int i=0; i<NUMBER_OF_LEDS; i++)
        leds.setLEDstate(i, tempLedStateArray[i]);

    newDelay(1500);

    //restore normal fade speed
    leds.setFadeSpeed(DEFAULT_FADE_SPEED);

}

void setTonicLEDs() {

    //first, turn off all tonic LEDs
    leds.tonicLEDsOff();
    for (int i=0; i<MIDI_NOTES; i++)  {

        //turn tonic LED on only if corresponding note is active
        if (pads.noteActive((tonic_t)i))
            leds.setNoteLEDstate((tonic_t)i, ledIntensityDim);

    }

}

void handleTransportControl(uint8_t buttonNumber, buttonState_t state)  {

    uint8_t sysExArray[] =  { 0xF0, 0x7F, 0x7F, 0x06, 0x00, 0xF7 }; //based on MIDI spec for transport control
    transportControl_t type = transportStop;
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

            ledIntensity_t recordState = leds.getLEDstate(LED_TRANSPORT_RECORD);
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

    for (int i=0; i<NOTES_PER_PAD; i++)
        Serial.println(tonicArray[i]);

    //turn off all LEDs
    for (int i=0; i<MIDI_OCTAVE_RANGE; i++)
        leds.setNoteLEDstate((tonic_t)i, ledIntensityOff);

    //set dim led state for assigned notes on current pad
    for (int i=0; i<NOTES_PER_PAD; i++)
        leds.setNoteLEDstate((tonic_t)tonicArray[i], ledIntensityDim);

    //set full led state for assigned notes on current pad if note matches current octave
    for (int i=0; i<NOTES_PER_PAD; i++) {

        if (tonicArray[i] != MIDI_OCTAVE_RANGE) {

            if (octaveArray[i] == pads.getActiveOctave())
                leds.setNoteLEDstate((tonic_t)tonicArray[i], ledIntensityFull);

        }

    }

}

void padsFunctionOnOff(uint8_t buttonNumber, buttonState_t state)    {

    //determine action based on pressed button

    uint8_t ledNumber = 0;
    functionsOnOff_t lcdMessageType;
    ledIntensity_t ledState = ledIntensityOff;
    uint8_t lastTouchedPad = pads.getLastTouchedPad();

    switch (buttonNumber)    {

        case BUTTON_ON_OFF_NOTES:
        pads.notesOnOff();
        lcdMessageType = featureNotes;
        ledNumber = LED_ON_OFF_NOTES;
        if (pads.getNoteSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_AFTERTOUCH:
        pads.aftertouchOnOff();
        lcdMessageType = featureAftertouch;
        ledNumber = LED_ON_OFF_AFTERTOUCH;
        if (pads.getAfterTouchSendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_X:
        pads.xOnOff();
        lcdMessageType = featureX;
        ledNumber = LED_ON_OFF_X;
        if (pads.getCCXsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_Y:
        pads.yOnOff();
        lcdMessageType = featureY;
        ledNumber = LED_ON_OFF_Y;
        if (pads.getCCYsendEnabled(lastTouchedPad)) ledState = ledIntensityFull; else ledState = ledIntensityOff;
        break;

        case BUTTON_ON_OFF_SPLIT:
        pads.setSplit();
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
            if (!buttonState)    {

                changeOutput_t shiftResult = pads.shiftOctave(direction);
                int8_t activeOctave = pads.getActiveOctave();
                lcDisplay.displayNoteChange(shiftResult, octaveChange, activeOctave);
                direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityOff) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityOff);

            }   else {

                //direction ? leds.setLEDstate(LED_OCTAVE_UP, ledIntensityFull) : leds.setLEDstate(LED_OCTAVE_DOWN, ledIntensityFull);

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
                changeOutput_t shiftResult = pads.shiftNote(direction);
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

void handleTonicPress(tonic_t _tonic) {

    if (!pads.getPadEditMode())   {

        //change tonic
        //FIXME: DO NOT ALLOW THIS WHILE PADS ARE PRESSED
        for (int i=0; i<NUMBER_OF_PADS; i++)
            if (pads.getPadPressed(i))  {
                
                //
                return;
                
            }

        changeOutput_t result = pads.setTonic(_tonic);

        if (result == outputChanged)    {

            setTonicLEDs();

        }

        tonic_t activeTonic = pads.getActiveTonic();
        lcDisplay.displayNoteChange(result, noteChange, activeTonic);

    }   else {

        //add note to pad
        lcDisplay.displayPadEditResult(pads.assignPadNote(_tonic));
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
    setTonicLEDs();

}

void handlePadPress(uint8_t pad, uint8_t notes[], uint8_t numberOfNotes, uint8_t velocity, uint8_t ccX, uint8_t ccY)  {

    //pad is pressed

    if (pads.getNoteSendEnabled(pad))   {

        uint8_t tonicArray[NOTES_PER_PAD],
        octaveArray[NOTES_PER_PAD];

        for (int i=0; i<numberOfNotes; i++) {

            tonicArray[i] = (uint8_t)pads.getTonicFromNote(notes[i]);
            leds.setNoteLEDstate((tonic_t)tonicArray[i], ledIntensityFull);
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

        }   if (!noteActive) leds.setNoteLEDstate(pads.getTonicFromNote((tonic_t)notes[z]), ledIntensityDim);

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
    splitState_t _splitState = pads.getSplitState();
    changeOutput_t result;
    curveType_t activeCurve = curveTypeInvalid;
    uint8_t value;
    int8_t activePreset;

    switch(encoderNumber)   {

        case PROGRAM_ENCODER:
        if (buttons.getButtonPressed(BUTTON_TRANSPORT_STOP))    {

            //stop button is modifier
            //disable it on release
            buttons.pauseCallback(BUTTON_TRANSPORT_STOP);
            //change midi channel in case it's pressed
            uint8_t midiChannel = pads.getMIDIchannel();

            if (direction) midiChannel++;
            else           midiChannel--;

            if (midiChannel < 1)  midiChannel = 16;
            if (midiChannel > 16) midiChannel = 1;
            pads.setMIDIchannel(midiChannel);

            lcDisplay.displayMIDIchannelChange(pads.getMIDIchannel());

        } else {

            int8_t activeProgram = pads.getActiveProgram();

            if (direction) activeProgram++; else activeProgram--;
            if (activeProgram == NUMBER_OF_PROGRAMS) activeProgram = 0;
            else if (activeProgram < 0) activeProgram = (NUMBER_OF_PROGRAMS-1);
            pads.setActiveProgram(activeProgram);
            lcDisplay.setProgram(activeProgram+1);

            //get last active preset on current program
            uint8_t currentPreset = pads.getActivePreset();

            //preset is changed
            setTonicLEDs();

            //display preset on display
            lcDisplay.setPreset(currentPreset);

        }
        break;

        case PRESET_ENCODER:
        activePreset = pads.getActivePreset();
        if (direction) activePreset++; else activePreset--;
        if (activePreset == (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES)) activePreset = 0;
        else if (activePreset < 0) activePreset = (NUMBER_OF_PREDEFINED_SCALES+NUMBER_OF_USER_SCALES-1);

        pads.setActivePreset(activePreset);

        setTonicLEDs();

        //display preset on display
        lcDisplay.setPreset(activePreset);
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

void setLEDstate(uint8_t ledNumber, ledIntensity_t state)   {

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

    //buttons use TWI, and we need to setup port expanders
    //the process requires interrupts to be enabled so init buttons first
    //and later reconfigure timers
    buttons.init();
    cli();

    configuration.init();
    timers.init();

    midi.init();
    lcDisplay.init();
    leds.init();
    pads.init();
    encoders.init();

    //setup program/preset on display
    uint8_t activeProgram = pads.getActiveProgram();
    uint8_t activePreset = pads.getActivePreset();

    lcDisplay.setProgram(activeProgram+1);
    lcDisplay.setPreset(activePreset);

    setTonicLEDs();

    #if START_UP_ANIMATION > 0
        startUpAnimation();
    #else
        sei();
    #endif

}

void setup()    {

    #if MODE_SERIAL
        Serial.begin(38400);
        _delay_ms(100);
    #endif

    configureCallbacks();
    initHardware();

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