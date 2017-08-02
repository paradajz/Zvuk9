#pragma once

#include "Variables.h"
#include "map/Map.h"
#include "lcd/lcd.h"
#include "pins/Pins.h"
#include "Import.h"
#include "DataTypes.h"

///
/// \brief Hardcoded board revision.
/// @{
///
#define HARDWARE_VERSION_MAJOR      3
#define HARDWARE_VERSION_MINOR      0
#define HARDWARE_VERSION_REVISION   0
/// @}

//function prototypes
inline void setAnalogPin(uint8_t muxNumber) __attribute__((always_inline));
inline void nextMuxInput() __attribute__((always_inline));
inline void ledRowsOff() __attribute__((always_inline));
inline void ledRowOn(uint8_t rowNumber, uint8_t intensity) __attribute__((always_inline));
inline void checkLEDs() __attribute__((always_inline));
inline void activateInputColumn(uint8_t column) __attribute__((always_inline));
inline void storeDigitalIn(uint8_t column) __attribute__((always_inline));
inline void activateOutputColumn() __attribute__((always_inline));

class Board
{
    public:
    Board();
    void init();
    bool encoderEnabled(uint8_t encoderNumber);
    int8_t getEncoderState(uint8_t encoderID);
    uint8_t getButtonState(uint8_t buttonID);
    uint8_t getEncoderPair(uint8_t buttonID);
    uint16_t getPadPressure(uint8_t pad);
    int16_t getPadX(uint8_t pad);
    int16_t getPadY(uint8_t pad);
    void reboot(rebootType_t type);

    private:
    void initPins();
    void initTimers();
    void initPads();
};

extern Board board;
