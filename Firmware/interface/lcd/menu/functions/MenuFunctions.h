#include "../../LCD.h"

#ifdef LCD_H_

#ifndef MENU_FUNCTIONS_H_
#define MENU_FUNCTIONS_H_

#include "../../../../init/Init.h"

#include "../../../pads/Pads.h"

#include <avr/boot.h>

//uint16_t dev_signature[20];
//
//static inline void USB_Device_GetSerialString_test()    //testing only
//{
//
    //uint8_t curr_int = SREG;
    //cli();
//
    //uint8_t SigReadAddress = 0x0E;
//
    //printf("Device serial number: ");
//
    //for (uint8_t SerialCharNum = 0; SerialCharNum < 20; SerialCharNum++)
    //{
        //uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);
//
        //if (SerialCharNum & 0x01)
        //{
            //SerialByte >>= 4;
            //SigReadAddress++;
        //}
//
        //SerialByte &= 0x0F;
//
        //dev_signature[SerialCharNum] = /*cpu_to_le16*/((SerialByte >= 10) ?
        //(('A' - 10) + SerialByte) : ('0' + SerialByte));
//
        //printf("%d", dev_signature[SerialCharNum]);
    //}   printf("\n");
//
    //SREG = curr_int;
//}

bool factoryReset(uint8_t type);

bool deviceInfo(uint8_t arg = 0);

bool padEditMode(uint8_t arg = 0);

bool checkCalibration(uint8_t arg = 0);

bool enableCalibration(uint8_t type);

bool changeOctaveButtons(uint8_t arg = 0);

#endif
#endif