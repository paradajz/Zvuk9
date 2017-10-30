/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

/*

  U8x8lib.h
  
  C++ Arduino wrapper for the u8x8 struct and c functions.
  
  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  


*/

#ifndef _U8X8LIB_HH
#define _U8X8LIB_HH

#include "clib/u8x8.h"
#include "../../../pins/Pins.h"

extern "C" uint8_t u8x8_byte_arduino_hw_spi(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern "C" uint8_t u8x8_gpio_and_delay_arduino(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

class U8X8
{
    protected:
    u8x8_t u8x8;

    public:
    uint8_t tx, ty;

    U8X8()
    {
        home();
    }

    u8x8_t *getU8x8()
    {
        return &u8x8;
    }

    void setI2CAddress(uint8_t adr)
    {
        u8x8_SetI2CAddress(&u8x8, adr);
    }

    uint8_t getCols()
    {
        return u8x8_GetCols(&u8x8);
    }

    uint8_t getRows()
    {
        return u8x8_GetRows(&u8x8);
    }

    void drawTile(uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr)
    {
        u8x8_DrawTile(&u8x8, x, y, cnt, tile_ptr);
    }

    void initDisplay()
    {
        u8x8_InitDisplay(&u8x8);
    }

    void clearDisplay()
    {
        u8x8_ClearDisplay(&u8x8);
    }

    void fillDisplay()
    {
        u8x8_FillDisplay(&u8x8);
    }

    void setPowerSave(uint8_t is_enable)
    {
        u8x8_SetPowerSave(&u8x8, is_enable);
    }

    void begin()
    {
        initDisplay();
        clearDisplay();
        setPowerSave(0);
    }

    void setFlipMode(uint8_t mode)
    {
        u8x8_SetFlipMode(&u8x8, mode);
    }

    void clearLine(uint8_t line)
    {
        u8x8_ClearLine(&u8x8, line);
    }

    void setContrast(uint8_t value)
    {
        u8x8_SetContrast(&u8x8, value);
    }

    void setInverseFont(uint8_t value)
    {
        u8x8_SetInverseFont(&u8x8, value);
    }

    void setFont(const uint8_t *font_8x8)
    {
        u8x8_SetFont(&u8x8, font_8x8);
    }

    void drawGlyph(uint8_t x, uint8_t y, uint8_t encoding)
    {
        u8x8_DrawGlyph(&u8x8, x, y, encoding);
    }

    void draw2x2Glyph(uint8_t x, uint8_t y, uint8_t encoding)
    {
        u8x8_Draw2x2Glyph(&u8x8, x, y, encoding);
    }

    void drawString(uint8_t x, uint8_t y, const char *s)
    {
        u8x8_DrawString(&u8x8, x, y, s);
    }

    void drawUTF8(uint8_t x, uint8_t y, const char *s)
    {
        u8x8_DrawUTF8(&u8x8, x, y, s);
    }

    void draw2x2String(uint8_t x, uint8_t y, const char *s)
    {
        u8x8_Draw2x2String(&u8x8, x, y, s);
    }

    void draw2x2UTF8(uint8_t x, uint8_t y, const char *s)
    {
        u8x8_Draw2x2UTF8(&u8x8, x, y, s);
    }

    uint8_t getUTF8Len(const char *s)
    {
        return u8x8_GetUTF8Len(&u8x8, s);
    }

    size_t write(uint8_t v);

    size_t write(const uint8_t *buffer, size_t size)
    {
        size_t cnt = 0;

        while( size > 0 )
        {
            cnt += write(*buffer++);
            size--;
        }

      return cnt;
    }

    void inverse()
    {
        setInverseFont(1);
    }

    void noInverse()
    {
        setInverseFont(0);
    }

    /* return 0 for no event or U8X8_MSG_GPIO_MENU_SELECT, */
    /* U8X8_MSG_GPIO_MENU_NEXT, U8X8_MSG_GPIO_MENU_PREV, */
    /* U8X8_MSG_GPIO_MENU_HOME */
    uint8_t getMenuEvent()
    {
        return u8x8_GetMenuEvent(&u8x8);
    }

    uint8_t userInterfaceSelectionList(const char *title, uint8_t start_pos, const char *sl)
    {
        return u8x8_UserInterfaceSelectionList(&u8x8, title, start_pos, sl);
    }

    uint8_t userInterfaceMessage(const char *title1, const char *title2, const char *title3, const char *buttons)
    {
        return u8x8_UserInterfaceMessage(&u8x8, title1, title2, title3, buttons);
    }

    uint8_t userInterfaceInputValue(const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post)
    {
        return u8x8_UserInterfaceInputValue(&u8x8, title, pre, value, lo, hi, digits, post);
    }

    /* LiquidCrystal compatible functions */
    void home()
    {
        tx = 0;
        ty = 0;
    }

    void clear()
    {
        clearDisplay();
        home();
    }

    void noDisplay()
    {
        u8x8_SetPowerSave(&u8x8, 1);
    }

    void display()
    {
        u8x8_SetPowerSave(&u8x8, 0);
    }

    void setCursor(uint8_t x, uint8_t y)
    {
        tx = x;
        ty = y;
    }
};

class U8X8_SSD1322_NHD_256X64_4W_HW_SPI : public U8X8
{
    public: U8X8_SSD1322_NHD_256X64_4W_HW_SPI() : U8X8()
    {
        u8x8_Setup(getU8x8(), u8x8_d_ssd1322_nhd_256x64, u8x8_cad_011, u8x8_byte_arduino_hw_spi, u8x8_gpio_and_delay_arduino);
    }
};

#endif /* _U8X8LIB_HH */
