MCU_FAMILY := f4
MCU_BASE := stm32f405
MCU := stm32f405rg
MCU_DIR := board/stm32/variants/f4/stm32f405rg
DEFINES += HSE_VALUE=16000000
DEFINES += BOARD_STRING=\"zvuk9\"
DEFINES += USB_MIDI_SUPPORTED
DEFINES += DIN_MIDI_SUPPORTED
DEFINES += UART_CHANNEL_DIN=2
DEFINES += NUMBER_OF_BUTTON_COLUMNS=8
DEFINES += NUMBER_OF_BUTTON_ROWS=8
DEFINES += MAX_NUMBER_OF_BUTTONS=64
DEFINES += ENCODERS_SUPPORTED
DEFINES += MAX_NUMBER_OF_ENCODERS=32
DEFINES += LEDS_SUPPORTED
DEFINES += NUMBER_OF_LED_COLUMNS=8
DEFINES += NUMBER_OF_LED_ROWS=3
DEFINES += MAX_NUMBER_OF_LEDS=24
DEFINES += MAX_NUMBER_OF_RGB_LEDS=8
DEFINES += LED_EXT_INVERT
DEFINES += ANALOG_SUPPORTED
DEFINES += NUMBER_OF_MUX=4
DEFINES += NUMBER_OF_MUX_INPUTS=16
DEFINES += MAX_ADC_CHANNELS=4
DEFINES += MAX_NUMBER_OF_ANALOG=64
DEFINES += NUMBER_OF_PADS=9
DEFINES += NUMBER_OF_IN_SR=1
DEFINES += FW_UID=$(shell ../scripts/fw_uid_gen.sh $(TARGET))