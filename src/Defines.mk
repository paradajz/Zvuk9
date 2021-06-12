BOARD_TARGET_BASE_DIR    := board/target
BOARD_TARGET_DIR         := $(BOARD_TARGET_BASE_DIR)/$(TARGET)
COMMAND_FW_UPDATE_START  := 0x4F70456E6E45704F
COMMAND_FW_UPDATE_END    := 0x4465436B
SYSEX_MANUFACTURER_ID_0  := 0x00
SYSEX_MANUFACTURER_ID_1  := 0x53
SYSEX_MANUFACTURER_ID_2  := 0x43
FW_METADATA_SIZE         := 4
CDC_TX_BUFFER_SIZE       := 4096
CDC_RX_BUFFER_SIZE       := 1024
UART_BAUDRATE_MIDI_STD   := 31250

SW_VERSION_MAJOR         := 0#$(shell git describe --tags --abbrev=0 | cut -c 2- | cut -d. -f1)
SW_VERSION_MINOR         := 0#$(shell git describe --tags --abbrev=0 | cut -c 2- | cut -d. -f2)
SW_VERSION_REVISION      := 0#$(shell git describe --tags --abbrev=0 | cut -c 2- | cut -d. -f3)

DEFINES += \
UART_BAUDRATE_MIDI_STD=$(UART_BAUDRATE_MIDI_STD) \
CDC_TX_BUFFER_SIZE=$(CDC_TX_BUFFER_SIZE) \
CDC_RX_BUFFER_SIZE=$(CDC_RX_BUFFER_SIZE) \
FIXED_NUM_CONFIGURATIONS=1 \
SYSEX_MANUFACTURER_ID_0=$(SYSEX_MANUFACTURER_ID_0) \
SYSEX_MANUFACTURER_ID_1=$(SYSEX_MANUFACTURER_ID_1) \
SYSEX_MANUFACTURER_ID_2=$(SYSEX_MANUFACTURER_ID_2) \
COMMAND_FW_UPDATE_START=$(COMMAND_FW_UPDATE_START) \
COMMAND_FW_UPDATE_END=$(COMMAND_FW_UPDATE_END) \
SW_VERSION_MAJOR=$(SW_VERSION_MAJOR) \
SW_VERSION_MINOR=$(SW_VERSION_MINOR) \
SW_VERSION_REVISION=$(SW_VERSION_REVISION) \
UART_TX_BUFFER_SIZE=200 \
UART_RX_BUFFER_SIZE=200

ifeq ($(DEBUG), 1)
    DEFINES += DEBUG
endif

-include $(BOARD_TARGET_DIR)/Defines.mk

ARCH                     := stm32
CPU                      := cortex-m4
FPU                      := fpv4-sp-d16
FLOAT-ABI                := hard
APP_START_ADDR           := 0x8008000
BOOT_START_ADDR          := 0x8000000
CDC_START_ADDR           := 0x8020000
FW_METADATA_LOCATION     := $(shell echo $$(($(APP_START_ADDR) + 0x190)))
MCU_FAMILY               := f4
MCU_BASE                 := stm32f405
MCU_DIR                  := board/stm32/variants/f4/stm32f405rg

#for *gen applications rewrite the arch to native type
ifneq (,$(findstring gen,$(TYPE)))
    ARCH := native
endif

ifeq ($(ARCH),stm32)
    DEFINES += \
    __STM32__ \
    STM32F405xx \
    USE_HAL_DRIVER \
    FIXED_CONTROL_ENDPOINT_SIZE=64 \
    UID_BITS=96 \
    USE_USB_FS \
    DEVICE_FS=0 \
    DEVICE_HS=1 \
    ADC_12_BIT
endif

ifeq ($(TYPE),boot)
    DEFINES += FW_BOOT
    FLASH_START_ADDR := $(BOOT_START_ADDR)
else ifeq ($(TYPE),app)
    DEFINES += FW_APP
    FLASH_START_ADDR := $(APP_START_ADDR)
else ifeq ($(TYPE),flashgen)
    #same as app
    DEFINES += FW_APP
    FLASH_START_ADDR := $(APP_START_ADDR)
else ifeq ($(TYPE),cdc)
    DEFINES += FW_CDC
    FLASH_START_ADDR := $(CDC_START_ADDR)
else ifeq ($(TYPE),sysexgen)
    #nothing to do
else
    $(error Invalid firmware type specified)
endif

DEFINES += FLASH_START_ADDR=$(FLASH_START_ADDR)
DEFINES += BOOT_START_ADDR=$(BOOT_START_ADDR)
DEFINES += APP_START_ADDR=$(APP_START_ADDR)
DEFINES += CDC_START_ADDR=$(CDC_START_ADDR)
DEFINES += FW_METADATA_LOCATION=$(FW_METADATA_LOCATION)

ifeq ($(TYPE),cdc)
    DEFINES += USE_UART
else ifneq (,$(findstring UART_CHANNEL,$(DEFINES)))
    DEFINES += USE_UART
endif