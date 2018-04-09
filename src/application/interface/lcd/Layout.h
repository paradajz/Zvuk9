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

#pragma once

///
/// \brief Positions of various items on LCD
/// @{

#define LCD_ROW_DEVICE_NAME_MESSAGE                     1
#define LCD_ROW_WELCOME_MESSAGE                         2

#define LCD_ROW_PROGRAM_INFO_PROGRAM                    0
#define LCD_POSITION_PROGRAM_INFO_PROGRAM               0

#define LCD_ROW_PROGRAM_INFO_DELIMITER_1                0
#define LCD_POSITION_PROGRAM_INFO_DELIMITER_1           4

#define LCD_ROW_PROGRAM_INFO_SCALE                      0
#define LCD_POSITION_PROGRAM_INFO_SCALE                 (LCD_POSITION_PROGRAM_INFO_DELIMITER_1+2)

#define LCD_ROW_PROGRAM_INFO_DELIMITER_2                0
#define LCD_POSITION_PROGRAM_INFO_DELIMITER_2           23

#define LCD_ROW_PROGRAM_INFO_TONIC                      0
#define LCD_POSITION_PROGRAM_INFO_TONIC                 (LCD_POSITION_PROGRAM_INFO_DELIMITER_2+2)

#define LCD_ROW_PROGRAM_INFO_DELIMITER_3                0
#define LCD_POSITION_PROGRAM_INFO_DELIMITER_3           (LCD_POSITION_PROGRAM_INFO_TONIC+3)

#define LCD_ROW_PROGRAM_INFO_SHIFT                      0
#define LCD_POSITION_PROGRAM_INFO_SHIFT                 (LCD_POSITION_PROGRAM_INFO_DELIMITER_3+2)

#define LCD_ROW_PRESS_INFO_PAD_NUMBER                   1
#define LCD_POSITION_PRESS_INFO_PAD_NUMBER              0

#define LCD_ROW_PRESS_INFO_NOTES                        1
#define LCD_POSITION_PRESS_INFO_NOTES                   7

#define LCD_ROW_PRESS_INFO_VELOCITY                     2
#define LCD_POSITION_PRESS_INFO_VELOCITY                0

#define LCD_ROW_PRESS_INFO_DELIMITER_1                  2
#define LCD_POSITION_PRESS_INFO_DELIMITER_1             7

#define LCD_ROW_PRESS_INFO_ATOUCH                       2
#define LCD_POSITION_PRESS_INFO_ATOUCH                  (LCD_POSITION_PRESS_INFO_DELIMITER_1+2)

#define LCD_POSITION_CLEAR_VELOCITY                     LCD_POSITION_PRESS_INFO_VELOCITY
#define LCD_POSITION_CLEAR_AFTERTOUCH                   (LCD_POSITION_PRESS_INFO_DELIMITER_1-1)

#define LCD_ROW_PRESS_INFO_X                            2
#define LCD_POSITION_PRESS_INFO_X                       17
#define LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER      19
#define LCD_POSITION_PRESS_INFO_X_VALUE                 21

#define LCD_ROW_PRESS_INFO_Y                            (LCD_ROW_PRESS_INFO_X+1)
#define LCD_POSITION_PRESS_INFO_Y                       LCD_POSITION_PRESS_INFO_X
#define LCD_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER      LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER
#define LCD_POSITION_PRESS_INFO_Y_VALUE                 LCD_POSITION_PRESS_INFO_X_VALUE

#define LCD_ROW_PRESS_INFO_MIDI_CHANNEL                 3
#define LCD_POSITION_PRESS_INFO_MIDI_CHANNEL            0

#define LCD_ROW_PAD_EDIT_PAD_NUMBER                     0
#define LCD_POSITION_PAD_EDIT_PAD_NUMBER                0

#define LCD_ROW_PAD_EDIT_ASSIGNED_NOTES_INFO            1

#define LCD_ROW_PAD_EDIT_NOTES                          2
#define LCD_POSITION_PAD_EDIT_NOTES                     0

#define LCD_ROW_MESSAGE_1                               1
#define LCD_POSITION_MESSAGE_1                          0

#define LCD_ROW_MESSAGE_2                               2
#define LCD_POSITION_MESSAGE_2                          0

#define LCD_ROW_CALIBRATION_VALUES                      2
#define LCD_ROW_CALIBRATION_SCROLL_INFO                 3
#define LCD_POSITION_CALIBRATION_RAW_VALUE_TEXT         0
#define LCD_POSITION_CALIBRATION_RAW_VALUE_VALUE        5
#define LCD_POSITION_CALIBRATION_MIDI_VALUE_TEXT        10
#define LCD_POSITION_CALIBRATION_MIDI_VALUE_VALUE       16

#define LCD_ROW_MENU_DEVICE_INFO_1                      1
#define LCD_POSITION_MENU_DEVICE_INFO_1                 0

#define LCD_ROW_MENU_DEVICE_INFO_2                      2
#define LCD_POSITION_MENU_DEVICE_INFO_2                 0

#define LCD_ROW_FACTORY_RESET_TITLE                     0
#define LCD_ROW_FACTORY_RESET_INFO_CONFIRM              1
#define LCD_ROW_FACTORY_RESET_PADS                      2
#define LCD_ROW_FACTORY_RESET_INFO_CANCEL               3

#define LCD_POSITION_FACTORY_RESET_PAD_1                12
#define LCD_POSITION_FACTORY_RESET_PAD_7                17
#define LCD_POSITION_FACTORY_RESET_PAD_9                22

#define LCD_ROW_FACTORY_RESET_PROGRESS_1                1
#define LCD_ROW_FACTORY_RESET_PROGRESS_2                2

/// @}
