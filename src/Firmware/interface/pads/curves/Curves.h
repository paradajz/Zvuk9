#pragma once

#include "DataTypes.h"
#include "../DataTypes.h"
#include "../../../board/Board.h"

///
/// \brief Class responsible for curve arithmetic.
/// @{
///
class Curves
{
    public:
    Curves();
    void init();
    uint8_t getCurveValue(curve_t type, uint8_t value, uint8_t min, uint8_t max);
    uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
    uint32_t invertRange(uint32_t value, uint32_t min, uint32_t max);

    private:
    uint8_t curveMin[NUMBER_OF_CURVES];
    uint8_t curveMax[NUMBER_OF_CURVES];
};

extern Curves curves;
/// @}
