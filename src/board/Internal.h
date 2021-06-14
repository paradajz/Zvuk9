#pragma once

#include <stddef.h>
#include <inttypes.h>
#include "midi/src/MIDI.h"
#include "core/src/general/IO.h"

#ifdef __STM32__
#include "EmuEEPROM/src/EmuEEPROM.h"
#endif

#ifndef __AVR__
#include <vector>
#endif

//for internal board usage only - do not include/call in application directly

namespace Board
{
    namespace detail
    {
        /// Specifies incoming or outgoing MIDI data traffic.
        enum class midiTrafficDirection_t : uint8_t
        {
            incoming,
            outgoing
        };

        /// Default error handler.
        void errorHandler();

        namespace setup
        {
            /// Prepares MCU to run application.
            void application();

            /// Prepares MCU to run bootloader.
            void bootloader();

            /// Prepares MCU to run as CDC device.
            void cdc();

            /// Initializes all used clocks on the board.
            void clocks();

            /// Initializes all pins to correct states.
            void io();

            /// Initializes analog variables and ADC peripheral.
            void adc();

            /// Initializes USB peripheral and configures it as MIDI device.
            void usb();

            /// Initializes all used timers on board.
            void timers();
        }    // namespace setup

        namespace UART
        {
            namespace ll
            {
                //low-level UART API, MCU specific

                /// Enables the firing of interrupt once the UART data register is empty.
                /// This effectively starts the process of transmitting the data from UART TX buffer to UART interface.
                /// param [in]: channel     UART channel on MCU.
                void enableDataEmptyInt(uint8_t channel);

                /// Disables the firing of interrupt once the UART data register is empty.
                /// param [in]: channel     UART channel on MCU.
                void disableDataEmptyInt(uint8_t channel);

                /// Performs low-level initialization of the specified UART channel.
                /// param [in]: channel     UART channel on MCU.
                /// param [in]: baudRate    UART speed (baudRate).
                bool init(uint8_t channel, uint32_t baudRate);

                /// Performs low-level deinitialization of the specified UART channel.
                /// param [in]: channel UART channel on MCU.
                bool deInit(uint8_t channel);
            }    // namespace ll

            /// Used to store incoming data from UART to buffer.
            /// param [in]: channel UART channel on MCU.
            /// param [in]: data    Received data.
            void storeIncomingData(uint8_t channel, uint8_t data);

            /// Retrieves the next byte from the outgoing ring buffer.
            /// param [in]: channel             UART channel on MCU.
            /// param [in,out]: data            Reference to variable in which next byte to send is stored.
            /// param [in,out]: remainingBytes  Reference to variable in which total number of bytes remanining in buffer is stored.
            /// returns: True if byte has been successfully retrieved, false otherwise (buffer is empty).
            bool getNextByteToSend(uint8_t channel, uint8_t& data, size_t& remainingBytes);

            /// Used to indicate that the transmission is complete.
            /// param [in]: channel UART channel on MCU.
            void indicateTxComplete(uint8_t channel);
        }    // namespace UART

        namespace map
        {
            typedef struct
            {
                core::io::mcuPin_t miso;
                core::io::mcuPin_t mosi;
                core::io::mcuPin_t sck;
            } SPIpins_t;

            typedef struct
            {
                core::io::mcuPin_t sda;
                core::io::mcuPin_t sdl;
            } I2Cpins_t;

            typedef struct
            {
                uint32_t address;
                uint32_t size;
            } flashPage_t;

            /// Used to retrieve physical ADC channel for a given MCU pin.
            uint32_t adcChannel(const core::io::mcuPin_t& pin);

            /// Used to retrieve physical ADC channel for a given ADC channel index.
            uint32_t adcChannel(uint8_t index);

            /// Used to retrieve ADC port and pin for a given ADC channel index.
            const core::io::mcuPin_t& adcPin(uint8_t index);

            /// Used to retrieve physical analog component index for a given user-specified index.
            uint8_t adcIndex(uint8_t index);

            /// Used to retrieve button port and pin for a given button index.
            const core::io::mcuPin_t& buttonPin(uint8_t index);

            /// Used to retrieve physical button component index for a given user-specified index.
            uint8_t buttonIndex(uint8_t index);

            /// Used to retrieve LED port and pin for a given LED index.
            const core::io::mcuPin_t& ledPin(uint8_t index);

            /// Used to physical LED component index for a given user-specified index.
            uint8_t ledIndex(uint8_t index);

            /// Used to retrieve unused port and pin for a given index.
            const core::io::mcuPin_t& unusedPin(uint8_t index);

            /// Used to retrieve state of unused port and pin for a given index.
            bool unusedPinState(uint8_t index);

            /// Retrieves flash page descriptor containing page address and size.
            /// param [in]: pageIndex Index of flash sector for which to retrieve address and size.
            /// returns: Reference to flash page descriptor for specified page index.
            flashPage_t& flashPageDescriptor(size_t pageIndex);

            /// Used to retrieve descriptors for flash pages used for EEPROM emulation.
            /// @ {

