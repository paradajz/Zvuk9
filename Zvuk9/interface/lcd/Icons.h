#ifndef ICONS_H_
#define ICONS_H_

#include <avr/pgmspace.h>
#include "../../hardware/lcd/lcd.h"
#include "Macros.h"

#define ICON_SIZE                   8
#define TRANSPORT_CONTROL_ICON_ROW  3

const uint8_t PROGMEM playIcon[] PROGMEM = {

    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11100,
    0b11000,
    0b10000,
    0b00000

};

const uint8_t PROGMEM stopIcon[] PROGMEM = {

    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000

};

const uint8_t PROGMEM recordIcon[] PROGMEM = {

    0b00000,
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00000

};

const uint8_t* const iconArray[] PROGMEM = {

    stopIcon,
    playIcon,
    recordIcon,

};

void initIcons();

void initIcons()    {

    //set CG RAM start address 0
    lcd_command(_BV(LCD_CGRAM));

    uint8_t buffer[ICON_SIZE];

    for (int i=0; i<(int)progmemArraySize(iconArray); i++)   {

        memcpy_P(buffer, (uint8_t*)pgm_read_word(&(iconArray[i])), ICON_SIZE);

        for (int j=0; j<ICON_SIZE; j++)
            lcd_data(buffer[j]);

    }

}


#endif