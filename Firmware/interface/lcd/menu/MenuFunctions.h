#include "../LCD.h"

#include <avr/boot.h>

uint16_t dev_signature[20];

static inline void USB_Device_GetSerialString_test()    //testing only
{

    uint8_t curr_int = SREG;
    cli();

    uint8_t SigReadAddress = 0x0E;

    printf("Device serial number: ");

    for (uint8_t SerialCharNum = 0; SerialCharNum < 20; SerialCharNum++)
    {
        uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);

        if (SerialCharNum & 0x01)
        {
            SerialByte >>= 4;
            SigReadAddress++;
        }

        SerialByte &= 0x0F;

        dev_signature[SerialCharNum] = /*cpu_to_le16*/((SerialByte >= 10) ?
        (('A' - 10) + SerialByte) : ('0' + SerialByte));

        printf("%d", dev_signature[SerialCharNum]);
    }   printf("\n");

    SREG = curr_int;
}

void factory_reset(factoryResetType_t type) {

    #ifdef MODULE_LEDS
    leds.setFadeSpeed(1);
    leds.allLEDsOff();
    #endif
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

    USB_Device_GetSerialString_test();

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
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);

}


void enableCalibration_y()    {

    pads.setCalibrationMode(true, coordinateY);
    display.clearLine(1);
    display.clearLine(2);
    display.clearLine(3);

}