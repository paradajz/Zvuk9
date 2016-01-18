#ifndef HD44780_SETTINGS_H
#define HD44780_SETTINGS_H

#include "hardware/pins/HardwareIDs.h"

#define USE_ADELAY_LIBRARY       0           // Set to 1 to use my ADELAY library, 0 to use internal delay functions
#define LCD_BITS                 4           // 4 for 4 Bit I/O Mode, 8 for 8 Bit I/O Mode
#define RW_LINE_IMPLEMENTED      1           // 0 for no RW line (RW on LCD tied to ground), 1 for RW line present
#define WAIT_MODE                1           // 0=Use Delay Method (Faster if running <10Mhz)
                                             // 1=Use Check Busy Flag (Faster if running >10Mhz) ***Requires RW Line***
#define DELAY_RESET              15          // in mS

#if defined (KONTROLA_BREADBOARD)
#define LCD_DB4_PORT             PORTD       // If using 4 bit mode, yo must configure DB4-DB7
#define LCD_DB4_PIN              5
#define LCD_DB5_PORT             PORTD
#define LCD_DB5_PIN              6
#define LCD_DB6_PORT             PORTD
#define LCD_DB6_PIN              7
#define LCD_DB7_PORT             PORTE
#define LCD_DB7_PIN              0

#define LCD_RS_PORT              PORTB       // Port for RS line
#define LCD_RS_PIN               7           // Pin for RS line

#define LCD_RW_PORT              PORTD       // Port for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)
#define LCD_RW_PIN               2           // Pin for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)

#define LCD_E_PORT               PORTD       // Port for E line
#define LCD_E_PIN                4           // Pin for E line
#elif defined (KONTROLA_PCB)
#define LCD_DB4_PORT             PORTD       // If using 4 bit mode, you must configure DB4-DB7
#define LCD_DB4_PIN              5
#define LCD_DB5_PORT             PORTD
#define LCD_DB5_PIN              6
#define LCD_DB6_PORT             PORTD
#define LCD_DB6_PIN              7
#define LCD_DB7_PORT             PORTE
#define LCD_DB7_PIN              0

#define LCD_RS_PORT              PORTB       // Port for RS line
#define LCD_RS_PIN               7           // Pin for RS line

#define LCD_RW_PORT              PORTD       // Port for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)
#define LCD_RW_PIN               2           // Pin for RW line (ONLY used if RW_LINE_IMPLEMENTED=1)

#define LCD_E_PORT               PORTD       // Port for E line
#define LCD_E_PIN                4           // Pin for E line
#endif

#define LCD_DISPLAYS             1           // Up to 4 LCD displays can be used at one time
                                             // All pins are shared between displays except for the E
                                             // pin which each display will have its own

                                             // Display 1 Settings - if you only have 1 display, YOU MUST SET THESE
#define LCD_DISPLAY_LINES        4           // Number of Lines, Only Used for Set I/O Mode Command

#endif

