#ifndef HD44780_SETTINGS_H
#define HD44780_SETTINGS_H

#define LCD_CONTROLLER_KS0073   0

#define LCD_LINES               4
#define LCD_DISP_LENGTH         20
#define LCD_LINE_LENGTH         0x40

#define LCD_START_LINE1         0x00
#define LCD_START_LINE2         0x40
#define LCD_START_LINE3         0x14
#define LCD_START_LINE4         0x54
#define LCD_WRAP_LINES          0

#define LCD_IO_MODE             1

#define LCD_DATA0_PORT          PORTD
#define LCD_DATA1_PORT          PORTD
#define LCD_DATA2_PORT          PORTD
#define LCD_DATA3_PORT          PORTE
#define LCD_DATA0_PIN           5
#define LCD_DATA1_PIN           6
#define LCD_DATA2_PIN           7
#define LCD_DATA3_PIN           0

#define LCD_RS_PORT             PORTB
#define LCD_RS_PIN              7

#define LCD_RW_PORT             PORTD
#define LCD_RW_PIN              2

#define LCD_E_PORT              PORTD
#define LCD_E_PIN               4

#endif

