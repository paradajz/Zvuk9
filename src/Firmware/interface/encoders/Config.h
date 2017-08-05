#pragma once

///
/// \brief Used to achieve linear encoder acceleration on fast movement.
/// Every time fast movement is detected, amount of steps is increased by this value.
///
#define ENCODER_SPEED_CHANGE            2

///
/// \brief Time threshold (in milliseconds, approximately) between two encoder steps used to detect fast movement.
///
#define SPEED_TIMEOUT                   20