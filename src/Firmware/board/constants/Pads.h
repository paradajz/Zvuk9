#pragma once

#include "Timer.h"
#include "Hardware.h"
#include "../DataTypes.h"

///
/// \brief Hardcoded lower pressure limit for all pads/zones.
/// Only upper pressure limit is being calibrated.
///
#define DEFAULT_PAD_PRESSURE_LIMIT_LOWER            20