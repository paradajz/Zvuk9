#include "../LCD.h"

void factory_reset(factoryResetType_t type) {

    leds.setFadeSpeed(1);
    leds.allLEDsOff();
    configuration.factoryReset(type);
    reboot();

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

    factory_reset(factoryReset_partial);

}

void fullReset()    {

    factory_reset(factoryReset_restore);

}

void enableCalibration_x()    {

    pads.setCalibrationMode(true, coordinateX);
    display.displayCalibration(coordinateX);
    pads.getXLimits();

}


void enableCalibration_y()    {

    pads.setCalibrationMode(true, coordinateY);
    display.displayCalibration(coordinateY);

}