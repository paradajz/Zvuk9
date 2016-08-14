#include "SysEx.h"

#ifdef SYSEX_H_

SysEx::SysEx()  {

    sendGetCallback             = NULL;
    sendSetCallback             = NULL;
    sendResetCallback           = NULL;
    sendCustomCallback          = NULL;

    sysExEnabled = false;

    for (int i=0; i<MAX_NUMBER_OF_BLOCKS; i++)    {

        sysExMessage[i].block = INVALID_VALUE;
        sysExMessage[i].numberOfSections = INVALID_VALUE;

        for (int j=0; j<MAX_NUMBER_OF_SECTIONS; j++)    {

            sysExMessage[i].section[j].numberOfParameters = INVALID_VALUE;
            sysExMessage[i].section[j].minValue = INVALID_VALUE;
            sysExMessage[i].section[j].maxValue = INVALID_VALUE;

        }

    }

    for (int i=0; i<MAX_CUSTOM_STRINGS; i++)
        customStrings[i] = INVALID_VALUE;

    customStringCounter = 0;
    customReponseSize = 0;

}

void SysEx::enableConf()    {

    sysExEnabled = true;

}

void SysEx::disableConf()   {

    sysExEnabled = false;

}

bool SysEx::addCustomString(uint8_t value)  {

    if (customStringCounter >= MAX_CUSTOM_STRINGS) return false;

    //don't add custom string if it's already defined as one of default strings
    for (uint16_t i=0; i<sizeof(specialStringArray); i++)
        if (specialStringArray[i] == value)
            return false;

    customStrings[customStringCounter] = value;
    customStringCounter++;
    return true;

}

void SysEx::addBlock(uint8_t block, uint8_t sections) {

    //user is responsible for setting block ID
    sysExMessage[block].block = block;
    sysExMessage[block].numberOfSections = sections;

}

void SysEx::addSection(uint8_t block, uint8_t section, sysExParameter_t numberOfParameters, sysExParameter_t minValue, sysExParameter_t maxValue) {

    sysExMessage[block].section[section].numberOfParameters = numberOfParameters;
    sysExMessage[block].section[section].minValue = minValue;
    sysExMessage[block].section[section].maxValue = maxValue;

}

bool SysEx::checkSpecial(uint8_t *array, uint8_t size) {

    if (size == MIN_MESSAGE_LENGTH)   {

        uint16_t size = 0;

        switch(array[wishByte])  {

            case HELLO_STRING:
            //hello message, necessary for allowing configuration

            sendHelloResponse();
            return true;

            case PROTOCOL_VERSION_STRING:
            if (sysExEnabled)   {

                size = 0; //just in case
                addByte(defaultID.byte1, size, false);
                addByte(defaultID.byte2, size, false);
                addByte(defaultID.byte3, size, false);
                addByte(ack, size, false);
                addByte(PARAM_SIZE, size, false);
                sysExEnabled = true;
                midi.sendSysEx(sysExResponse, size, false);
                return true;

            }   else sendError(ERROR_HANDSHAKE);
            return true;

            default:
            //check for custom string
            for (int i=0; i<MAX_CUSTOM_STRINGS; i++) {

                if (customStrings[i] != INVALID_VALUE)  {

                    if (customStrings[i] == array[wishByte])    {

                        if (sysExEnabled)   {

                            size = 0;

                            //build first part of response manually
                            addByte(defaultID.byte1, size, false);
                            addByte(defaultID.byte2, size, false);
                            addByte(defaultID.byte3, size, false);
                            addByte(ack, size, false);
                            addByte(array[wishByte], size, false);
                            customReponseSize = size;
                            //now call callback handler
                            sendCustomCallback(customStrings[i]);
                            //send response
                            sysExEnabled = true;
                            midi.sendSysEx(sysExResponse, customReponseSize, false);
                            customReponseSize = 0;

                        }   else sendError(ERROR_HANDSHAKE);
                        return true;

                    }

                }

            }
            sendError(ERROR_WISH);
            return true;
            break;

        }

    }   return false;

}

