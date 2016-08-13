#include "Pads.h"
#include <avr/cpufunc.h>
#include <util/delay.h>

Pads::Pads()  {

    //default constructor

    for (int i=0; i<MAX_PADS; i++)        {

        lastXMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastYMIDIvalue[i] = DEFAULT_XY_AT_VALUE;
        lastAftertouchValue[i] = DEFAULT_XY_AT_VALUE;
        padPressHistory_buffer[i] = 0;
        octaveShiftAmount[i] = 0;
        noteShiftAmount[i] = 0;

        for (int j=0; j<NOTES_PER_PAD; j++)
            padNote[i][j] = BLANK_NOTE;

    }

    activeProgram = -1;
    activeScale = -1;
    activeOctave = 0;
    octaveShiftPadBuffer = 0;
    scaleShiftPadBuffer = 0;
    calibrationEnabled = false;

    for (int i=0; i<PAD_NOTE_BUFFER_SIZE; i++)  {

        pad_note_timer_buffer[i]    = 0;
        velocity_buffer[i]          = 0;
        pad_buffer[i]               = 0;

    }

    note_buffer_head = 0;
    note_buffer_tail = 0;

}

void Pads::init()   {

    initHardware();
    getConfiguration();
    setupSysEx();

}

void Pads::setupSysEx() {

    uint16_t parameters, minValue, maxValue;

    //program block

    {
        parameters = 1;
        minValue = 0;
        maxValue = NUMBER_OF_PROGRAMS-1;

        sysExSection lastActiveProgram_section      = { parameters, minValue, maxValue };


        parameters = NUMBER_OF_PROGRAMS-1;
        minValue = 0;
        maxValue = NUMBER_OF_PROGRAMS-1;

        sysExSection lastActiveScale_section        = { parameters, minValue, maxValue };


        parameters = GLOBAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS;
        minValue = CUSTOM_VALUE_CHECK;
        maxValue = CUSTOM_VALUE_CHECK;

        sysExSection globalSettings_section         = { parameters, minValue, maxValue };


        parameters = LOCAL_PROGRAM_SETTINGS*NUMBER_OF_PROGRAMS*MAX_PADS;
        minValue = CUSTOM_VALUE_CHECK;
        maxValue = CUSTOM_VALUE_CHECK;

        sysExSection localSettings_section          = { parameters, minValue, maxValue };


        parameters = PREDEFINED_SCALE_PARAMETERS*NUMBER_OF_PREDEFINED_SCALES*NUMBER_OF_PROGRAMS;
        minValue = CUSTOM_VALUE_CHECK;
        maxValue = CUSTOM_VALUE_CHECK;

        sysExSection predefinedScaleSettings_section = { parameters, minValue, maxValue };


        sysExSection *programSectionsArray[] = {

            &lastActiveProgram_section,
            &lastActiveScale_section,
            &globalSettings_section,
            &localSettings_section,
            &predefinedScaleSettings_section

        };

        //define message for sysex configuration
        sysEx.addBlock(CONF_BLOCK_PROGRAM, PROGRAM_SECTIONS);

        for (int i=0; i<PROGRAM_SECTIONS; i++)   {

            //define subtype messages
            sysEx.addSection(CONF_BLOCK_PROGRAM, i, programSectionsArray[i]->numberOfParameters, programSectionsArray[i]->minValue, programSectionsArray[i]->maxValue);

        }

    }

    //user scales block

    {
        parameters = MAX_PADS*NOTES_PER_PAD*NUMBER_OF_USER_SCALES;
        minValue = 0;
        maxValue = 127;

        sysExSection padNotes_section               = { parameters, minValue, maxValue };

        sysEx.addSection(CONF_BLOCK_USER_SCALE, 0, padNotes_section.numberOfParameters, padNotes_section.minValue, padNotes_section.maxValue);
    }

    //pad calibration block

    {
        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection lowerPressure_section          = { parameters, minValue, maxValue };

        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection upperPressure_section          = { parameters, minValue, maxValue };

        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection lowerX_section                 = { parameters, minValue, maxValue };

        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection upperX_section                 = { parameters, minValue, maxValue };

        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection lowerY_section                 = { parameters, minValue, maxValue };

        parameters = MAX_PADS;
        minValue = 0;
        maxValue = 1023;

        sysExSection upperY_section                 = { parameters, minValue, maxValue };

        sysExSection *padCalibrationSectionArray[] = {

            &lowerPressure_section,
            &upperPressure_section,
            &lowerX_section,
            &upperX_section,
            &lowerY_section,
            &upperY_section

        };

        //define message for sysex configuration
        sysEx.addBlock(CONF_BLOCK_PAD_CALIBRATION, PAD_CALIBRATION_SECTIONS);

        for (int i=0; i<PAD_CALIBRATION_SECTIONS; i++)   {

            //define subtype messages
            sysEx.addSection(CONF_BLOCK_PAD_CALIBRATION, i, padCalibrationSectionArray[i]->numberOfParameters, padCalibrationSectionArray[i]->minValue, padCalibrationSectionArray[i]->maxValue);

        }

    }

    //midi block

    {
        parameters = MIDI_SETTINGS;
        minValue = CUSTOM_VALUE_CHECK;
        maxValue = CUSTOM_VALUE_CHECK;

        sysExSection midiSettings_section           = { parameters, minValue, maxValue };

        sysEx.addBlock(CONF_BLOCK_MIDI, 1);
        sysEx.addSection(CONF_BLOCK_MIDI, 0, midiSettings_section.numberOfParameters, midiSettings_section.minValue, midiSettings_section.maxValue);
    }

    //pressure settings block

    {
        parameters = 1;
        minValue = 0;
        maxValue = 2; //HARDCODED - FIXME

        sysExSection pressureSensitivity_section    = { parameters, minValue, maxValue };

        parameters = 1;
        minValue = 0;
        maxValue = 2; //HARDCODED - FIXME

        sysExSection pressureCurve_section          = { parameters, minValue, maxValue };

        //define message for sysex configuration
        sysEx.addBlock(CONF_BLOCK_PRESSURE_SETTINGS, PRESSURE_SETTINGS_SECTIONS);

        sysExSection *pressureSettingsSectionArray[] = {

            &pressureSensitivity_section,
            &pressureCurve_section

        };

        for (int i=0; i<PRESSURE_SETTINGS_SECTIONS; i++)   {

            //define subtype messages
            sysEx.addSection(CONF_BLOCK_PRESSURE_SETTINGS, i, pressureSettingsSectionArray[i]->numberOfParameters, pressureSettingsSectionArray[i]->minValue, pressureSettingsSectionArray[i]->maxValue);

        }

    }

}

Pads pads;