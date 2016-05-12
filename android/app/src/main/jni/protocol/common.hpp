//
//  common.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>


#define DEVICE_DEBUG


#define DEVICE_ID_LENGTH 6
#define DEVICE_LIST_MAX 6
#define MAX_MESSAGE_LENGTH 48


#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t


#include "protocol.hpp"
#include "device.hpp"
#include "interface.hpp"
#include "test.hpp"


//----------------------------------------------------------------------------------
// protocol.cpp function prototypes

void protocolInitialize (void);

void sendHeartbeat (packets * pHeartbeatPacket, DEVICE_ID myDeviceID, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void sendHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID, BYTE apRSSI);
void sendRegistration (packets * pRegistration, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID);
void sendRegistrationReply (packets * pRegistrationPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID);
void sendData (packets * pDataPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD * hash, BYTE * pMessageBody, BYTE * pMessageBytesSent);
void sendMulti (packets * pMultiPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody, BYTE * pMessageBytesSent);
void sendDataAck (packets * pAckPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber);

void recPacket ( DEVICE_ID receivingDeviceID, packets * newPacket);

void recHeartbeat  (packets * pHeartbeatPacket, DEVICE_ID * apDeviceID, BYTE * pReceiveSignalStrength, BYTE * deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void recHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * pReceiveSignalStrength);
void recRegistration (packets * pRegistrationPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID);
void recRegistrationReply (packets * pRegistrationReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * internetConnected);
void recData (packets * pDataPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * messageTotalLength, BYTE * messageFragmentLength, DWORD * hash, BYTE * * ppMessageBody);
void recMultiData (packets * pMultiPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, BYTE * messageFragmentLength, DWORD * hash, WORD * sequenceNumber, BYTE * * ppMessageBody);
void recDataAck (packets * pAckPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * hash, WORD * sequenceNumber);


DWORD generateHash (BYTE * pMessageBody, WORD messageLength);

// from https://rosettacode.org/wiki/CRC-32#Implementation
uint32_t rc_crc32 (uint32_t crc, const char *buf, size_t len);

// From: http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
uint32_t GetMilliCount (void);
uint32_t GetMilliSpan (uint32_t nTimeStart);


//----------------------------------------------------------------------------------
// device.cpp function prototypes

sDeviceInfo * initializeDeviceInfo (DEVICE_ID accessPointDeviceID, DEVICE_ID deviceID, BYTE initialDeviceRole);
void runDeviceStateMachine (void);
void transmitPacket (packets * packet);
void receivePacket (void);
void removeFirstPacketFromQueue (void);

void framePacket (BYTE * * ppFramedPacketData, WORD * pFramedPacketLength, BYTE * pPacketData, BYTE packetLength);
void extractPacket (BYTE * * ppPacketData, BYTE * pPacketLength, BYTE * pFramedPacketData, WORD framePacketLength);

void CreateDeviceID (DEVICE_ID * newDeviceID, DWORD value);
BYTE CompareDeviceID (DEVICE_ID aDeviceID, DEVICE_ID bDeviceID);
void CopyDeviceID (DEVICE_ID * pDestination, DEVICE_ID source);
void CopyDeviceIDs (DEVICE_ID * pDestination, DEVICE_ID * pSource, BYTE count);


//----------------------------------------------------------------------------------
// interface.cpp function prototypes
void getDeviceInfo (sDeviceInfo * pDeviceInfo);
void getRegionDeviceInfo (sDeviceInfo * pDeviceInfo, sRegionDeviceInfo * pRegionDeviceInfo);

void registerWithAp (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID);  // void (* pRegisterCallBackFunction)() );
void unregisterWithAp (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID);  // void (* pUnregisterCallBackFunction)() );

void registerRequest (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID); // void (* pRegisterRequestCallBackFunction)() );
void unregisterRequest (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID); // void (* pUnregisterRequestCallBackFunction)() );

void sendMessage (sDeviceInfo * pDeviceInfo, DEVICE_ID destinationDeviceID, BYTE * pMessageBody); // void (* pSendMessageCallBackFunction)() );
void sendMessageCallBack (sDeviceInfo * pDeviceInfo, sMessage * pMessage);

void getMessage (sDeviceInfo * pDeviceInfo, sMessage * * ppMessage);


void processHeartbeat (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, BYTE receiveSignalStrength, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs);
void processHeartbeatReply (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE receiveSignalStrength);

void processRegistration (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID);
void processRegistrationReply (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE * pInternetConnected);

void processData (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody);
void processMulti (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody);
void processAck (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, DWORD hash, WORD sequenceNumber);


void sManagerInit (void);
void sendHeaderAndData (sDeviceInfo * pDeviceInfo, DEVICE_ID destinationDeviceID, BYTE * pMessageBody);
void sendSessionData (sSendManager * pSendManager);

void receiveHeaderAndData (DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody);
void receiveSessionData (DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody);

#endif /* common_h */