void SysEx::handleSysEx(uint8_t *sysExArray, uint8_t size)    {

    if (size < MIN_MESSAGE_LENGTH) return; //ignore small messages

    //don't respond to sysex message if device ID is wrong
    sysExManufacturerID id;
    id.byte1 = sysExArray[idByte_1];
    id.byte2 = sysExArray[idByte_2];
    id.byte3 = sysExArray[idByte_3];

    if (!checkID(id)) return;

    if (checkSpecial(sysExArray, size)) return;

    //message appears to be fine for now
    //check if hello message has been received by now
    if (!sysExEnabled) {

        //message is fine, but handshake hasn't been received
        sendError(ERROR_HANDSHAKE);
        return;

    }

    if (!checkRequest(sysExArray, size)) return; //request not valid
    if (!checkParameters(sysExArray, size)) return; //invalid parameters

    //message is ok
    sendResponse(sysExArray, size);

}

bool SysEx::checkRequest(uint8_t sysExArray[], uint8_t arrSize)  {

    //first, check message length
    if (arrSize < generateMinMessageLenght((sysExWish)sysExArray[wishByte], (sysExAmount)sysExArray[amountByte], sysExArray[blockByte], sysExArray[sectionByte]))    {

        sendError(ERROR_MESSAGE_LENGTH);
        return false;

    }

    //check wish validity
    if (!checkWish((sysExWish)sysExArray[(uint8_t)wishByte]))    {

        sendError(ERROR_WISH);
        return false;

    }

    //check if wanted amount is correct
    if (!checkAmount((sysExAmount)sysExArray[(uint8_t)amountByte]))    {

        sendError(ERROR_AMOUNT);
        return false;

    }

    //check if message type is correct
    if (!checkBlock(sysExArray[(uint8_t)blockByte]))    {

        sendError(ERROR_BLOCK);
        return false;

    }

    //check if subtype is correct
    if (!checkSection(sysExArray[(uint8_t)blockByte], sysExArray[(uint8_t)sectionByte])) {

        sendError(ERROR_SECTION);
        return false;

    }

    return true;

}

bool SysEx::checkParameters(uint8_t *sysExArray, uint8_t arrSize)   {

    //sysex request is fine

    sysExParameter_t index;

    //check if wanted parameter is valid only if single parameter is specified
    if (sysExArray[amountByte] == sysExAmount_single)   {

        #if PARAM_SIZE == 2
        encDec_14bit decoded;
        decoded.high = sysExArray[indexByte_higher];
        decoded.low = sysExArray[indexByte_lower];
        index = decoded.decode14bit();
        #elif PARAM_SIZE == 1
        index = sysExArray[indexByte];
        #endif

        if (!checkParameterIndex(sysExArray[blockByte], sysExArray[sectionByte], index))  {

            sendError(ERROR_PARAMETER);
            return false;

        }

        //if message wish is set, check new parameter
        if (sysExArray[wishByte] == sysExWish_set) {

            sysExParameter_t newValue;
            #if PARAM_SIZE == 2
            encDec_14bit decoded;
            decoded.high = sysExArray[newValueByte_higher_single];
            decoded.low = sysExArray[newValueByte_lower_single];
            newValue = decoded.decode14bit();
            #elif PARAM_SIZE == 1
            newValue = sysExArray[newValueByte_single];
            #endif


            if (!checkNewValue(sysExArray[blockByte], sysExArray[sectionByte], index, newValue))  {

                sendError(ERROR_NEW_PARAMETER);
                return false;

            }

        }

    } else {

        //all parameters

        //check each new parameter for set command
        if (sysExArray[wishByte] == sysExWish_set) {

            uint8_t arrayIndex = 0;
            sysExParameter_t newValue;

            #if PARAM_SIZE == 2
            arrayIndex = newValueByte_higher_all;
            #elif PARAM_SIZE == 1
            arrayIndex = newValueByte_all;
            #endif

            for (int i=0; i<(arrSize - arrayIndex)-1; i+=sizeof(sysExParameter_t))  {

                #if PARAM_SIZE == 2
                encDec_14bit decoded;
                decoded.high = sysExArray[arrayIndex];
                decoded.low = sysExArray[arrayIndex+1];
                newValue = decoded.decode14bit();
                #elif PARAM_SIZE == 1
                newValue = sysExArray[arrayIndex];
                #endif

                if (!checkNewValue(sysExArray[blockByte], sysExArray[sectionByte], i, newValue))   {

                    sendError(ERROR_NEW_PARAMETER);
                    return false;

                }

            }

        }

    }

    return true;

}

