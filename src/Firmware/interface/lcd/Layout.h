#pragma once

#define LCD_ROW_WELCOME_MESSAGE                         1

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

#define LCD_ROW_PRESS_INFO_X_CC                         2
#define LCD_POSITION_PRESS_INFO_X_CC                    18

#define LCD_ROW_PRESS_INFO_Y_CC                         (LCD_ROW_PRESS_INFO_X_CC+1)
#define LCD_POSITION_PRESS_INFO_Y_CC                    LCD_POSITION_PRESS_INFO_X_CC

#define LCD_ROW_PRESS_INFO_X_POS                        LCD_ROW_PRESS_INFO_X_CC
#define LCD_POSITION_PRESS_INFO_X_POS                   29

#define LCD_ROW_PRESS_INFO_Y_POS                        (LCD_ROW_PRESS_INFO_X_POS+1)
#define LCD_POSITION_PRESS_INFO_Y_POS                   LCD_POSITION_PRESS_INFO_X_POS

#define LCD_ROW_PRESS_INFO_X_CC_POS_DELIMITER           LCD_ROW_PRESS_INFO_X_CC
#define LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER      27

#define LCD_ROW_PRESS_INFO_Y_CC_POS_DELIMITER           LCD_ROW_PRESS_INFO_Y_CC
#define LCD_POSITION_PRESS_INFO_Y_CC_POS_DELIMITER      LCD_POSITION_PRESS_INFO_X_CC_POS_DELIMITER

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

#define LCD_ROW_MESSAGE_3                               3
#define LCD_POSITION_MESSAGE_3                          0

#define LCD_ROW_PAD_CALIBRATION                         0
#define LCD_POSITION_PAD_CALIBRATION                    0

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