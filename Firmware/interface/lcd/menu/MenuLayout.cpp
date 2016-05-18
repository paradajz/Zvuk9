#include "Menu.h"
#ifdef MENU_H_
#include "Items.h"

#include "../../../version/Firmware.h"
#include "../../../version/Hardware.h"
#include "../../../eeprom/EEPROMsettings.h"
#include "../../../init/Init.h"

void factory_reset(factoryResetType_t type) {

    leds.setFadeSpeed(1);
    leds.allLEDsOff();
    configuration.factoryReset(type);
    globalInit();
    menu.exitMenu();

}

void runMenuFunction() {

    #if MODE_SERIAL > 0
    printf("great success\n");
    #endif

    strcpy_P(display.stringBuffer, dummy_string);
    uint8_t size = progmemCharArraySize(dummy_string);

    for (int i=1; i<NUMBER_OF_LCD_ROWS; i++)
    display.updateDisplay(i, text, 0, true, size);

}

void displayDeviceInfo(deviceInfo type)   {

    uint8_t byte0, byte1, byte2;

    switch(type)    {

        case firmware:
        byte0 = FIRMWARE_VERSION_BYTE_0;
        byte1 = FIRMWARE_VERSION_BYTE_1;
        byte2 = FIRMWARE_VERSION_BYTE_2;
        break;

        case hardware:
        byte0 = HARDWARE_VERSION_BYTE_0;
        byte1 = HARDWARE_VERSION_BYTE_1;
        byte2 = HARDWARE_VERSION_BYTE_2;
        break;

        default:
        return;

    }

    uint8_t size = 0;
    display.stringBuffer[0] = '\0';
    display.addNumberToCharArray(byte0, size);
    display.appendText(".", size);
    display.addNumberToCharArray(byte1, size);
    display.appendText(".", size);
    display.addNumberToCharArray(byte2, size);

    display.updateDisplay(1, text, 0, true, size);

    strcpy_P(display.stringBuffer, emptyLine_string);
    size = progmemCharArraySize(dummy_string);

    display.updateDisplay(2, text, 0, true, size);
    display.updateDisplay(3, text, 0, true, size);

}

void displayHardwareVersion()   {

    displayDeviceInfo(hardware);

}

void displayFirmwareVersion()   {

    displayDeviceInfo(firmware);

}

void partialReset() {

    factory_reset(factoryReset_restore);

}

void fullReset()    {

    factory_reset(factoryReset_partial);

}