            size_t eepromFlashPageFactory();
            size_t eepromFlashPage1();
            size_t eepromFlashPage2();

            /// }

#ifdef __STM32__
            class STMPeripheral
            {
                public:
                STMPeripheral() = default;

                virtual std::vector<core::io::mcuPin_t> pins()         = 0;
                virtual void*                           interface()    = 0;
                virtual IRQn_Type                       irqn()         = 0;
                virtual void                            enableClock()  = 0;
                virtual void                            disableClock() = 0;
            };

            /// Used to retrieve physical UART interface used on MCU for a given UART channel index as well
            /// as pins on which the interface is connected.
            STMPeripheral* uartDescriptor(uint8_t channel);

            /// Used to retrieve physical I2C interface used on MCU for a given I2C channel index as well
            /// as pins on which the interface is connected.
            STMPeripheral* i2cDescriptor(uint8_t channel);

            /// Used to retrieve UART channel on board for a specified UART interface.
            /// If no channels are mapped to the provided interface, return false.
            bool uartChannel(USART_TypeDef* interface, uint8_t& channel);

            /// Used to retrieve I2C channel on board for a specified UART interface.
            /// If no channels are mapped to the provided interface, return false.
            bool i2cChannel(I2C_TypeDef* interface, uint8_t& channel);

            /// Used to retrieve physical ADC interface used on MCU.
            ADC_TypeDef* adcInterface();

            /// Used to retrieve timer instance used for main timer interrupt.
            TIM_TypeDef* mainTimerInstance();
#endif
        }    // namespace map

        namespace io
        {
            /// Enumerated order at which pads are being read.
            enum class padReadOrder_t : uint32_t
            {
                readPressure0,    /// First pressure reading.
                readPressure1,    /// Second pressure reading from opposite conductor.
                readPressure2,    /// First pressure reading.
                readPressure3,    /// Second pressure reading from opposite conductor.
                readX,            /// X coordinate.
                readY,            /// Y coordinate.
                AMOUNT
            };

            /// Continuously reads all digital inputs.
            void checkDigitalInputs();

            /// Removes all readings from digital inputs.
            void flushInputReadings();

            /// Checks if digital outputs need to be updated.
            void checkDigitalOutputs();

            /// Used to indicate that the MIDI event has occured using built-in LEDs on board.
            /// param [source]     Source of MIDI data. See MIDI::interface_t enumeration.
            /// param [direction]  Direction of MIDI data. See midiTrafficDirection_t enumeration.
            void indicateMIDItraffic(MIDI::interface_t source, midiTrafficDirection_t direction);

            /// Checks if indicator LEDs need to be turned on or off.
            void checkIndicators();

            /// Flashes integrated LEDs on board on startup to indicate that application is about to be loaded.
            void ledFlashStartup();

            /// MCU-specific delay routine used when setting 74HC595 shift register state.
            void sr595wait();

            /// MCU-specific delay routine used when setting 74HC165 shift register state.
            void sr165wait();

            /// Used to temporarily configure all common multiplexer pins as outputs to minimize
            /// the effect of channel-to-channel crosstalk.
            void dischargeMux();

            /// Used to restore pin setup for specified multiplexer.
            void restoreMux(uint8_t muxIndex);
        }    // namespace io

        namespace isrHandling
        {
            /// Global ISR handler for all UART events.
            /// param [in]: channel UART channel on MCU.
            void uart(uint8_t channel);

            /// Called in ADC ISR once the conversion is done.
            /// param [in]: adcValue    Retrieved ADC value.
            void adc(uint16_t adcValue);

            /// Global ISR handler for main timer.
            void mainTimer();
        }    // namespace isrHandling

        namespace cdc
        {
            /// Checks if incoming UART buffer contains data.
            /// If it does, it creates a new USB CDC packet containing that data.
            /// Should be checked periodicaly in timer.
            void checkIncomingData();
        }    // namespace cdc

        namespace flash
        {
            /// Checks whether the specified flash address is valid / in range for the current MCU.
            bool isInRange(uint32_t address);

            /// Retrieves total flash size in bytes.
            uint32_t size();

            /// Retrieves size of specified flash page.
            uint32_t pageSize(size_t index);

            /// Erases specified flash page.
            bool erasePage(size_t index);

            /// Writes filled page buffer to flash page if needed/supported.
            void writePage(size_t index);

            /// Write 16-bit data to specified address in flash memory.
            bool write16(uint32_t address, uint16_t data);

            /// Write 32-bit data to specified address in flash memory.
            bool write32(uint32_t address, uint32_t data);

            /// Read 8-bit data from specified address in flash memory.
            bool read8(uint32_t address, uint8_t& data);

            /// Read 16-bit data from specified address in flash memory.
            bool read16(uint32_t address, uint16_t& data);

            /// Read 32-bit data from specified address in flash memory.
            bool read32(uint32_t address, uint32_t& data);
        }    // namespace flash
    }        // namespace detail
}    // namespace Board