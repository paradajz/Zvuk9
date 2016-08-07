#ifndef SERVICE_MENU_OPTIONS_H_
#define SERVICE_MENU_OPTIONS_H_

typedef enum {

    serviceMenuItem_calibration,
    serviceMenuItem_deviceInfo,
    serviceMenuItem_factoryReset,

    serviceMenuItem_calibrateX,
    serviceMenuItem_calibrateY,
    serviceMenuItem_calibratePressure,

    SERVICE_MENU_ITEMS

} serviceMenuItems_t;

#endif