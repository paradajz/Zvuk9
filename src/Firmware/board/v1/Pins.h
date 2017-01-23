#pragma once

//mux
//common
#define MUX_COMMON_PIN_0_PORT       PORTF
#define MUX_COMMON_PIN_0_PIN        3

#define MUX_COMMON_PIN_1_PORT       PORTF
#define MUX_COMMON_PIN_1_PIN        1

#define MUX_COMMON_PIN_2_PORT       PORTF
#define MUX_COMMON_PIN_2_PIN        2

#define MUX_COMMON_PIN_3_PORT       PORTF
#define MUX_COMMON_PIN_3_PIN        0

//select
#define MUX_SELECT_PIN_0_PORT       PORTF
#define MUX_SELECT_PIN_0_PIN        5

#define MUX_SELECT_PIN_1_PORT       PORTF
#define MUX_SELECT_PIN_1_PIN        4

#define MUX_SELECT_PIN_2_PORT       PORTF
#define MUX_SELECT_PIN_2_PIN        6

#define MUX_SELECT_PIN_3_PORT       PORTF
#define MUX_SELECT_PIN_3_PIN        7

//LEDs

#define LED_ROW_1_PORT              PORTB
#define LED_ROW_1_PIN               4

#define LED_ROW_2_PORT              PORTB
#define LED_ROW_2_PIN               5

#define LED_ROW_3_PORT              PORTB
#define LED_ROW_3_PIN               6

#define DECODER_OUT_1_PORT          PORTB
#define DECODER_OUT_1_PIN           1

#define DECODER_OUT_2_PORT          PORTB
#define DECODER_OUT_2_PIN           2

#define DECODER_OUT_3_PORT          PORTB
#define DECODER_OUT_3_PIN           3

#define DECODER_CLEAR_MASK          0b11110001

const uint8_t decoderStateArray[] =
{
    0b00000000,
    0b00000010,
    0b00000100,
    0b00000110,
    0b00001000,
    0b00001010,
    0b00001100,
    0b00001110
};

//encoders

//encoder 0
#define ENCODER_PAIR_00_PIN_0_PORT  PORTC
#define ENCODER_PAIR_00_PIN_0_PIN   4

#define ENCODER_PAIR_00_PIN_1_PORT  PORTC
#define ENCODER_PAIR_00_PIN_1_PIN   5

//encoder 1
#define ENCODER_PAIR_01_PIN_0_PORT  PORTE
#define ENCODER_PAIR_01_PIN_0_PIN   7

#define ENCODER_PAIR_01_PIN_1_PORT  PORTC
#define ENCODER_PAIR_01_PIN_1_PIN   7

//encoder 2
#define ENCODER_PAIR_02_PIN_0_PORT  PORTA
#define ENCODER_PAIR_02_PIN_0_PIN   7

#define ENCODER_PAIR_02_PIN_1_PORT  PORTA
#define ENCODER_PAIR_02_PIN_1_PIN   3

//encoder 3
#define ENCODER_PAIR_03_PIN_0_PORT  PORTA
#define ENCODER_PAIR_03_PIN_0_PIN   5

#define ENCODER_PAIR_03_PIN_1_PORT  PORTA
#define ENCODER_PAIR_03_PIN_1_PIN   4

//encoder 4
#define ENCODER_PAIR_04_PIN_0_PORT  PORTA
#define ENCODER_PAIR_04_PIN_0_PIN   6

#define ENCODER_PAIR_04_PIN_1_PORT  PORTA
#define ENCODER_PAIR_04_PIN_1_PIN   2

//encoder 5
#define ENCODER_PAIR_05_PIN_0_PORT  PORTE
#define ENCODER_PAIR_05_PIN_0_PIN   1

#define ENCODER_PAIR_05_PIN_1_PORT  PORTC
#define ENCODER_PAIR_05_PIN_1_PIN   1

//encoder 6
#define ENCODER_PAIR_06_PIN_0_PORT  PORTA
#define ENCODER_PAIR_06_PIN_0_PIN   1

#define ENCODER_PAIR_06_PIN_1_PORT  PORTA
#define ENCODER_PAIR_06_PIN_1_PIN   0

//encoder 7
#define ENCODER_PAIR_07_PIN_0_PORT  PORTE
#define ENCODER_PAIR_07_PIN_0_PIN   6

#define ENCODER_PAIR_07_PIN_1_PORT  PORTB
#define ENCODER_PAIR_07_PIN_1_PIN   0

//encoder 8
#define ENCODER_PAIR_08_PIN_0_PORT  PORTC
#define ENCODER_PAIR_08_PIN_0_PIN   3

#define ENCODER_PAIR_08_PIN_1_PORT  PORTC
#define ENCODER_PAIR_08_PIN_1_PIN   6

//encoder 9
#define ENCODER_PAIR_09_PIN_0_PORT  PORTC
#define ENCODER_PAIR_09_PIN_0_PIN   0

#define ENCODER_PAIR_09_PIN_1_PORT  PORTC
#define ENCODER_PAIR_09_PIN_1_PIN   2

//LCD
#define LCD_DB4_PORT                PORTD       // If using 4 bit mode, you must configure DB4-DB7
#define LCD_DB4_PIN                 5

#define LCD_DB5_PORT                PORTD
#define LCD_DB5_PIN                 6

#define LCD_DB6_PORT                PORTD
#define LCD_DB6_PIN                 7

#define LCD_DB7_PORT                PORTE
#define LCD_DB7_PIN                 0

#define LCD_RS_PORT                 PORTB       // Port for RS line
#define LCD_RS_PIN                  7           // Pin for RS line

#define LCD_RW_PORT                 PORTD       // Port for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)
#define LCD_RW_PIN                  2           // Pin for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)

#define LCD_E_PORT                  PORTD       // Port for E line
#define LCD_E_PIN                   4           // Pin for E line

//bootloader button
#define BTLDR_BUTTON_PORT           PORTE
#define BTLDR_BUTTON_PIN            2
