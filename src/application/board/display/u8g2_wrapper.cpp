/*
    OpenDeck MIDI platform firmware
    Copyright (C) 2015-2018 Igor Petrovic

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
*/

//c++ wrapper based on original wrapper for arduino

#include "u8g2_wrapper.h"
#include "../pins/Pins.h"

uint8_t rows, columns;

extern "C" uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
        //init lcd pins
        setOutput(LCD_CS_PORT, LCD_CS_PIN);
        setOutput(LCD_DC_PORT, LCD_DC_PIN);
        break;

        case U8X8_MSG_DELAY_10MICRO:
        /* not used at the moment */
        break;

        case U8X8_MSG_DELAY_100NANO:
        /* not used at the moment */
        break;

        case U8X8_MSG_DELAY_MILLI:
        wait_ms(arg_int);
        break;

        case U8X8_MSG_GPIO_CS:
        arg_int ? setHigh(LCD_CS_PORT, LCD_CS_PIN) : setLow(LCD_CS_PORT, LCD_CS_PIN);
        break;

        case U8X8_MSG_GPIO_DC:
        arg_int ? setHigh(LCD_DC_PORT, LCD_DC_PIN) : setLow(LCD_DC_PORT, LCD_DC_PIN);
        break;

        default:
        return 0;
        break;
    }

    return 1;
}

extern "C" uint8_t u8x8_byte_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;
    uint8_t internal_spi_mode;

    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;

        while( arg_int > 0 )
        {
            spiTransfer((uint8_t)*data);
            data++;
            arg_int--;
        }
        break;

        case U8X8_MSG_BYTE_INIT:
        /* disable chipselect */
        setHigh(LCD_CS_PORT, LCD_CS_PIN);
        spiInit();
        break;

        case U8X8_MSG_BYTE_SET_DC:
        //u8x8_gpio_SetDC(u8x8, arg_int);
        arg_int ? setHigh(LCD_DC_PORT, LCD_DC_PIN) : setLow(LCD_DC_PORT, LCD_DC_PIN);
        break;

        case U8X8_MSG_BYTE_START_TRANSFER:
        internal_spi_mode = 0;

        switch(u8x8->display_info->spi_mode)
        {
            case 0:
            internal_spi_mode = SPI_MODE0;
            break;

            case 1:
            internal_spi_mode = SPI_MODE1;
            break;

            case 2:
            internal_spi_mode = SPI_MODE2;
            break;
            
            case 3:
            internal_spi_mode = SPI_MODE3;
            break;
        }

        spiInit();

        if (u8x8->display_info->sck_pulse_width_ns < 70)
            setClockDivider(SPI_CLOCK_DIV2);
        else if (u8x8->display_info->sck_pulse_width_ns < 140)
            setClockDivider(SPI_CLOCK_DIV4);
        else
            setClockDivider(SPI_CLOCK_DIV8);

        setDataMode(internal_spi_mode);
        setBitOrder(MSBFIRST);

        //enable chip select
        setLow(LCD_CS_PORT, LCD_CS_PIN);
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
        break;

        case U8X8_MSG_BYTE_END_TRANSFER:
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
        //disable chip select
        setHigh(LCD_CS_PORT, LCD_CS_PIN);
        break;

        default:
        return 0;
    }

    return 1;
}


U8X8::U8X8()
{

}

bool U8X8::initDisplay()
{
    //setup defaults
    u8x8_SetupDefaults(&u8x8);

    u8x8.display_cb = u8x8_d_ssd1322_nhd_256x64;
    u8x8.cad_cb = u8x8_cad_011;
    u8x8.byte_cb = u8x8_byte_hw_spi;
    u8x8.gpio_and_delay_cb = u8x8_gpio_and_delay;
    rows = 4;
    columns = 32;

    /* setup display info */
    u8x8_SetupMemory(&u8x8);
    u8x8_InitDisplay(&u8x8);

    clearDisplay();
    setPowerSave(0);

    return true;
}

uint8_t U8X8::getColumns()
{
    return columns;
}

uint8_t U8X8::getRows()
{
    return rows;
}

