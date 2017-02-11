#pragma once

#ifdef BOARD_R2
//mux
//common
#define MUX_COMMON_PIN_0_PORT       PORTF
#define MUX_COMMON_PIN_0_PIN        7

#define MUX_COMMON_PIN_1_PORT       PORTF
#define MUX_COMMON_PIN_1_PIN        5

#define MUX_COMMON_PIN_2_PORT       PORTF
#define MUX_COMMON_PIN_2_PIN        6

#define MUX_COMMON_PIN_3_PORT       PORTF
#define MUX_COMMON_PIN_3_PIN        4

//select
#define MUX_SELECT_PIN_0_PORT       PORTF
#define MUX_SELECT_PIN_0_PIN        1

#define MUX_SELECT_PIN_1_PORT       PORTF
#define MUX_SELECT_PIN_1_PIN        0

#define MUX_SELECT_PIN_2_PORT       PORTF
#define MUX_SELECT_PIN_2_PIN        2

#define MUX_SELECT_PIN_3_PORT       PORTF
#define MUX_SELECT_PIN_3_PIN        3

//LEDs

#define LED_ROW_1_PORT              PORTB
#define LED_ROW_1_PIN               4

#define LED_ROW_2_PORT              PORTB
#define LED_ROW_2_PIN               5

#define LED_ROW_3_PORT              PORTB
#define LED_ROW_3_PIN               6

#define DECODER_OUT_1_PORT          PORTC
#define DECODER_OUT_1_PIN           0

#define DECODER_OUT_2_PORT          PORTC
#define DECODER_OUT_2_PIN           1

#define DECODER_OUT_3_PORT          PORTC
#define DECODER_OUT_3_PIN           2

#define DECODER_OUT_PORT            PORTC

//used to clear all select bits in encoder (valid only if all three pins are on same port)
#define DECODER_OUT_CLEAR_MASK      0b11111000

const uint8_t decoderOutOrderArray[] =
{
    0b00000000,
    0b00000001,
    0b00000010,
    0b00000011,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111
};

//encoders/buttons

#define DECODER_IN_A0_PORT          PORTC
#define DECODER_IN_A0_PIN           3

#define DECODER_IN_A1_PORT          PORTC
#define DECODER_IN_A1_PIN           4

#define DECODER_IN_A2_PORT          PORTC
#define DECODER_IN_A2_PIN           5

#define DECODER_IN_PORT             PORTC

//used to clear all select bits in encoder (valid only if all three pins are on same port)
#define DECODER_IN_CLEAR_MASK       0b11000111

const uint8_t decoderInOrderArray[] =
{
    0b00000000,
    0b00001000,
    0b00010000,
    0b00011000,
    0b00100000,
    0b00101000,
    0b00110000,
    0b00111000
};

#define INPUT_SHIFT_REG_IN_PORT     PORTD
#define INPUT_SHIFT_REG_IN_PIN      5

#define INPUT_SHIFT_REG_CLOCK_PORT  PORTD
#define INPUT_SHIFT_REG_CLOCK_PIN   6

#define INPUT_SHIFT_REG_LATCH_PORT  PORTD
#define INPUT_SHIFT_REG_LATCH_PIN   7

//LCD
//todo

//bootloader button
#define BTLDR_BUTTON_PORT           PORTE
#define BTLDR_BUTTON_PIN            2

#endif