void Menu::createLayout()   {

    menuItem[menuItem_calibration].stringPointer = serviceMenuOption_padCalibration;
    menuItem[menuItem_calibration].level = 1;
    menuItem[menuItem_calibration].function = NULL;

        {

            menuItem[menuItem_calibrateX].stringPointer = calibration_x;
            menuItem[menuItem_calibrateX].level = 11;
            menuItem[menuItem_calibrateX].function = NULL;

            {
                menuItem[menuItem_calibrateXlower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibrateXlower].level = 111;
                menuItem[menuItem_calibrateXlower].function = NULL;

                    {
                        menuItem[menuItem_calibrateXlowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateXlowerInteractive].level = 1111;
                        menuItem[menuItem_calibrateXlowerInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibrateXlowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateXlowerManual].level = 1112;
                        menuItem[menuItem_calibrateXlowerManual].function = runMenuFunction;
                    }

                menuItem[menuItem_calibrateXupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateXupper].level = 112;
                menuItem[menuItem_calibrateXupper].function = NULL;

                    {
                        menuItem[menuItem_calibrateXupperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateXupperInteractive].level = 1121;
                        menuItem[menuItem_calibrateXupperInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibrateXupperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateXupperManual].level = 1122;
                        menuItem[menuItem_calibrateXupperManual].function = runMenuFunction;
                    }
            }

            menuItem[menuItem_calibrateY].stringPointer = calibration_y;
            menuItem[menuItem_calibrateY].level = 12;
            menuItem[menuItem_calibrateY].function = NULL;

            {
                menuItem[menuItem_calibrateYlower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibrateYlower].level = 121;
                menuItem[menuItem_calibrateYlower].function = NULL;

                    {
                        menuItem[menuItem_calibrateYlowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateYlowerInteractive].level = 1211;
                        menuItem[menuItem_calibrateYlowerInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibrateYlowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateYlowerManual].level = 1212;
                        menuItem[menuItem_calibrateYlowerManual].function = runMenuFunction;
                    }

                menuItem[menuItem_calibrateYupper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibrateYupper].level = 122;

                    {
                        menuItem[menuItem_calibrateYupperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibrateYupperInteractive].level = 1221;
                        menuItem[menuItem_calibrateYupperInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibrateYupperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibrateYupperManual].level = 1222;
                        menuItem[menuItem_calibrateYupperManual].function = runMenuFunction;
                    }
            }

            menuItem[menuItem_calibratePressure].stringPointer = calibration_pressure;
            menuItem[menuItem_calibratePressure].level = 13;
            menuItem[menuItem_calibratePressure].function = NULL;

            {
                menuItem[menuItem_calibratePressureLower].stringPointer = calibrationDirection_lower;
                menuItem[menuItem_calibratePressureLower].level = 131;
                menuItem[menuItem_calibratePressureLower].function = NULL;

                    {
                        menuItem[menuItem_calibratePressureLowerInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureLowerInteractive].level = 1311;
                        menuItem[menuItem_calibratePressureLowerInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibratePressureLowerManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureLowerManual].level = 1312;
                        menuItem[menuItem_calibratePressureLowerManual].function = runMenuFunction;
                    }

                menuItem[menuItem_calibratePressureUpper].stringPointer = calibrationDirection_upper;
                menuItem[menuItem_calibratePressureUpper].level = 132;
                menuItem[menuItem_calibratePressureUpper].function = NULL;

                    {
                        menuItem[menuItem_calibratePressureUpperInteractive].stringPointer = calibrationMode_interactive;
                        menuItem[menuItem_calibratePressureUpperInteractive].level = 1321;
                        menuItem[menuItem_calibratePressureUpperInteractive].function = runMenuFunction;

                        menuItem[menuItem_calibratePressureUpperManual].stringPointer = calibrationMode_manual;
                        menuItem[menuItem_calibratePressureUpperManual].level = 1322;
                        menuItem[menuItem_calibratePressureUpperManual].function = runMenuFunction;
                    }
            }

        }

    menuItem[menuItem_deviceInfo].stringPointer = serviceMenuOption_deviceInfo;
    menuItem[menuItem_deviceInfo].level = 2;
    menuItem[menuItem_deviceInfo].function = NULL;

        {
            menuItem[menuItem_softwareInfo].stringPointer = deviceInfo_swVersion;
            menuItem[menuItem_softwareInfo].level = 21;
            menuItem[menuItem_softwareInfo].function = displayFirmwareVersion;

            menuItem[menuItem_hardwareInfo].stringPointer = deviceInfo_hwVersion;
            menuItem[menuItem_hardwareInfo].level = 22;
            menuItem[menuItem_hardwareInfo].function = displayHardwareVersion;
        }

    menuItem[menuItem_factoryReset].stringPointer = serviceMenuOption_factoryReset;
    menuItem[menuItem_factoryReset].level = 3;
    menuItem[menuItem_factoryReset].function = NULL;

        {
            menuItem[menuItem_partialReset].stringPointer = factoryReset_partial_string;
            menuItem[menuItem_partialReset].level = 31;
            menuItem[menuItem_partialReset].function = NULL;

                {
                    menuItem[menuItem_rejectPartialReset].stringPointer = no_option;
                    menuItem[menuItem_rejectPartialReset].level = 311;
                    menuItem[menuItem_rejectPartialReset].function = NULL;

                    menuItem[menuItem_confirmPartialReset].stringPointer = yes_option;
                    menuItem[menuItem_confirmPartialReset].level = 312;
                    menuItem[menuItem_confirmPartialReset].function = partialReset;
                }

            menuItem[menuItem_fullReset].stringPointer = factoryReset_full_string;
            menuItem[menuItem_fullReset].level = 32;
            menuItem[menuItem_fullReset].function = NULL;

                {
                        menuItem[menuItem_rejectFullReset].stringPointer = no_option;
                        menuItem[menuItem_rejectFullReset].level = 321;
                        menuItem[menuItem_rejectPartialReset].function = NULL;

                        menuItem[menuItem_confirmFullReset].stringPointer = yes_option;
                        menuItem[menuItem_confirmFullReset].level = 322;
                        menuItem[menuItem_confirmFullReset].function = fullReset;
                }
        }

}
#endif