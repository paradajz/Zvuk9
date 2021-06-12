vpath modules/%.cpp ../
vpath modules/%.c ../

#common include dirs
INCLUDE_DIRS := \
-I"../modules/" \
-I"$(BOARD_TARGET_DIR)/" \
-I"application/" \
-I"board/$(ARCH)/variants/$(MCU_FAMILY)" \
-I"$(MCU_DIR)" \
-I"./"

ifeq (,$(findstring gen,$(TYPE)))
    ifeq ($(ARCH),stm32)
        INCLUDE_DIRS += \
        $(addprefix -I,$(shell $(FIND) ./board/stm32/gen/common -type d -not -path "*Src*")) \
        $(addprefix -I,$(shell $(FIND) ./board/stm32/gen/$(MCU_FAMILY)/common -type d -not -path "*Src*")) \
        $(addprefix -I,$(shell $(FIND) ./board/stm32/gen/$(MCU_FAMILY)/$(MCU_BASE)/Drivers -type d -not -path "*Src*")) \
        -I"./board/stm32/variants/$(MCU_FAMILY)/$(MCU)"
    endif

    ifeq ($(TYPE),boot)
        INCLUDE_DIRS += \
        -I"bootloader/"
    endif
endif

LINKER_FILE := $(MCU_DIR)/$(MCU).ld

ifeq (,$(findstring gen,$(TYPE)))
    #architecture specific
    ifeq ($(ARCH),stm32)
        SOURCES += $(shell $(FIND) ./board/stm32/gen/common -regex '.*\.\(s\|c\)')
        SOURCES += $(shell $(FIND) ./board/stm32/gen/$(MCU_FAMILY)/common -regex '.*\.\(s\|c\)')
        SOURCES += $(shell $(FIND) ./board/stm32/gen/$(MCU_FAMILY)/$(MCU_BASE) -regex '.*\.\(s\|c\)')
        SOURCES += $(shell $(FIND) ./board/stm32/variants/$(MCU_FAMILY) -maxdepth 1 -name "*.cpp")
        SOURCES += modules/EmuEEPROM/src/EmuEEPROM.cpp
    endif

    SOURCES += $(shell $(FIND) ./board/common -maxdepth 1 -type f -name "*.cpp")
    SOURCES += $(shell $(FIND) ./$(MCU_DIR) -maxdepth 1 -type f -regex '.*\.\(s\|c\|cpp\)')
    SOURCES += $(BOARD_TARGET_DIR)/Pins.cpp

    ifeq ($(TYPE),boot)
        #bootloader sources
        #common
        SOURCES += \
        board/common/bootloader/Bootloader.cpp \
        board/$(ARCH)/common/Bootloader.cpp \
        board/$(ARCH)/common/Init.cpp \
        board/$(ARCH)/common/ShiftRegistersWait.cpp

        ifeq ($(ARCH),avr)
            SOURCES += board/$(ARCH)/common/Flash.cpp
            SOURCES += board/$(ARCH)/common/FlashPages.cpp
        else ifeq ($(ARCH),stm32)
            SOURCES += board/$(ARCH)/common/ISR.cpp
        endif

        SOURCES += $(shell find ./bootloader -type f -name "*.cpp")

        ifneq (,$(findstring USB_MIDI_SUPPORTED,$(DEFINES)))
            SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/midi -type f -name "*.cpp")
            SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/midi -type f -name "*.c")
            SOURCES += $(shell $(FIND) ./board/$(ARCH)/comm/usb/midi -type f -name "*.cpp")

            ifneq (,$(findstring USB_LINK_MCU,$(DEFINES)))
                #for USB link MCUs, compile UART as well - needed to communicate with main MCU
                SOURCES += \
                board/$(ARCH)/comm/uart/UART.cpp \
                board/common/comm/uart/UART.cpp
            endif
        else
            SOURCES += \
            board/$(ARCH)/comm/uart/UART.cpp \
            board/common/comm/uart/UART.cpp

            SOURCES += $(shell $(FIND) ./board/common/comm/USBMIDIOverSerial -type f -name "*.cpp")
        endif
    else ifeq ($(TYPE),app)
        #application sources
        #common for all targets
        SOURCES += $(shell $(FIND) ./board/$(ARCH)/common -type f -name "*.cpp")

        ifneq (,$(findstring USB_MIDI_SUPPORTED,$(DEFINES)))
            SOURCES += $(shell $(FIND) ./board/$(ARCH)/comm/usb/midi -type f -name "*.cpp")
            SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/midi -type f -name "*.cpp")
            SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/midi -type f -name "*.c")
        endif

        ifneq (,$(findstring USE_UART,$(DEFINES)))
            SOURCES += \
            board/$(ARCH)/comm/uart/UART.cpp \
            board/common/comm/uart/UART.cpp
        endif

        # SOURCES += $(shell $(FIND) ./application -type f -name "*.cpp")
        SOURCES += application/main.cpp

        SOURCES += $(shell $(FIND) ./board/common/io -type f -name "*.cpp")

        #u8x8 sources
        SOURCES += \
        modules/u8g2/csrc/u8x8_string.c \
        modules/u8g2/csrc/u8x8_setup.c \
        modules/u8g2/csrc/u8x8_u8toa.c \
        modules/u8g2/csrc/u8x8_8x8.c \
        modules/u8g2/csrc/u8x8_u16toa.c \
        modules/u8g2/csrc/u8x8_display.c \
        modules/u8g2/csrc/u8x8_fonts.c \
        modules/u8g2/csrc/u8x8_byte.c \
        modules/u8g2/csrc/u8x8_cad.c \
        modules/u8g2/csrc/u8x8_gpio.c \
        modules/u8g2/csrc/u8x8_d_ssd1306_128x64_noname.c \
        modules/u8g2/csrc/u8x8_d_ssd1306_128x32.c
    else ifeq ($(TYPE),cdc)
        #cdc mode sources
        #stm32 only
        SOURCES += \
        board/$(ARCH)/common/Init.cpp \
        board/$(ARCH)/common/ShiftRegistersWait.cpp \
        board/$(ARCH)/common/ISR.cpp \
        board/$(ARCH)/comm/uart/UART.cpp \
        board/common/comm/uart/UART.cpp

        SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/cdc -type f -name "*.cpp")
        SOURCES += $(shell $(FIND) ./board/common/comm/usb/descriptors/cdc -type f -name "*.c")
        SOURCES += $(shell $(FIND) ./board/$(ARCH)/comm/usb/cdc -type f -name "*.cpp")
        SOURCES += $(shell find ./cdc -type f -name "*.cpp")
    endif
else ifeq ($(TYPE),flashgen)
    SOURCES += $(shell $(FIND) ./application/database -type f -name "*.cpp")
    SOURCES += $(shell $(FIND) ../modules/dbms/src -maxdepth 1 -type f -name "*.cpp" | sed "s|^\.\./||")
    SOURCES += modules/EmuEEPROM/src/EmuEEPROM.cpp
    SOURCES += $(MCU_DIR)/FlashPages.cpp
    SOURCES += flashgen/main.cpp
else ifeq ($(TYPE),sysexgen)
    SOURCES += sysexgen/main.cpp
endif

#make sure all objects are located in build directory
OBJECTS := $(addprefix $(BUILD_DIR)/,$(SOURCES))
#also make sure objects have .o extension
OBJECTS := $(addsuffix .o,$(OBJECTS))

#include generated dependency files to allow incremental build when only headers change
-include $(OBJECTS:%.o=%.d)