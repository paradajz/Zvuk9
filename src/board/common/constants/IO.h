#pragma once

/// Time in milliseconds which needs to pass after the pad value has reached the PAD_PRESS_PRESSURE threshold.
/// Once the time passes, current reading is considered velocity.
#define PAD_VELOCITY_TIMEOUT 25

/// Time in milliseconds after which the pad is really considered released once the pressure value has dropped below PAD_PRESS_PRESSURE.
#define PAD_RELEASE_DEBOUNCE_TIMEOUT 25