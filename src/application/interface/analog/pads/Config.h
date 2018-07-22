/*
    Zvuk9 MIDI controller
    Copyright (C) 2014-2017 Ad Bit LLC
    Author: Igor Petrović
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    You may ONLY use this file:
    1) if you have a valid commercial Ad Bit LLC license and then in accordance with
    the terms contained in the written license agreement between you and Ad Bit LLC,
    or alternatively
    2) if you follow the terms found in GNU General Public License version 3 as
    published by the Free Software Foundation here
    <https://www.gnu.org/licenses/gpl-3.0.txt>
*/

#pragma once

///
/// \ingroup interfacePads
/// @{

///
/// \brief Number of pressure samples.
///
#define STABLE_SAMPLE_COUNT                         3

///
/// \brief Time in milliseconds after which X/Y values are being read after pad has been pressed.
/// Initial X/Y values tend to be unstable.
///
#define XY_READ_DELAY                               3

///
/// \brief Time in milliseconds during which aftertouch reading is ignored after pad has been pressed.
/// Used to avoid incorrect aftertouch activation on initial press.
///
#define AFTERTOUCH_READ_DELAY                       200

///
/// \brief Time in milliseconds during which pressure reading is ignored after X or Y has changed value.
///
#define PRESSURE_IGNORE_XY_CHANGE                   2

///
/// \brief Time in milliseconds after which notes from pad are sent once X and Y MIDI messages have been sent.
///
#define PAD_NOTE_SEND_DELAY                         (XY_READ_DELAY+3)

///
/// \brief Size of pad note buffer in bytes.
/// Pads don't send notes immediately so they're stored in buffer first instead.
/// After PAD_NOTE_SEND_DELAY milliseconds notes from buffer are sent.
///
#define PAD_NOTE_BUFFER_SIZE                        16

///
/// \brief Value used to reset last X, Y and aftertouch values so that new value can always differ from the last one.
///
#define DEFAULT_XY_AT_VALUE                         255

///
/// \brief Value used to reset last X and Y values for pitch bend so that new value can always differ from the last one.
///
#define DEFAULT_PITCH_BEND_VALUE                    16000

///
/// \brief Value used to reset last initial X and Y values so that new value can always differ from the last one.
///
#define DEFAULT_INITIAL_XY_VALUE                    16000

///
/// \brief Value range for pitch bend type 1.
/// When Pitch Bend Type 1 is used, pad areas from 25% to 75% are "dead", that is, they give PB value 0.
/// \warning These values assume 10-bit ADC resolution. Please adjust the values according to ADC resolution.
/// @{

#define PITCH_BEND_1_LOWER_MIN                      56
#define PITCH_BEND_1_LOWER_MAX                      154

#define PITCH_BEND_1_UPPER_MIN                      870
#define PITCH_BEND_1_UPPER_MAX                      960

/// @}

///
/// \brief Minimum and maximum MIDI pitch bend values.
/// @{

#define MIDI_PITCHBEND_MIN                          -8192
#define MIDI_PITCHBEND_MAX                          8191

/// @}

///
/// \brief When Pitch Bend Type 2 is used, 7% of pad surface is used for full PB range in positive, and 7% in negative direction.
/// \warning This value assume 10-bit ADC resolution. Please adjust the values according to ADC resolution.
///
#define PITCH_BEND_2_FULL_RANGE_AREA                71

///
/// \brief Area where pitch bend type 2 is zero (5% of surface).
/// This value is added to initial position (max value for dead area) and subtracted as well (min value for dead area).
/// \warning This value assume 10-bit ADC resolution. Please adjust the values according to ADC resolution.
///
#define PITCH_BEND_2_DEAD_AREA                      51

///
/// \brief Percentage used to calculate upper pressure limit on medium pressure sensitivity to send velocity.
/// Upper velocity value = Lower velocity value + (Lower velocity value * VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM%)
///
#define VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM        50

///
/// \brief Percentage used to calculate upper pressure limit on hard pressure sensitivity to send velocity.
/// Upper velocity value = Lower velocity value + (Lower velocity value * VELOCITY_PRESSURE_RATIO_UPPER_HARD%)
///
#define VELOCITY_PRESSURE_RATIO_UPPER_HARD          60

///
/// \brief Percentage used to calculate lower pressure limit to send aftertouch.
/// Lower aftertouch value = Upper pressure (without sensitivity scaling) value + (Upper pressure (velocity) value * AFTERTOUCH_PRESSURE_RATIO_LOWER%)
///
#define AFTERTOUCH_PRESSURE_RATIO_LOWER             100

///
/// \brief Percentage used to calculate upper pressure limit to send aftertouch.
/// Upper aftertouch value = Lower aftertouch value + (Lower aftertouch value * AFTERTOUCH_PRESSURE_RATIO_UPPER%)
///
#define AFTERTOUCH_PRESSURE_RATIO_UPPER             90

///
/// \brief Time in milliseconds after which aftertouch value needs to change by AFTERTOUCH_SEND_TIMEOUT_STEP in order to send new value.
/// Used to debounce rapidly changing aftertouch values.
///
#define AFTERTOUCH_SEND_TIMEOUT                     3

///
/// \brief Difference between current and previous aftertouch reading after AFTERTOUCH_SEND_TIMEOUTms used to detect value change.
///
#define AFTERTOUCH_SEND_TIMEOUT_STEP                5

///
/// \brief Time in milliseconds after which X/Y value needs to change by XY_SEND_TIMEOUT_STEP in order to send new value.
/// Used to debounce rapidly changing X/Y values.
///
#define XY_SEND_TIMEOUT                             5

///
/// \brief Minimum MIDI pressure (velocity) value necessary to send X/Y values (after initial values have been sent).
/// Used to avoid unstable X/Y values on low pressure.
///
#define XY_MIN_PRESSURE_PRESSED                     25

///
/// \brief Difference between current and previous aftertouch reading after XY_SEND_TIMEOUT used to detect value change.
///
#define XY_SEND_TIMEOUT_STEP                        3

///
/// \brief Number of zones per pad on X-axis for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_X_ZONES                5

///
/// \brief Range of values in single pressure zone on X axis.
///
#define PRESSURE_CALIBRATION_MAX_X_ZONE_VALUE       ((1023/PRESSURE_CALIBRATION_X_ZONES)+1)

///
/// \brief Number of zones per pad on Y-axis for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_Y_ZONES                5

///
/// \brief Range of values in single pressure zone on Y axis.
///
#define PRESSURE_CALIBRATION_MAX_Y_ZONE_VALUE       ((1023/PRESSURE_CALIBRATION_Y_ZONES)+1)

///
/// \brief Total number of zones per pad for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_ZONES                  (PRESSURE_CALIBRATION_X_ZONES*PRESSURE_CALIBRATION_Y_ZONES)

///
/// \brief Time in seconds after which last read pressure value on certain pressure zone is considered calibrated value in calibration mode.
///
#define PRESSURE_ZONE_CALIBRATION_TIMEOUT           5

/// @}
