#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define NUMBER_OF_ENCODERS          10
#define NUMBER_OF_LEDS              24
#define MAX_PADS                    9
#define MAX_NUMBER_OF_BUTTONS       32
#define NUMBER_OF_LED_COLUMNS       8
#define NUMBER_OF_LED_ROWS          3

#define NUMBER_OF_TRANSITIONS       64
#define DEFAULT_FADE_SPEED          15

#define LED_INTENSITY_OFF           0
#define LED_INTENSITY_FULL          255
#define LED_INTENSITY_HALF          100

#define LED_BLINK_TIME              250 //ms

/*

    LED state is stored into one byte (ledState). The bits have following meaning (7 being the MSB bit):

    7: x
    6: x
    5: x
    4: x
    3: Blink bit (timer changes this bit)
    2: LED blinking is on
    1: LED is constantly turned on, half intensity
    0: LED is constantly turned on, full intensity

    */

#define LED_FULL_INTENSITY_BIT  0
#define LED_HALF_INTENSITY_BIT  1
#define LED_BLINK_ENABLED_BIT   2
#define LED_BLINK_STATE_BIT     3

/*

    Encoder data formatting, uint16_t variable type
    0      1      2      3
    0000 | 0000 | 0000 | 0000

    0 - encoder direction (0/1 - left/right)
    1 - encoderMoving (0/1/2 - stopped/left/right)
    2 - counted pulses (default value is 8 to avoid issues with negative values)
    3 - temp encoder state (2 readings of 2 encoder pairs)

*/

#define PULSES_PER_STEP                     4
#define ENCODER_CLEAR_TEMP_STATE_MASK       0xFFF0
#define ENCODER_CLEAR_PULSES_MASK           0xFF0F
#define ENCODER_CLEAR_MOVING_STATUS_MASK    0xF0FF

#define ENCODER_DIRECTION_BIT               15

#define ENCODER_DEFAULT_PULSE_COUNT_STATE   8

const int8_t encoderLookUpTable[] = { 0, 1, -1, 2, -1, 0, -2, 1, 1, -2, 0, -1, 2, -1, 1, 0 };

//MCP23017 address bytes
const uint8_t expanderAddress[] = { 0x21, 0x20 };   //chip address
const uint8_t gpioAddress[]     = { 0x12, 0x13 };   //input/output
const uint8_t iodirAddress[]    = { 0x00, 0x01 };   //port A/port B
const uint8_t gppuAddress[]     = { 0x0C, 0x0D };   //interrupt/pull-up



#endif