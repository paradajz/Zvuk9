/*
             LUFA Library
     Copyright (C) Dean Camera, 2015.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2015  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the Mass Storage class bootloader. This file contains the complete bootloader logic.
 */

#define  INCLUDE_FROM_BOOTLOADER_MASSSTORAGE_C
#include "BootloaderMassStorage.h"
#include "Pins.h"
#include "PinManipulation.h"
#include <util/crc16.h>

#define LAST_FLASH_ADDRESS  0x1DFFE

/** LUFA Mass Storage Class driver interface configuration and state information. This structure is
 *  passed to all Mass Storage Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_MS_Device_t Disk_MS_Interface =
	{
		.Config =
			{
				.InterfaceNumber           = INTERFACE_ID_MassStorage,
				.DataINEndpoint            =
					{
						.Address           = MASS_STORAGE_IN_EPADDR,
						.Size              = MASS_STORAGE_IO_EPSIZE,
						.Banks             = 1,
					},
				.DataOUTEndpoint           =
					{
						.Address           = MASS_STORAGE_OUT_EPADDR,
						.Size              = MASS_STORAGE_IO_EPSIZE,
						.Banks             = 1,
					},
				.TotalLUNs                 = 1,
			},
	};

/** Flag to indicate if the bootloader should be running, or should exit and allow the application code to run
 *  via a soft reset. When cleared, the bootloader will abort, the USB interface will shut down and the application
 *  started via a forced watchdog reset.
 */
bool RunBootloader = true;

/** Indicates if the bootloader is allowed to exit immediately if \ref RunBootloader is \c false. During shutdown all
 *  pending commands must be processed before jumping to the user-application, thus this tracks the main program loop
 *  iterations since a SCSI command from the host was received.
 */
static uint8_t TicksSinceLastCommand = 0;

void checkNewRevision() {

    //this function calculates application CRC
    //CRC is already written at last flash address
    //if two don't match, new code is uploaded
    //update eeprom counter

    uint32_t addr;
    uint8_t byte_u8;
    uint16_t crc_u16;

    /* Compute the CRC */
    crc_u16 = 0;

    for (addr=0; addr<LAST_FLASH_ADDRESS; addr++)   {

        byte_u8 = pgm_read_byte_far(addr);
        crc_u16 = _crc16_update(crc_u16, byte_u8);

    }

    if (pgm_read_word_far(LAST_FLASH_ADDRESS) == crc_u16)
    {
        //no updates
    }
    else
    {
        GPIOR0 = 0xFF;
        eeprom_update_byte((uint8_t*)4090, crc_u16 & 0xFF);
        eeprom_update_byte((uint8_t*)4091, (crc_u16 >> 8) & 0xFF);
        //return true; //new code
    }

}

//check whether we should jump to bootloader or application
void Application_Jump_Check(void)
{
    bool JumpToApplication = false;

    //make sure bootloader button is configured as input
    setInputMacro(BTLDR_BUTTON_DDR, BTLDR_BUTTON_PIN_INDEX);

    //a bit of cheating - HWBE fuse is unprogrammed, however, we are using HWB pin to
    //detect whether we should run bootloader or not
    //check if buton on HWB pin is released
    if (!((BTLDR_BUTTON_PIN_REGISTER >> BTLDR_BUTTON_PIN_INDEX) & 0x01))  {

        //bootloader button is released
        JumpToApplication = true;

    }

    /* Clear external reset source if source is external*/
    if (!(MCUSR & (1 << EXTRF)))
    MCUSR &= ~(1 << EXTRF);

    /* Don't run the user application if the reset vector is blank (no app loaded) */
    bool ApplicationValid = (pgm_read_word_near(0) != 0xFFFF);

    /* If a request has been made to jump to the user application, honor it */
    if (JumpToApplication && ApplicationValid)
    {
        /* Turn off the watchdog */
        MCUSR &= ~(1 << WDRF);
        wdt_disable();

        checkNewRevision();
        //GPIOR0 = 0xFF;
        // cppcheck-suppress constStatement
        ((void (*)(void))0x0000)();
    }
}

/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
    SetupHardware();

    GlobalInterruptEnable();

    while (RunBootloader || TicksSinceLastCommand++ < 0xFF)
    {
        MS_Device_USBTask(&Disk_MS_Interface);
        USB_USBTask();
    }

    /* Disconnect from the host - USB interface will be reset later along with the AVR */
    USB_Detach();

    /* Enable the watchdog and force a timeout to reset the AVR */
    wdt_enable(WDTO_250MS);

    for (;;);
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
static void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Relocate the interrupt vector table to the bootloader section */
    MCUCR = (1 << IVCE);
    MCUCR = (1 << IVSEL);

    /* Hardware Initialization */
    USB_Init();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs. */
void EVENT_USB_Device_Connect(void)
{

}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs and stops the Mass Storage management task.
 */
void EVENT_USB_Device_Disconnect(void)
{

}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup Mass Storage Data Endpoints */
	ConfigSuccess &= MS_Device_ConfigureEndpoints(&Disk_MS_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	MS_Device_ProcessControlRequest(&Disk_MS_Interface);
}

/** Mass Storage class driver callback function the reception of SCSI commands from the host, which must be processed.
 *
 *  \param[in] MSInterfaceInfo  Pointer to the Mass Storage class interface configuration structure being referenced
 */
bool CALLBACK_MS_Device_SCSICommandReceived(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo)
{
	bool CommandSuccess;

	CommandSuccess = SCSI_DecodeSCSICommand(MSInterfaceInfo);

	/* Signal that a command was processed, must not exit bootloader yet */
	TicksSinceLastCommand = 0;

	return CommandSuccess;
}