bool SysEx::checkID(sysExManufacturerID id)   {

    return  (

        (id.byte1  == defaultID.byte1)   &&
        (id.byte2 == defaultID.byte2)   &&
        (id.byte3== defaultID.byte3)

    );

}

bool SysEx::checkWish(sysExWish wish)   {

    switch(wish)    {

        case sysExWish_get:
        case sysExWish_set:
        case sysExWish_restore:
        return true;

        default:
        return false;

    }

}

bool SysEx::checkAmount(sysExAmount amount)    {

    switch(amount)  {

        case sysExAmount_all:
        case sysExAmount_single:
        return true;

        default:
        return false;

    }

}

bool SysEx::checkBlock(uint8_t block) {

    //check if message type is valid
    for (int i=0; i<MAX_NUMBER_OF_BLOCKS; i++)
        if (sysExMessage[i].block == block)
            return true;

    return false;

}

bool SysEx::checkSection(uint8_t block, uint8_t section)    {

    return (section < sysExMessage[block].numberOfSections);

}



bool SysEx::checkParameterIndex(uint8_t block, uint8_t section, sysExParameter_t parameter)   {

    //block and section passed validation, check parameter index
    return (parameter < sysExMessage[block].section[section].numberOfParameters);

}

bool SysEx::checkNewValue(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue) {

    sysExParameter_t minValue = sysExMessage[block].section[section].minValue;
    sysExParameter_t maxValue = sysExMessage[block].section[section].maxValue;

    if ((minValue != CUSTOM_VALUE_CHECK) && (maxValue != CUSTOM_VALUE_CHECK))
        return ((newValue >= minValue) && (newValue <= maxValue));
    else if ((minValue != CUSTOM_VALUE_CHECK) && (maxValue == CUSTOM_VALUE_CHECK))
        return (newValue >= minValue); //check only min value
    else if ((minValue == CUSTOM_VALUE_CHECK) && (maxValue != CUSTOM_VALUE_CHECK))
        return (newValue <= maxValue);   //check only max value
    else if ((minValue == CUSTOM_VALUE_CHECK) && (maxValue == CUSTOM_VALUE_CHECK))
        return true; //don't check new parameter

    return false;

}

void SysEx::addByte(sysExParameter_t value, uint16_t &size, bool parameter)   {

    #if PARAM_SIZE == 2
        if (parameter)  {

            sysExResponse[size] = highByte_7bit(value);
            size++;
            sysExResponse[size] = lowByte_7bit(value);
            size++;

        }   else {

            sysExResponse[size] = value;
            size++;

        }
    #elif PARAM_SIZE == 1
        sysExResponse[size] = value;
        size++;
    #endif

}

void SysEx::addToResponse(sysExParameter_t value)    {

    //public function used to append bytes to response
    //first part of the message is already built - 3 ID bytes and custom string
    addByte(value, customReponseSize, false);

}

uint16_t SysEx::generateMinMessageLenght(sysExWish wish, sysExAmount amount, uint8_t block, uint8_t section)    {

    //single parameter
    if (amount == sysExAmount_single)  {

        if ((wish == sysExWish_get) ||
        (wish == sysExWish_restore))    return ML_REQ_STANDARD + sizeof(sysExParameter_t);      //get   //add parameter length
        else                            return ML_REQ_STANDARD + 2*sizeof(sysExParameter_t);    //set   //add parameter length and new value length

    }   else if (amount == sysExAmount_all)   {

        if ((wish == sysExWish_get) || (wish == sysExWish_restore))             //get/restore
            return ML_REQ_STANDARD;

        else    {                                                                   //set

            return ML_REQ_STANDARD + (sysExMessage[block].section[section].numberOfParameters*sizeof(sysExParameter_t));

        }

    }   else return 0;

}

void SysEx::sendHelloResponse()   {

    uint16_t size = 0;

    addByte(defaultID.byte1, size, false);
    addByte(defaultID.byte2, size, false);
    addByte(defaultID.byte3, size, false);
    addByte(ack, size, false);
    addByte(HELLO_STRING, size, false);

    sysExEnabled = true;
    midi.sendSysEx(sysExResponse, size, false);

}

