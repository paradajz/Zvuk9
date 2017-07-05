#pragma once

///
/// \brief Counts after which pad press is considered stable once pad press has been detected.
/// Debouncing time is 20 milliseconds. Divide time with PAD_READOUT_TIME to get counts instead of milliseconds to save RAM.
///
#define PAD_DEBOUNCE_TIME                           1

///
/// \brief Time in milliseconds after which notes from pad are sent once X and Y MIDI messages have been sent.
///
#define PAD_NOTE_SEND_DELAY                         5

///
///\brief Percentage used to calculate upper pressure limit on medium pressure sensitivity to send velocity.
/// Upper velocity value = Lower velocity value + (Lower velocity value * VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM%)
///
#define VELOCITY_PRESSURE_RATIO_UPPER_MEDIUM        30

///
///\brief Percentage used to calculate upper pressure limit on hard pressure sensitivity to send velocity.
/// Upper velocity value = Lower velocity value + (Lower velocity value * VELOCITY_PRESSURE_RATIO_UPPER_HARD%)
///
#define VELOCITY_PRESSURE_RATIO_UPPER_HARD          50

///
///\brief Percentage used to calculate lower pressure limit to send aftertouch.
/// Lower aftertouch value = Upper pressure (without sensitivity scaling) value + (Upper pressure (velocity) value * AFTERTOUCH_PRESSURE_RATIO_LOWER%)
///
#define AFTERTOUCH_PRESSURE_RATIO_LOWER             70

///
///\brief Percentage used to calculate upper pressure limit to send aftertouch.
/// Upper aftertouch value = Lower aftertouch value + (Lower aftertouch value * AFTERTOUCH_PRESSURE_RATIO_UPPER%)
///
#define AFTERTOUCH_PRESSURE_RATIO_UPPER             11

///
/// \brief Time in milliseconds after which aftertouch value needs to change by AFTERTOUCH_SEND_TIMEOUT_STEP in order to send new value.
/// Used to debounce rapidly changing aftertouch values.
///
#define AFTERTOUCH_SEND_TIMEOUT                     (100/PAD_READOUT_TIME)+1

///
/// \brief Difference between current and previous aftertouch reading after AFTERTOUCH_SEND_TIMEOUTms used to detect value change.
///
#define AFTERTOUCH_SEND_TIMEOUT_STEP                3

///
/// \brief Time in milliseconds after which X/Y value needs to change by XY_SEND_TIMEOUT_STEP in order to send new value.
/// Used to debounce rapidly changing X/Y values.
///
#define XY_SEND_TIMEOUT                             (35/PAD_READOUT_TIME)

///
/// \brief Difference between current and previous aftertouch reading after XY_SEND_TIMEOUT used to detect value change.
///
#define XY_SEND_TIMEOUT_STEP                        3

///
/// \brief Offset values used to compensate slightly inaccurate calibration when calibrating X/Y coordinates.
/// @{
///
#define X_MIN_CALIBRATION_OFFSET                    10
#define X_MAX_CALIBRATION_OFFSET                    -10
/// @}

///
/// \brief Number of zones per pad on X-axis for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_X_ZONES                4

///
/// \brief Range of values in single pressure zone on X axis.
///
#define PRESSURE_CALIBRATION_MAX_X_ZONE_VALUE       ((1023/PRESSURE_CALIBRATION_X_ZONES)+1)

///
/// \brief Number of zones per pad on Y-axis for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_Y_ZONES                4

///
/// \brief Range of values in single pressure zone on Y axis.
///
#define PRESSURE_CALIBRATION_MAX_Y_ZONE_VALUE       ((1023/PRESSURE_CALIBRATION_Y_ZONES)+1)

///
/// \brief Total number of zones per pad for which calibration data is stored.
///
#define PRESSURE_CALIBRATION_ZONES                  (PRESSURE_CALIBRATION_X_ZONES*PRESSURE_CALIBRATION_Y_ZONES)

///
/// \brief Hardcoded lower pressure limit for all pads/zones.
/// Only upper pressure limit is being calibrated.
///
#define DEFAULT_PAD_PRESSURE_LIMIT_LOWER            40

///
/// \brief Time in seconds after which last read pressure value on certain pressure zone is considered calibrated value in calibration mode.
///
#define PRESSURE_ZONE_CALIBRATION_TIMEOUT           3
