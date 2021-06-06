#include "board/Board.h"

namespace Board
{
    namespace detail
    {
        ///
        /// \brief Initializes all pins to correct states.
        ///
        void initPins();

        ///
        /// \brief Initializes USB peripheral and configures it as MIDI device.
        ///
        void initUSB_MIDI();

        ///
        /// \brief Initializes UART peripheral used to send and receive MIDI data.
        ///
        void initUART_MIDI();

        ///
        /// \brief Initializes main and PWM timers.
        ///
        void initTimers();

        ///
        /// \brief Initializes pads and ADC peripheral.
        ///
        void initPads();
    }    // namespace detail
}    // namespace Board
