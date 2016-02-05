#ifndef KONTROLAPCBIDS_H_
#define KONTROLAPCBIDS_H_

#define BOARD_V1

//mux
#define MUX_COMMON_PIN_0            A3
#define MUX_COMMON_PIN_1            A1
#define MUX_COMMON_PIN_2            A2
#define MUX_COMMON_PIN_3            A0

#define MUX_COMMON_PIN_AR_0         3
#define MUX_COMMON_PIN_AR_1         1
#define MUX_COMMON_PIN_AR_2         2
#define MUX_COMMON_PIN_AR_3         0

#define MUX_SELECT_PIN_0            A5
#define MUX_SELECT_PIN_1            A4
#define MUX_SELECT_PIN_2            A6
#define MUX_SELECT_PIN_3            A7

//pads
#define PAD_0                       2
#define PAD_1                       1
#define PAD_2                       0
#define PAD_3                       3
#define PAD_4                       4
#define PAD_5                       5
#define PAD_6                       6
#define PAD_7                       7
#define PAD_8                       8

//LEDs
#define LED_ROW_1_PIN               24
#define LED_ROW_2_PIN               25
#define LED_ROW_3_PIN               26

#define DECODER_OUT_1               21
#define DECODER_OUT_2               22
#define DECODER_OUT_3               23

#define LED_NOTE_C_SHARP            18
#define LED_NOTE_D_SHARP            22
#define LED_NOTE_F_SHARP            17
#define LED_NOTE_G_SHARP            21
#define LED_NOTE_A_SHARP            19

#define LED_NOTE_C                  10
#define LED_NOTE_D                  14
#define LED_NOTE_E                  9
#define LED_NOTE_F                  13
#define LED_NOTE_G                  11
#define LED_NOTE_A                  15
#define LED_NOTE_B                  23

#define LED_ON_OFF_NOTES            2
#define LED_ON_OFF_AFTERTOUCH       1
#define LED_ON_OFF_X                3
#define LED_ON_OFF_Y                0
#define LED_ON_OFF_SPLIT            6

#define LED_TRANSPORT_PLAY          5
#define LED_TRANSPORT_STOP          7
#define LED_TRANSPORT_RECORD        4

#define LED_OCTAVE_DOWN             8
#define LED_OCTAVE_UP               12

//buttons
#define BUTTON_NOTE_C_SHARP         19
#define BUTTON_NOTE_D_SHARP         17
#define BUTTON_NOTE_F_SHARP         5
#define BUTTON_NOTE_G_SHARP         3
#define BUTTON_NOTE_A_SHARP         1
#define BUTTON_NOTE_C               18
#define BUTTON_NOTE_D               16
#define BUTTON_NOTE_E               14
#define BUTTON_NOTE_F               15
#define BUTTON_NOTE_G               4
#define BUTTON_NOTE_A               2
#define BUTTON_NOTE_B               0

#define BUTTON_OCTAVE_DOWN          7
#define BUTTON_OCTAVE_UP            6

#define BUTTON_ON_OFF_NOTES         28
#define BUTTON_ON_OFF_AFTERTOUCH    29
#define BUTTON_ON_OFF_X             30
#define BUTTON_ON_OFF_Y             31
#define BUTTON_ON_OFF_SPLIT         27

#define BUTTON_TRANSPORT_PLAY       22
#define BUTTON_TRANSPORT_STOP       21
#define BUTTON_TRANSPORT_RECORD     20

//encoders
#define NUMBER_OF_ENCODERS          10

#define ENCODER_PAIR_00_PIN_0       14 //pwm
#define ENCODER_PAIR_00_PIN_1       15 //pwm

#define ENCODER_PAIR_01_PIN_0       19
#define ENCODER_PAIR_01_PIN_1       17

#define ENCODER_PAIR_02_PIN_0       35
#define ENCODER_PAIR_02_PIN_1       31

#define ENCODER_PAIR_03_PIN_0       33
#define ENCODER_PAIR_03_PIN_1       32

#define ENCODER_PAIR_04_PIN_0       34
#define ENCODER_PAIR_04_PIN_1       30

#define ENCODER_PAIR_05_PIN_0       9
#define ENCODER_PAIR_05_PIN_1       11

#define ENCODER_PAIR_06_PIN_0       29
#define ENCODER_PAIR_06_PIN_1       28

#define ENCODER_PAIR_07_PIN_0       18
#define ENCODER_PAIR_07_PIN_1       20

#define ENCODER_PAIR_08_PIN_0       13
#define ENCODER_PAIR_08_PIN_1       16 //pwm

#define ENCODER_PAIR_09_PIN_0       10
#define ENCODER_PAIR_09_PIN_1       12

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

#endif