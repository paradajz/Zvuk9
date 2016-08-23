#include "../Debug.h"

#if MODE_SERIAL < 1

#ifndef SYSEX_H_
#define SYSEX_H_

#include <avr/io.h>
#include "../midi/MIDI.h"
#include "Status.h"
#include "SpecialRequests.h"
#include "Config.h"

typedef struct {

    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

} sysExManufacturerID;

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
    uint8_t parts;

} sysExSection;

//class

class SysEx {

    public:
    SysEx();
    void handleSysEx(uint8_t *sysExArray, uint8_t size);
    void disableConf();
    void enableConf();
    bool configurationEnabled();
    bool addCustomRequest(uint8_t value);
    void addToResponse(sysExParameter_t value);

    void setHandleGet(sysExParameter_t(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index));
    void setHandleSet(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue));
    void setHandleReset(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index));
    void setHandleCustomRequest(bool(*fptr)(uint8_t value));

    bool addBlock(uint8_t sections);
    bool addSection(uint8_t block, sysExParameter_t numberOfParameters, sysExParameter_t minValue, sysExParameter_t maxValue);

    bool checkRequest();
    bool checkParameters();

    void sendCustomMessage(uint8_t id, sysExParameter_t value);

    private:

    typedef enum {

        startByte,      //0
        idByte_1,       //1
        idByte_2,       //2
        idByte_3,       //3
        statusByte,     //4
        partByte,       //5
        wishByte,       //6
        amountByte,     //7
        blockByte,      //8
        sectionByte,    //9
        REQUEST_SIZE,
        RESPONSE_SIZE = partByte + 1,
        MIN_MESSAGE_LENGTH = wishByte + 1 + 1, //add next byte and end
        ML_REQ_STANDARD = REQUEST_SIZE + 1 //add end byte

    } sysExRequestByteOrder;

    typedef enum {

        indexByte = REQUEST_SIZE,
        newValueByte_single = indexByte+sizeof(sysExParameter_t),
        newValueByte_all = indexByte

    } sysExParameterByteOrder;

    typedef enum {

        //message wish
        sysExWish_get,
        sysExWish_set,
        sysExWish_restore,
        sysExWish_backup,
        SYSEX_WISH_MAX

    } sysExWish;

    typedef enum {

        //wanted data amount
        sysExAmount_single,
        sysExAmount_all,
        SYSEX_AMOUNT_MAX

    } sysExAmount;

    typedef struct {

        //a struct containing entire info for block/message type

        uint8_t numberOfSections;
        uint8_t sectionCounter;
        sysExSection section[MAX_NUMBER_OF_SECTIONS];

    } sysExBlock;

    typedef struct {

        sysExManufacturerID id;
        sysExStatus_t status;
        sysExWish wish;
        sysExAmount amount;
        uint8_t block;
        uint8_t section;
        uint8_t part;
        sysExParameter_t index;
        sysExParameter_t newValue;

    } decodedMessage_t;

    bool checkID();
    bool checkSpecialRequests();
    bool checkWish();
    bool checkAmount();
    bool checkBlock();
    bool checkSection();
    bool checkPart();
    bool checkParameterIndex();
    bool checkNewValue();

    uint8_t generateMinMessageLenght();
    void setStatus(sysExStatus_t status);

    sysExParameter_t (*sendGetCallback)(uint8_t block, uint8_t section, sysExParameter_t index);
    bool (*sendSetCallback)(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue);
    bool (*sendResetCallback)(uint8_t block, uint8_t section, sysExParameter_t index);
    bool (*sendCustomRequestCallback)(uint8_t value);

    bool                sysExEnabled,
                        forcedSend;
    sysExBlock          sysExMessage[MAX_NUMBER_OF_BLOCKS];
    decodedMessage_t    decodedMessage;
    uint8_t             *sysExArray,
                        sysExArraySize,
                        customRequests[MAX_CUSTOM_REQUESTS],
                        customRequestCounter,
                        sysExBlockCounter,
                        responseSize;

};

extern SysEx sysEx;

#endif
#endif