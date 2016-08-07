#include "../Debug.h"

#if MODE_SERIAL < 1

#ifndef SYSEX_H_
#define SYSEX_H_

#include <avr/io.h>
#include "../Types.h"
#include "Version.h"
#include "ManufacturerID.h"
#include "../midi/MIDI.h"
#include "Errors.h"
#include "SpecialStrings.h"

#define MAX_NUMBER_OF_MESSAGES  7
#define MAX_NUMBER_OF_SUBTYPES  7
#define SUBTYPE_FIELDS          4

#define PARAMETERS_BYTE         0
#define NEW_VALUE_MIN_BYTE      1
#define NEW_VALUE_MAX_BYTE      2

#define INVALID_VALUE           128
#define IGNORE_NEW_VALUE        255

typedef struct {

    uint8_t parameters;
    uint8_t lowValue;
    uint8_t highValue;

} subtype;

#define CONFIG_TIMEOUT          60000   //1 minute

//message length
#define ML_SPECIAL              0x06
#define ML_REQ_STANDARD         0x09
#define ML_RES_BASIC            0x08
#define ML_SET_RESTORE          0x04

#define ENABLE                  0x01
#define DISABLE                 0x00

#define RESPONSE_ACK            0x41
#define RESPONSE_NACK           0x46

//definitions

typedef enum {

    //message wish
    WISH_START,
    WISH_GET = WISH_START,
    WISH_SET,
    WISH_RESTORE,
    WISH_END

} sysExWish;

typedef enum {

    //wanted data amount
    AMOUNT_START,
    AMOUNT_SINGLE = AMOUNT_START,
    AMOUNT_ALL,
    AMOUNT_END

} sysExAmount;

typedef enum {

    MS_M_ID_0 = 1,
    MS_M_ID_1 = 2,
    MS_M_ID_2 = 3,
    MS_WISH = 4,
    MS_AMOUNT = 5,
    MS_BLOCK = 6,
    MS_SECTION = 7,
    MS_PARAMETER_ID = 8,
    MS_NEW_PARAMETER_ID_SINGLE = 9,
    MS_NEW_PARAMETER_ID_ALL = 8,

} sysExMessageByteOrder;

typedef struct {

    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

} sysExManufacturerID;

//class

class SysEx {

    public:
    SysEx();
    void handleSysEx(uint8_t *sysExArray, uint8_t size);
    void disableConf();
    void enableConf();
    bool configurationEnabled();
    void sendError(sysExError errorID);

    void setHandleReboot(void(*fptr)(void));
    void setHandleFactoryReset(void(*fptr)(void));
    void setHandleGet(uint8_t(*fptr)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameterID));
    void setHandleSet(bool(*fptr)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameterID, uint8_t newParameter));
    void setHandleReset(bool(*fptr)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameterID));

    void addMessageType(uint8_t messageID, uint8_t subTypes);
    void addMessageSubType(uint8_t messageID, uint8_t subTypeId, uint8_t numberOfParameters, uint8_t minValue, uint8_t maxValue);

    bool checkMessageValidity(uint8_t *sysExArray, uint8_t arrSize);
    void sendResponse(uint8_t sysExArray[], uint8_t arraySize);

    void sendComponentID(uint8_t blockID, uint8_t componentID);

    private:

    typedef struct {

        //a struct containing entire info for message type
        //first variable is message ID
        //second variable is total subtype number for message
        //subTypeInfo array has following layout:
        //index 0: subtype index
        //index 1: number of parameters in subtype
        //index 2: minimum value for new parameter
        //index 3: maximum value for new parameter

        uint8_t messageTypeID;
        uint8_t numberOfSubtypes;
        uint8_t subTypeInfo[MAX_NUMBER_OF_SUBTYPES][SUBTYPE_FIELDS];

    } sysExMessageTypeInfo;

    bool checkID(sysExManufacturerID id);
    bool checkSpecial(uint8_t *array, uint8_t size);
    bool checkWish(uint8_t wish);
    bool checkAmount(uint8_t amount);
    bool checkBlock(uint8_t messageType);
    bool checkSection(uint8_t messageType, uint8_t messageSubType);
    bool checkParameterID(uint8_t messageType, uint8_t messageSubType, uint8_t parameter);
    bool checkNewParameter(uint8_t messageType, uint8_t messageSubType, uint8_t parameter, uint8_t newParameter);

    uint8_t generateMinMessageLenght(uint8_t wish, uint8_t amount, uint8_t messageType, uint8_t messageSubType);
    void sendHelloResponse();

    void (*sendRebootCallback)(void);
    void (*sendFactoryResetCallback)(void);
    uint8_t (*sendGetCallback)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameter);
    bool (*sendSetCallback)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameter, uint8_t newParameter);
    bool (*sendResetCallback)(uint8_t messageID, uint8_t messageSubtype, uint8_t parameter);

    bool                    sysExEnabled;
    bool                    specialMessageChecked;
    bool                    dataAvailable;
    sysExMessageTypeInfo    messageInfo[MAX_NUMBER_OF_MESSAGES];

};

extern SysEx sysEx;

#endif
#endif