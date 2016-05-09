/*

Define custom data types.

*/

#ifndef TYPES_H_
#define TYPES_H_

typedef enum {

    ccLimitTypeXmin,
    ccLimitTypeXmax,
    ccLimitTypeYmin,
    ccLimitTypeYmax

} ccLimitType_t;

typedef enum {

    ccTypeX,
    ccTypeY

} ccType_t;

typedef enum {

    ledIntensityOff,
    ledIntensityDim,
    ledIntensityFull

} ledIntensity_t;

typedef enum {

    featureNotes,
    featureAftertouch,
    featureX,
    featureY,
    featureSplit,
    featureAftertouchType

} functionsOnOff_t;

typedef enum {

    transportStop,
    transportPlay,
    transportRecord

} transportControl_t;

typedef enum {

    noChange,
    outputChanged,
    outOfRange,
    overflow,
    notAllowed

} changeOutput_t;

typedef enum {

    tonicChange,
    octaveChange,
    noteShift,
    noteAdded,
    noteRemoved

} noteChangeType_t;

typedef enum {

    coordinateZ,
    coordinateX,
    coordinateY

} coordinateType_t;

typedef enum {

    notUserPreset,
    padNotReleased,
    padEditEnter,
    padEditExit

} padEditModeResult_t;

typedef enum {

    pressureVelocity,
    pressureAftertouch

} pressureType_t;

typedef enum {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

} scaleType_t;

typedef enum {

    curveCoordinateX,
    curveCoordinateY,
    curveCoordinateInvalid

} curveCoordinate_t;

typedef enum {

    curveTypeLinear,
    curveTypeWideMiddle,
    curveTypeWideEnds,
    curveTypeInvalid

} curveType_t;

typedef enum {

    C,
    C_SHARP,
    D,
    D_SHARP,
    E,
    F,
    F_SHARP,
    G,
    G_SHARP,
    A,
    A_SHARP,
    B,
    MIDI_NOTES

} note_t;

typedef enum {

    scaleNaturalMinor,
    scaleMelodicMinor,
    scaleHarmonicMinor,
    scaleMajor,
    scaleHarmonicMajor,
    scaleMinorPentatonic,
    scaleMajorPentatonic,
    scaleUser

} scale_t;

typedef enum {

    userMenu,
    serviceMenu,
    noMenu

} menuType_t;

typedef enum {

    text,
    message

} lcdTextType;

typedef enum {

    changeProgram,
    changePreset,
    changeCCnumber,
    changeCClimit,
    changeCurve,
    enterPadEditMode

} padReleaseError_t;

typedef enum {

    noMessage,
    showMessage,
    messageDisplayed,
    clearMessage

} messageStatus_t;

typedef enum {

    aftertouchPoly,
    aftertouchChannel,
    AFTERTOUCH_TYPES

} aftertouchType_t;

enum midiMessageType_t {

    midiMessageNoteOff              = 0x80, //Note Off
    midiMessageNoteOn               = 0x90, //Note On
    midiMessageControlChange        = 0xB0, //Control Change / Channel Mode
    midiMessageProgramChange        = 0xC0, //Program Change
    midiMessageAfterTouchChannel    = 0xD0, //Channel (monophonic) AfterTouch
    midiMessageAfterTouchPoly       = 0xA0, //Polyphonic AfterTouch
    midiMessagePitchBend            = 0xE0, //Pitch Bend
    midiMessageSystemExclusive      = 0xF0, //System Exclusive
    midiMessageTimeCodeQuarterFrame = 0xF1, //System Common - MIDI Time Code Quarter Frame
    midiMessageSongPosition         = 0xF2, //System Common - Song Position Pointer
    midiMessageSongSelect           = 0xF3, //System Common - Song Select
    midiMessageTuneRequest          = 0xF6, //System Common - Tune Request
    midiMessageClock                = 0xF8, //System Real Time - Timing Clock
    midiMessageStart                = 0xFA, //System Real Time - Start
    midiMessageContinue             = 0xFB, //System Real Time - Continue
    midiMessageStop                 = 0xFC, //System Real Time - Stop
    midiMessageActiveSensing        = 0xFE, //System Real Time - Active Sensing
    midiMessageSystemReset          = 0xFF, //System Real Time - System Reset
    midiMessageInvalidType          = 0x00  //For notifying errors

};

enum usbMIDIsystemCin_t {

    //normally, usb midi cin (cable index number) is just midiMessageType shifted left by four bytes
    //system common/exclusive messages have a bit convulted pattern so they're grouped in different enum
    sysCommon1byteCin = 0x50,
    sysCommon2byteCin = 0x20,
    sysCommon3byteCin = 0x30,
    sysExStartCin = 0x40,
    sysExStop1byteCin = sysCommon1byteCin,
    sysExStop2byteCin = 0x60,
    sysExStop3byteCin = 0x70

};

enum midiInterfaceType_t   {

    dinInterface,
    usbInterface

};

enum midiFilterMode_t {

    Off,                //thru disabled (nothing passes through)
    Full,               //fully enabled Thru (every incoming message is sent back)
    SameChannel,        //only the messages on the Input Channel will be sent back
    DifferentChannel    //all the messages but the ones on the Input Channel will be sent back

};

#endif