//
//  protocol.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/14/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef protocol_hpp
#define protocol_hpp


typedef BYTE DEVICE_ID [DEVICE_ID_LENGTH];

enum packetType
{
    TYPE_NULL            = 0,
    TYPE_HEARTBEAT       = 1,
    TYPE_HEARTBEAT_REPLY = 2,
    TYPE_REGISTER        = 3,
    TYPE_REGISTER_REPLY  = 4,
    TYPE_DATA            = 5,
    TYPE_MULTI           = 6,
    TYPE_ACK             = 7
};

//----------------------------------------------------------------------------------
typedef struct standardPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
} standardPacket;

typedef struct heartbeatPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    BYTE      deviceCount;
    DEVICE_ID registeredDevices[DEVICE_LIST_MAX];
} heartbeatPacket;

typedef struct heartbeatReplyPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID clientDeviceID;
    BYTE      accessPointReceiveStrength;
} heartbeatReplyPacket;

typedef struct registerPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID clientDeviceID;
} registerPacket;

typedef struct registerReplyPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID clientDeviceID;
    BYTE internetConnected;
} registerReplyPacket;

typedef struct dataPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      messageTotalLength;
    BYTE      messageBody[MAX_MESSAGE_LENGTH];
} dataPacket;

typedef struct multiPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      sequenceNumber;
    WORD      messageFragmentLength;
    BYTE      messageBody[MAX_MESSAGE_LENGTH];
} multiPacket;

typedef struct ackPacket_t
{
    BYTE      typeOfPacket;
    DEVICE_ID accessPointDeviceID;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      sequenceNumber;
} ackPacket;

typedef union packets_t
{
    standardPacket       standard;
    heartbeatPacket      heartbeat;
    heartbeatReplyPacket heartbeatReply;
    registerPacket       registration;
    registerReplyPacket  registrationReply;
    dataPacket           data;
    multiPacket          multi;
    ackPacket            ack;
} packets;


#endif /* protocol_hpp */
