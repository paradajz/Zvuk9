#include "Pins.h"

namespace
{
    const core::io::mcuPin_t aInPins[MAX_ADC_CHANNELS] = {
        CORE_IO_MCU_PIN_DEF(MUX_PORT_INPUT_0, MUX_PIN_INPUT_0),
        CORE_IO_MCU_PIN_DEF(MUX_PORT_INPUT_1, MUX_PIN_INPUT_1),
        CORE_IO_MCU_PIN_DEF(MUX_PORT_INPUT_2, MUX_PIN_INPUT_2),
        CORE_IO_MCU_PIN_DEF(MUX_PORT_INPUT_3, MUX_PIN_INPUT_3),
    };

    const core::io::mcuPin_t dOutPins[] = {
        CORE_IO_MCU_PIN_DEF(LED_ROW_PORT_0, LED_ROW_PIN_0),
        CORE_IO_MCU_PIN_DEF(LED_ROW_PORT_1, LED_ROW_PIN_1),
        CORE_IO_MCU_PIN_DEF(LED_ROW_PORT_2, LED_ROW_PIN_2),
    };
}    // namespace

#include "board/common/Map.cpp.include"