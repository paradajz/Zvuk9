#include "../Debug.h"

#if MODE_SERIAL < 1

#ifndef SYSEX_H_
#define SYSEX_H_

#include <avr/io.h>
#include "../midi/MIDI.h"
#include "Errors.h"
#include "SpecialStrings.h"
#include "Config.h"

#define MAX_NUMBER_OF_BLOCKS    7
#define MAX_NUMBER_OF_SECTIONS  7
#define MAX_CUSTOM_STRINGS      5

#define INVALID_VALUE           128
#define CUSTOM_VALUE_CHECK      255

#define CONFIG_TIMEOUT          60000   //1 minute

typedef struct {

    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

} sysExManufacturerID;

const sysExManufacturerID defaultID = {

    SYS_EX_M_ID_0,
    SYS_EX_M_ID_1,
    SYS_EX_M_ID_2

};

#if PARAM_SIZE == 2
typedef uint16_t sysExParameter_t;
#elif PARAM_SIZE == 1
typedef uint8_t sysExParameter_t;
#else
#error Incorrect parameter size for SysEx
#endif

typedef struct {

    sysExParameter_t numberOfParameters;
    sysExParameter_t minValue;
    sysExParameter_t maxValue;

} sysExSection;

//class

class SysEx {

    public:
    SysEx();
    void handleSysEx(uint8_t *sysExArray, uint8_t size);
    void disableConf();
    void enableConf();
    bool configurationEnabled();
    void sendError(sysExError errorID);
    bool addCustomString(uint8_t value);
    void addToResponse(sysExParameter_t value);

    void setHandleGet(sysExParameter_t(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index));
    void setHandleSet(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue));
    void setHandleReset(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index));
    void setHandleCustom(bool(*fptr)(uint8_t value));

    void addBlock(uint8_t block, uint8_t sections);
    void addSection(uint8_t block, uint8_t section, sysExParameter_t numberOfParameters, sysExParameter_t minValue, sysExParameter_t maxValue);

    bool checkRequest(uint8_t *sysExArray, uint8_t arrSize);
    bool checkParameters(uint8_t *sysExArray, uint8_t arrSize);
    void sendResponse(uint8_t sysExArray[], uint8_t arraySize);

    void sendComponentID(uint8_t block, sysExParameter_t index);

    private:

    typedef enum {

        startByte,
        idByte_1,
        idByte_2,
        idByte_3,
        wishByte,
        amountByte,
        blockByte,
        sectionByte,
        MIN_MESSAGE_LENGTH = wishByte + 1 + 1, //add next byte and end
        ML_REQ_STANDARD = MIN_MESSAGE_LENGTH + 1 + 1 + 1 //min length + amount + block + section

    } sysExRequestByteOrder;

    typedef enum {

        #if PARAM_SIZE == 2
        indexByte_higher = sectionByte+1,
        indexByte_lower = indexByte_higher+1,
        newValueByte_higher_single = indexByte_lower+1,
        newValueByte_lower_single = newValueByte_higher_single+1,
        newValueByte_higher_all = indexByte_higher,
        newValueByte_lower_all = indexByte_lower
        #elif PARAM_SIZE == 1
        indexByte = sectionByte+1,
        newValueByte_single = indexByte+1,
        newValueByte_all = indexByte
        #endif

    } sysExParameterByteOrder;

    typedef enum {

        //message wish
        sysExWish_get,
        sysExWish_set,
        sysExWish_restore,
        sysExWish_special

    } sysExWish;

    typedef enum {

        //wanted data amount
        sysExAmount_single,
        sysExAmount_all

    } sysExAmount;

    typedef struct {

        //a struct containing entire info for block/message type

        uint8_t block;
        uint8_t numberOfSections;
        sysExSection section[MAX_NUMBER_OF_SECTIONS];

    } sysExBlock;

    typedef enum {

        ack = 0x41,
        nack = 0x46

    } sysExStatus;

    uint8_t sysExResponse[MIDI_SYSEX_ARRAY_SIZE];
    uint8_t customStrings[MAX_CUSTOM_STRINGS];
    uint8_t customStringCounter;
    uint16_t customReponseSize;

    bool checkID(sysExManufacturerID id);
    bool checkSpecial(uint8_t *array, uint8_t size);
    bool checkWish(sysExWish wish);
    bool checkAmount(sysExAmount amount);
    bool checkBlock(uint8_t block);
    bool checkSection(uint8_t block, uint8_t section);
    bool checkParameterIndex(uint8_t block, uint8_t section, sysExParameter_t index);
    bool checkNewValue(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue);

    void addByte(sysExParameter_t value, uint16_t &size, bool parameter);

    uint16_t generateMinMessageLenght(sysExWish wish, sysExAmount amount, uint8_t block, uint8_t section);
    void sendHelloResponse();

    sysExParameter_t (*sendGetCallback)(uint8_t block, uint8_t section, sysExParameter_t index);
    bool (*sendSetCallback)(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue);
    bool (*sendResetCallback)(uint8_t block, uint8_t section, sysExParameter_t index);
    bool (*sendCustomCallback)(uint8_t value);

    bool        sysExEnabled;
    sysExBlock  sysExMessage[MAX_NUMBER_OF_BLOCKS];

};

extern SysEx sysEx;

#endif
#endif