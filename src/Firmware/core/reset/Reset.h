#pragma once

///
/// \brief Provides helper functions to facilitate software reset on MCU.
/// \defgroup reset Reset
/// \ingroup core
/// @{

///
/// \brief Disables all peripherals present on MCU.
///
void disablePeripherals();

///
/// \brief Initiates watchdog software MCU reset by setting watch-dog timeout and waiting until watchdog is activated.
///
void mcuReset();

///
/// \brief Used to reset watchdog timer on MCU startup.
///
void wdt_init() __attribute__((naked)) __attribute__((section(".init3")));

/// @}