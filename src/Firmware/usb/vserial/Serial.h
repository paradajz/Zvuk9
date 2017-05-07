#ifdef DEBUG
#pragma once
#include "Descriptors.h"

#include "../../lufa/LUFA/Drivers/USB/USB.h"
#include "../../lufa/LUFA/Platform/Platform.h"

/* Function Prototypes: */
void SetupHardware(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

class Serial
{
    public:
    Serial();
    void init();
    void update();

    private:
};

extern Serial vserial;

#endif
