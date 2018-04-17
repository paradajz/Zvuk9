#common defines
DEFINES := \
ARCH=ARCH_AVR8 \
F_CPU=16000000UL \
F_USB=16000000UL \
BOARD=BOARD_NONE \
USE_STATIC_OPTIONS=0 \
USB_DEVICE_ONLY \
FIXED_CONTROL_ENDPOINT_SIZE=8 \
FIXED_NUM_CONFIGURATIONS=1 \
HID_VENDOR_ID=0x03EB \
HID_PRODUCT_ID=0x2067 \
MIDI_VENDOR_ID=0x1209 \
MIDI_PRODUCT_ID=0x8472 \
MIDI_BAUD_RATE_STD=31250 \

#flash type specific
ifeq ($(findstring boot,$(MAKECMDGOALS)), boot)
    DEFINES += \
    ORDERED_EP_CONFIG \
    NO_SOF_EVENTS \
    USE_RAM_DESCRIPTORS \
    NO_INTERNAL_SERIAL \
    DEVICE_STATE_AS_GPIOR \
    NO_DEVICE_REMOTE_WAKEUP \
    NO_DEVICE_SELF_POWER
else
    DEFINES += \
    USE_FLASH_DESCRIPTORS \
    INTERRUPT_CONTROL_ENDPOINT \
    MIDI_SYSEX_ARRAY_SIZE=45 \
    RING_BUFFER_SIZE=50 \
    LED_INVERT
endif

ifeq ($(findstring debug,$(MAKECMDGOALS)), debug)
    DEFINES += \
    DEBUG
else
    DEFINES += \
    NDEBUG \
    START_UP_ANIMATION
endif

#board specific
ifeq ($(findstring upload,$(MAKECMDGOALS)), upload)
    #hack used to set MCU if only make upload target is called
    #check if MCU file exists
    ifneq ("$(wildcard build/MCU)","")
        MCU := $(shell cat build/MCU)
    else
        $(error Please run make for specific target first)
    endif
else
    MCU := at90usb1286
    DEFINES += STRING_BUFFER_SIZE=40
endif

#mcu specific
ifeq ($(MCU),at90usb1286)
    FUSE_UNLOCK := 0x3f
    FUSE_EXT := 0xf8
    FUSE_HIGH := 0xd2
    FUSE_LOW := 0xff
    FUSE_LOCK := 0x2f
    EEPROM_SIZE := 4096
    BOOT_START_ADDR := 0x1F000
    FLASH_SIZE_START_ADDR := 0x98
    FLASH_SIZE_END_ADDR := 0x9C
endif

DEFINES += BOOT_START_ADDR=$(BOOT_START_ADDR)
DEFINES += EEPROM_SIZE=$(EEPROM_SIZE)
#for database, total size is three bytes smaller than full eeprom
#one byte for reboot selection and two for crc
DEFINES += LESSDB_SIZE=$(shell echo $(EEPROM_SIZE)-3 | bc)
