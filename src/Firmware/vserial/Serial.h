//this intents to replicate arduino serial print functions

#include "../Debug.h"

#if MODE_SERIAL > 0

#ifndef VSERIAL_H_
#define VSERIAL_H_

#include "../hardware/usb/vserial/Descriptors.h"

#include "../hardware/usb/lufa/LUFA/Drivers/USB/USB.h"
#include "../hardware/usb/lufa/LUFA/Platform/Platform.h"

/* Function Prototypes: */
void SetupHardware(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

class Serial {

    public:
    Serial();
    void init();
    void update();

    private:

};

extern Serial vserial;

#endif
#endif