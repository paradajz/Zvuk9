/*

Define custom data types.

*/

#ifndef TYPES_H_
#define TYPES_H_

enum ccLimitType {

    ccLimitTypeXmin = 0x00,
    ccLimitTypeXmax = 0x01,
    ccLimitTypeYmin = 0x02,
    ccLimitTypeYmax = 0x03

};

enum ccType {

    ccTypeX = 0x00,
    ccTypeY = 0x01

};

enum ledIntensity   {

    ledIntensityOff = 0x00,
    ledIntensityDim = 70,
    ledIntensityFull = 255

};

enum splitState {

    splitOff = 0x00,
    splitXY = 0x01,
    splitXYFunctions = 0x02,
    splitEnd

};

enum functionsOnOff    {

    featureNotes,
    featureAftertouch,
    featureX,
    featureY,
    featureSplit,

};

enum transportControl {

    transportStop,
    transportPlay,
    transportRecord

};

enum changeOutput {

    noChange,
    outputChanged,
    outOfRange,
    notAllowed

};

enum changeType {

    noteChange,
    octaveChange,
    noteUpOrDown,
    ccChange,
    ccLimitChange,
    curveChange,
    midiChannelChange

};

enum coordinateType {

    coordinateZ,
    coordinateX,
    coordinateY

};

enum padEditError   {

    noUserPreset,
    padNotReleased

};

enum lcdLineType {

    regularLine,
    messageLine,
    scrollLine

};

enum pressureType   {

    pressureVelocity,
    pressureAfterTouch

};

enum encoderPosition {

    encMoveLeft = 1,
    encMoveRight = 127,
    encStopped = 0

};

enum scaleType {

    fiveNoteScale = 5,
    sevenNoteScale = 7,
    userScale

};

enum tonic {

    tonicC,
    tonicCSharp,
    tonicD,
    tonicDSharp,
    tonicE,
    tonicF,
    tonicFSharp,
    tonicG,
    tonicGSharp,
    tonicA,
    tonicASharp,
    tonicB,
    tonicInvalid

};

enum curveCoordinate  {

    curveCoordinateX = 0x00,
    curveCoordinateY = 0x01,
    curveCoordinateInvalid

};

enum curveType {

    curveTypeLinear,
    curveTypeWideMiddle,
    curveTypeWideEnds,
    curveTypeInvalid

};

enum buttonState    {

    buttonReleased,
    buttonPressed

};

#endif /* TYPES_H_ */