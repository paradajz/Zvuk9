#pragma once

#include "DataTypes.h"
#include "../DataTypes.h"

///
/// \brief Class responsible for curve arithmetic.
/// @{
///
class Curves
{
    public:
    Curves();
    void init();
    uint8_t getCurveValue(padCoordinate_t coordinate, curve_t type, uint8_t value, uint8_t min, uint8_t max);
    uint16_t map_uint16(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
    uint8_t map_uint8(uint8_t x, uint8_t in_min, uint8_t in_max, uint8_t out_min, uint8_t out_max);
    uint8_t invertRange(uint8_t value, uint8_t min, uint8_t max);

    private:
    uint8_t scale[PAD_COORDINATES][128]; //x+y+z
    uint8_t lastCurve[PAD_COORDINATES], //x+y+z
            lastMin[PAD_COORDINATES], //x+y
            lastMax[PAD_COORDINATES]; //x+y
};

extern Curves curves;
/// @}
