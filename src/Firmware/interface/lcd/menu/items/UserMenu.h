#pragma once

typedef enum
{
    userMenuItem_pressureSettings,
    userMenuItem_pressureSensitivity,
    userMenuItem_pressureCurve,
    userMenuItem_midiSettings,
    userMenuItem_deviceInfo,
    userMenuItem_factoryReset,

    userMenuItem_pressureSensitivity_1,
    userMenuItem_pressureSensitivity_2,
    userMenuItem_pressureSensitivity_3,

    userMenuItem_pressureCurve_linear,
    userMenuItem_pressureCurve_log,
    userMenuItem_pressureCurve_exp,

    userMenuItem_midiSettings_aftertouchType,
    userMenuItem_midiSettings_runningStatus,
    userMenuItem_midiSettings_noteOff,
    userMenuItem_midiSettings_transport,

    userMenuItem_midiSettings_aftertouchType_channel,
    userMenuItem_midiSettings_aftertouchType_poly,

    userMenuItem_midiSettings_noteOff_channel,
    userMenuItem_midiSettings_noteOff_zero_vel,

    userMenuItem_midiSettings_runningStatus_enabled,
    userMenuItem_midiSettings_runningStatus_disabled,

    userMenuItem_midiSettings_transport_mmc,
    userMenuItem_midiSettings_transport_cc,
    userMenuItem_midiSettings_transport_mmc_cc,

    USER_MENU_ITEMS
} userMenuItems_t;
