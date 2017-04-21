#pragma once

///
/// \brief Setup and manipulation of ADC peripheral.
/// \defgroup adc ADC
/// \ingroup core
/// @{

///
/// \brief Starts ADC conversion by setting ADSC bit in ADCSRA register.
///
#define startADCconversion() (ADCSRA |= (1<<ADSC))

///
/// \brief Enables ADC interrupts by setting ADIE bit in ADCSRA register.
///
#define adcInterruptEnable() (ADCSRA |= (1<<ADIE))

///
/// \brief Sets up ADC according to parameters specified in Config.h
///
void setUpADC();

///
/// \brief Sets active ADC channel
/// @param[in] channel ADC Channel.
///
void setADCchannel(uint8_t channel);

///
/// \brief Get ADC value from last set ADC channel.
/// \return Value from ADC registers (ADCH and ADCL).
///
uint16_t getADCvalue();

///
/// \brief Disable digital input circuitry on specified ADC channel to reduce noise.
/// @param[in] channel ADC Channel.
///
void disconnectDigitalInADC(uint8_t channel);

/// @}