void U8X8::drawTile(uint8_t x, uint8_t y, uint8_t cnt, uint8_t *tile_ptr)
{
    u8x8_DrawTile(&u8x8, x, y, cnt, tile_ptr);
}

void U8X8::clearDisplay()
{
    u8x8_ClearDisplay(&u8x8);
}

void U8X8::fillDisplay()
{
    u8x8_FillDisplay(&u8x8);
}

void U8X8::setPowerSave(uint8_t is_enable)
{
    u8x8_SetPowerSave(&u8x8, is_enable);
}

void U8X8::setFlipMode(uint8_t mode)
{
    u8x8_SetFlipMode(&u8x8, mode);
}

void U8X8::refreshDisplay(void)
{
    // Dec 16: Only required for SSD1606
    u8x8_RefreshDisplay(&u8x8);
}

void U8X8::clearLine(uint8_t line)
{
    u8x8_ClearLine(&u8x8, line);
}

void U8X8::setContrast(uint8_t value)
{
    u8x8_SetContrast(&u8x8, value);
}

void U8X8::setInverseFont(uint8_t value)
{
    u8x8_SetInverseFont(&u8x8, value);
}

void U8X8::setFont(const uint8_t *font_8x8)
{
    u8x8_SetFont(&u8x8, font_8x8);
}

void U8X8::drawGlyph(uint8_t x, uint8_t y, uint8_t encoding)
{
    u8x8_DrawGlyph(&u8x8, x, y, encoding);
}

void U8X8::draw2x2Glyph(uint8_t x, uint8_t y, uint8_t encoding)
{
    u8x8_Draw2x2Glyph(&u8x8, x, y, encoding);
}

void U8X8::drawString(uint8_t x, uint8_t y, const char *s)
{
    u8x8_DrawString(&u8x8, x, y, s);
}

void U8X8::drawUTF8(uint8_t x, uint8_t y, const char *s)
{
    u8x8_DrawUTF8(&u8x8, x, y, s);
}

void U8X8::draw2x2String(uint8_t x, uint8_t y, const char *s)
{
    u8x8_Draw2x2String(&u8x8, x, y, s);
}

void U8X8::draw2x2UTF8(uint8_t x, uint8_t y, const char *s)
{
    u8x8_Draw2x2UTF8(&u8x8, x, y, s);
}

uint8_t U8X8::getUTF8Len(const char *s)
{
    return u8x8_GetUTF8Len(&u8x8, s);
}

void U8X8::inverse()
{
    setInverseFont(1);
}

void U8X8::noInverse()
{
    setInverseFont(0);
}

/* return 0 for no event or U8X8_MSG_GPIO_MENU_SELECT, */
/* U8X8_MSG_GPIO_MENU_NEXT, U8X8_MSG_GPIO_MENU_PREV, */
/* U8X8_MSG_GPIO_MENU_HOME */
uint8_t U8X8::getMenuEvent()
{
    return u8x8_GetMenuEvent(&u8x8);
}

uint8_t U8X8::userInterfaceSelectionList(const char *title, uint8_t start_pos, const char *sl)
{
    return u8x8_UserInterfaceSelectionList(&u8x8, title, start_pos, sl);
}

uint8_t U8X8::userInterfaceMessage(const char *title1, const char *title2, const char *title3, const char *buttons)
{
    return u8x8_UserInterfaceMessage(&u8x8, title1, title2, title3, buttons);
}

uint8_t U8X8::userInterfaceInputValue(const char *title, const char *pre, uint8_t *value, uint8_t lo, uint8_t hi, uint8_t digits, const char *post)
{
    return u8x8_UserInterfaceInputValue(&u8x8, title, pre, value, lo, hi, digits, post);
}

void U8X8::home()
{
    tx = 0;
    ty = 0;
}

void U8X8::noDisplay()
{
    u8x8_SetPowerSave(&u8x8, 1);
}

void U8X8::display()
{
    u8x8_SetPowerSave(&u8x8, 0);
}

void U8X8::setCursor(uint8_t x, uint8_t y)
{
    tx = x;
    ty = y;
}

U8X8 display_hw;