#ifndef SPECIALREQUESTS_H_
#define SPECIALREQUESTS_H_

#define HELLO_REQUEST            0x48
#define COMPONENT_INFO_REQUEST   0x49
#define PROTOCOL_VERSION_REQUEST 0x50

const uint8_t specialRequestArray[] = {

    HELLO_REQUEST,
    COMPONENT_INFO_REQUEST,
    PROTOCOL_VERSION_REQUEST

};

#endif