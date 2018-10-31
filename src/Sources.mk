#common include files
INCLUDE_FILES += -include "application/Zvuk9.h"

#common include directories
INCLUDE_DIRS := \
-I"modules/lufa/" \
-I"modules/" \
-I"application/" \
-I"application/board/avr/"

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

    #u8x8 sources
    SOURCES += modules/u8g2/csrc/u8x8_string.c
    SOURCES += modules/u8g2/csrc/u8x8_setup.c
    SOURCES += modules/u8g2/csrc/u8x8_u8toa.c
    SOURCES += modules/u8g2/csrc/u8x8_8x8.c
    SOURCES += modules/u8g2/csrc/u8x8_u16toa.c
    SOURCES += modules/u8g2/csrc/u8x8_display.c
    SOURCES += modules/u8g2/csrc/u8x8_fonts.c
    SOURCES += modules/u8g2/csrc/u8x8_byte.c
    SOURCES += modules/u8g2/csrc/u8x8_cad.c
    SOURCES += modules/u8g2/csrc/u8x8_gpio.c
    SOURCES += modules/u8g2/csrc/u8x8_d_ssd1322.c

    ifeq ($(findstring rls,$(MAKECMDGOALS)), rls)
        SOURCES += \
        modules/lufa/LUFA/Drivers/USB/Class/Device/AudioClassDevice.c \
        modules/lufa/LUFA/Drivers/USB/Class/Device/MIDIClassDevice.c

        #filter out usb cdc
        SOURCES := $(filter-out %vserial/VSerial.c %vserial/Descriptors.c,$(SOURCES))
    else
        SOURCES += \
        modules/lufa/LUFA/Drivers/USB/Class/Device/CDCClassDevice.c

        #filter out usb midi
        SOURCES := $(filter-out %usb/midi/Descriptors.c %usb/midi/USB_MIDI.cpp,$(SOURCES))
    endif

    SOURCES += application/Zvuk9.cpp
else
    #bootloader
    SOURCES += \
    bootloader/mcu/BootloaderHID.c \
    bootloader/mcu/Descriptors.c \
    modules/lufa/LUFA/Drivers/USB/Class/Common/HIDParser.c \
    modules/lufa/LUFA/Drivers/USB/Class/Device/HIDClassDevice.c
endif

SOURCES := $(filter-out %tests-all.cpp,$(SOURCES))