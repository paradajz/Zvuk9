#pragma once

///
/// \brief Provides delay function and interface to accommodate MCU run time in milliseconds.
/// \defgroup timer Timer
/// \ingroup core
/// @{

///
/// \brief Definition of variable holding current MCU run time. Must be implemented externally in order to
/// use rTimeMs() function correctly.
///
extern volatile uint32_t rTime_ms;

///
///\brief Date-time structure.
///
typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} dateTime_t;

///
/// \brief Delays for desired time interval in milliseconds.
/// This function makes use of built-in _delay_ms function. Function is called repeatedly with argument 1 until
/// ms parameter reaches 0, since _delay_ms accepts only constant known at compile-time.
/// @param [in] ms  Delay time in milliseconds.
///
void wait_ms(uint32_t ms);

///
/// \brief Returns amount of time MCU has been running in milliseconds.
/// Actual incrementation of rTime_ms must be done externally.
/// \returns Runtime in milliseconds.
///
uint32_t rTimeMs();

/// @}