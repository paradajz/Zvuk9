#ifndef SPECIALREQUESTS_H_
#define SPECIALREQUESTS_H_

typedef enum {

    HELLO_REQUEST,              //00
    COMPONENT_INFO_REQUEST,     //01
    PROTOCOL_VERSION_REQUEST,   //02
    PARAMS_PER_MESSAGE_REQUEST, //03
    SPECIAL_PARAMETERS

} sysEx_specialRequestID;

#endif