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

#include "Curves.h"
#include "Functions.h"

///
/// \brief Default constructor.
///
Curves::Curves()
{
    
}

///
/// \brief Initialize default values and function handlers.
///
void Curves::init()
{
    //get min/max for each curve
    for (int i=0; i<NUMBER_OF_CURVES; i++)
    {
        uint8_t min_val = 255;
        uint8_t max_val = 0;

        for (int j=0; j<128; j++)
        {
            uint8_t value = pgm_read_byte(&(curveArray[i][j]));

            if (value < min_val)
                min_val = value;

            if (value > max_val)
                max_val = value;
        }

        curveMin[i] = min_val;
        curveMax[i] = max_val;
    }
}

///
/// \brief Maps value from one range to another.
/// @param [in] x Input value.
/// @param [in] in_min  Lowest possible input value.
/// @param [in] in_max  Largest possible input value.
/// @param [in] out_min Lowest possible output value.
/// @param [in] out_max Largest possible output value.
/// \returns Mapped value (uint32_t).
///
int32_t Curves::map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

///
/// \brief Calculates inverted value based on input range.
/// @param [in] value   Input value.
/// @param [in] min     Lowest possible input value.
/// @param [in] max     Largest possible input value.
/// \returns Inverted value.
///
uint32_t Curves::invertRange(uint32_t value, uint32_t min, uint32_t max)
{
    return (max + min) - value;
}

///
/// \brief Returns value for wanted curve and curve index.
/// @param [in] curve   Wanted curve.
/// @param [in] value   Wanted curve index.
/// @param [in] min     Lowest possible output value.
/// @param [in] max     Largest possible output value.
/// \returns Curve value.
///
uint8_t Curves::getCurveValue(curve_t curve, uint8_t value, uint8_t min, uint8_t max)
{
    uint8_t out_min = min < curveMin[curve] ? curveMin[curve] : min;
    uint8_t out_max = max > curveMax[curve] ? curveMax[curve] : max;

    if (out_min || (out_max != 127))
         return map(pgm_read_byte(&(curveArray[curve][value])), curveMin[curve], curveMax[curve], out_min, out_max);
    else
        return pgm_read_byte(&(curveArray[curve][value]));
}

Curves curves;
