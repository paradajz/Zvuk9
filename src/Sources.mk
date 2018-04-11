SOURCES :=

#lufa sources
#common for bootloader and firmware
SOURCES += \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Device_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/EndpointStream_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Endpoint_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/PipeStream_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Pipe_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Template/Template_Endpoint_Control_R.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Template/Template_Endpoint_Control_W.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Template/Template_Endpoint_RW.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/Template/Template_Pipe_RW.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/USBController_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/AVR8/USBInterrupt_AVR8.c \
modules/lufa/LUFA/Drivers/USB/Core/ConfigDescriptors.c \
modules/lufa/LUFA/Drivers/USB/Core/DeviceStandardReq.c \
modules/lufa/LUFA/Drivers/USB/Core/Events.c \
modules/lufa/LUFA/Drivers/USB/Core/USBTask.c

#only for firmware
ifneq ($(findstring boot,$(MAKECMDGOALS)), boot)

    SOURCES += $(shell find application/board -name "*.cpp")
    SOURCES += $(shell find application/board -name "*.c")
    SOURCES += $(shell find application/database -name "*.cpp")
    SOURCES += $(shell find application/interface -name "*.cpp")
    SOURCES += $(shell find modules/midi -name "*.cpp")
    SOURCES += $(shell find modules/dbms -name "*.cpp")
    SOURCES += $(shell find modules/core -name "*.cpp")
    SOURCES += $(shell find modules/u8g2/csrc -name "*.c")

    SOURCES += application/Zvuk9.cpp

    ifeq ($(findstring rls,$(MAKECMDGOALS)), rls)
        SOURCES += \
        modules/lufa/LUFA/Drivers/USB/Class/Device/AudioClassDevice.c \
        modules/lufa/LUFA/Drivers/USB/Class/Device/MIDIClassDevice.c
    else
        SOURCES += \
        modules/lufa/LUFA/Drivers/USB/Class/Device/CDCClassDevice.c

        SOURCES += $(shell find application/vserial -name "*.cpp")
        SOURCES += $(shell find application/vserial -name "*.c")

        #filter out usb midi here, cdc is used instead in debug
        SOURCES := $(filter-out %usb/Descriptors.c %USB_MIDI.cpp,$(SOURCES))
    endif
else
    #bootloader
    SOURCES += \
    bootloader/BootloaderHID.c \
    bootloader/Descriptors.c \
    modules/lufa/LUFA/Drivers/USB/Class/Common/HIDParser.c \
    modules/lufa/LUFA/Drivers/USB/Class/Device/HIDClassDevice.c
endif

#some modules contain Tests.cpp which should not be compiled
#remove Array.cpp from DBMS module used for testing purposes
SOURCES := $(filter-out %Tests.cpp,$(SOURCES))