void SysEx::sendError(sysExError errorID)  {

    //public function, interface objects can use this function directly
    //to override internal error checking

    uint16_t size = 0;

    addByte(defaultID.byte1, size, false);
    addByte(defaultID.byte2, size, false);
    addByte(defaultID.byte3, size, false);
    addByte(nack, size, false);
    addByte(errorID, size, false);

    midi.sendSysEx(sysExResponse, size, false);

}

void SysEx::sendComponentID(uint8_t block, sysExParameter_t index)   {

    uint16_t size = 0;

    addByte(defaultID.byte1, size, false);
    addByte(defaultID.byte2, size, false);
    addByte(defaultID.byte3, size, false);
    addByte(COMPONENT_INFO_STRING, size, false);
    addByte(block, size, false);
    addByte(index, size, true);

    midi.sendSysEx(sysExResponse, size, false);

}

void SysEx::sendResponse(uint8_t sysExArray[], uint8_t arraySize)  {

    uint8_t numberOfParameters  = 1,
            _parameter          = 0;

    uint16_t size = 0;
    uint8_t arrayIndex = 0;
    bool eepromError = false;

    //build basic response
    addByte(defaultID.byte1, size, false);
    addByte(defaultID.byte2, size, false);
    addByte(defaultID.byte3, size, false);
    addByte(ack, size, false);

    if (sysExArray[amountByte] == sysExAmount_all) {

        uint8_t block = sysExArray[blockByte];
        uint8_t section = sysExArray[sectionByte];
        numberOfParameters = sysExMessage[block].section[section].numberOfParameters;

    }

    //create response based on wanted message type
    switch(sysExArray[wishByte]) {

        case sysExWish_get:
        if (sysExArray[amountByte] == sysExAmount_all)  _parameter = 0;
        else                                            _parameter = sysExArray[indexByte_higher];

        for (int i=0; i<numberOfParameters; i++) {

            addByte(sendGetCallback(sysExArray[blockByte], sysExArray[sectionByte], _parameter), size, true);
            _parameter++;

        }
        break;

        //case sysExWish_set:
        //if (sysExArray[amountByte] == sysExAmount_all) {
//
            //_parameter = 0;
            //arrayIndex = MS_NEW_PARAMETER_ID_ALL;
//
        //}   else    {
//
            //_parameter = sysExArray[MS_PARAMETER_ID];
            //arrayIndex = MS_NEW_PARAMETER_ID_SINGLE;
//
        //}
//
        //for (int i=0; i<numberOfParameters; i++)   {
//
            //if (!sendSetCallback(sysExArray[blockByte], sysExArray[sectionByte], _parameter, sysExArray[arrayIndex+i]))  {
//
                //eepromError = true;
                //break;
//
            //}
//
            //_parameter++;
//
        //}   arrayLength = ML_SET_RESTORE;
        //break;

        //case sysExWish_restore:
        //if (sysExArray[amountByte] == sysExAmount_all)
        //_parameter = 0;
        //else _parameter = sysExArray[MS_PARAMETER_ID];
//
        //for (int i=0; i<componentNr; i++)   {
//
            //if (!sendResetCallback(sysExArray[blockByte], sysExArray[sectionByte], _parameter))  {
//
                //eepromError = true;
                //break;
//
            //}   _parameter++;
//
        //}   arrayLength = ML_SET_RESTORE;
        //break;

    }

    if (!eepromError)   midi.sendSysEx(sysExResponse, size, false);
    else                sendError(ERROR_EEPROM);

}

//callbacks

void SysEx::setHandleGet(sysExParameter_t(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index))    {

    sendGetCallback = fptr;

}

void SysEx::setHandleSet(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index, sysExParameter_t newValue))    {

    sendSetCallback = fptr;

}

void SysEx::setHandleReset(bool(*fptr)(uint8_t block, uint8_t section, sysExParameter_t index))    {

    sendResetCallback = fptr;

}

void SysEx::setHandleCustom(bool(*fptr)(uint8_t value))  {

    sendCustomCallback = fptr;

}

bool SysEx::configurationEnabled()  {

    return sysExEnabled;

}

SysEx sysEx;
#endif