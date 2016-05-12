//
//  protocol.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/14/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "common.hpp"


DEVICE_ID nullDeviceID;

//----------------------------------------------------------------------------------
void protocolInitialize (void)
{
    CreateDeviceID (& nullDeviceID, 0);
}

// access points (AP's) and repeaters send this packet periodically
void sendHeartbeat (packets * pHeartbeatPacket, DEVICE_ID myDeviceID, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs)
{
    pHeartbeatPacket->standard.typeOfPacket = TYPE_HEARTBEAT;
    CopyDeviceID (& pHeartbeatPacket->heartbeat.accessPointDeviceID, myDeviceID);
    pHeartbeatPacket->heartbeat.deviceCount = deviceCount;
    CopyDeviceIDs (pHeartbeatPacket->heartbeat.registeredDevices, pArrayOfDeviceIDs, deviceCount);
}

// clients, AP's and repeaters send this packet to reply heartbeats from remote AP's and repeaters
void sendHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID, BYTE apRSSI)
{
    pHeartbeatReplyPacket->standard.typeOfPacket = TYPE_HEARTBEAT_REPLY;
    CopyDeviceID (& pHeartbeatReplyPacket->heartbeatReply.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pHeartbeatReplyPacket->heartbeatReply.clientDeviceID,      myDeviceID);
    pHeartbeatReplyPacket->heartbeatReply.accessPointReceiveStrength = apRSSI;
}

// clients register themselves with AP's/repeaters who they want to use to route messages for them
void sendRegistration (packets * pRegistration, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID)
{
    pRegistration->standard.typeOfPacket = TYPE_REGISTER;
    CopyDeviceID (& pRegistration->registration.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pRegistration->registration.clientDeviceID,      myDeviceID);
}

// AP's/repeaters reply to client and remote AP's/repeater registrations
void sendRegistrationReply (packets * pRegistrationPacket, DEVICE_ID apDeviceID, DEVICE_ID myDeviceID, BYTE internetConnected)
{
    pRegistrationPacket->standard.typeOfPacket = TYPE_REGISTER_REPLY;
    CopyDeviceID (& pRegistrationPacket->registrationReply.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pRegistrationPacket->registrationReply.clientDeviceID,      myDeviceID);
    pRegistrationPacket->registrationReply.internetConnected = internetConnected;
}

// clients, AP's and repeaters send messages using this packet tothe send the heder and initial data fragment
void sendData (packets * pDataPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD * pHash, BYTE * pMessageBody, BYTE * pMessageBytesSent)
{    
    pDataPacket->standard.typeOfPacket = TYPE_DATA;
    CopyDeviceID (& pDataPacket->data.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pDataPacket->data.destinationDeviceID, destDeviceID);
    CopyDeviceID (& pDataPacket->data.sourceDeviceID,      sourceDeviceID);
    *pHash = generateHash(pMessageBody, messageLength);
    pDataPacket->data.hash = *pHash;
    pDataPacket->data.messageTotalLength = messageLength;
    *pMessageBytesSent = (messageLength < MAX_MESSAGE_LENGTH) ? messageLength : MAX_MESSAGE_LENGTH;
    memcpy (pDataPacket->data.messageBody, pMessageBody, *pMessageBytesSent);
}

// clients, AP's and repeaters send additional fragments of messages using this packet
void sendMulti (packets * pMultiPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID sourceDeviceID, WORD messageLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody, BYTE * pMessageBytesSent)
{    
    pMultiPacket->standard.typeOfPacket = TYPE_MULTI;
    CopyDeviceID (& pMultiPacket->multi.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pMultiPacket->multi.destinationDeviceID, destDeviceID);
    CopyDeviceID (& pMultiPacket->multi.sourceDeviceID,      sourceDeviceID);
    pMultiPacket->multi.hash = hash;
    pMultiPacket->multi.sequenceNumber = sequenceNumber;
    *pMessageBytesSent = (messageLength < MAX_MESSAGE_LENGTH) ? messageLength : MAX_MESSAGE_LENGTH;
    pMultiPacket->multi.messageFragmentLength = *pMessageBytesSent;
    memcpy (pMultiPacket->multi.messageBody, pMessageBody, *pMessageBytesSent);
}

// clients, AP's and repeaters send this ack packet for initial and fragments of messages using this packet
void sendDataAck (packets * pAckPacket, DEVICE_ID apDeviceID, DEVICE_ID destDeviceID, DEVICE_ID myDeviceID, DWORD hash, WORD sequenceNumber)
{
    pAckPacket->standard.typeOfPacket = TYPE_ACK;
    CopyDeviceID (& pAckPacket->ack.accessPointDeviceID, apDeviceID);
    CopyDeviceID (& pAckPacket->ack.destinationDeviceID, destDeviceID);
    CopyDeviceID (& pAckPacket->ack.sourceDeviceID,      myDeviceID);
    pAckPacket->ack.hash = hash;
    pAckPacket->ack.sequenceNumber = sequenceNumber;
}

//----------------------------------------------------------------------------------
// reception of packets starts here and dispatches to specific packet handlers
// and then the protocol process logic and application interface
void recPacket (DEVICE_ID receivingDeviceID, packets * newPacket)
{
    DEVICE_ID apDeviceID;
    DEVICE_ID clDeviceID;
    DEVICE_ID destDeviceID;
    DEVICE_ID sourceDeviceID;
    BYTE      receiveSignalStrength;
    BYTE      deviceCount;
    DEVICE_ID pArrayOfDeviceIDs[DEVICE_LIST_MAX + 2];
    BYTE      internetConnected;
    DWORD     messageTotalLength;
    BYTE      messageFragmentLength;
    DWORD     hash;
    WORD      sequenceNumber;
    BYTE    * pMessageBody;
    
    
    switch (newPacket->standard.typeOfPacket)
    {
        case TYPE_HEARTBEAT:
            recHeartbeat (newPacket, & apDeviceID, & receiveSignalStrength, & deviceCount, pArrayOfDeviceIDs);
            processHeartbeat (receivingDeviceID, apDeviceID, receiveSignalStrength, deviceCount, pArrayOfDeviceIDs);
            break;
        case TYPE_HEARTBEAT_REPLY:
            recHeartbeatReply (newPacket, & apDeviceID, & clDeviceID, & receiveSignalStrength);
            processHeartbeatReply (receivingDeviceID, apDeviceID, clDeviceID, receiveSignalStrength);
            break;
        case TYPE_REGISTER:
            recRegistration (newPacket, & apDeviceID, & clDeviceID);
            processRegistration (receivingDeviceID, apDeviceID, clDeviceID);
            break;
        case TYPE_REGISTER_REPLY:
            recRegistrationReply (newPacket, & apDeviceID, & clDeviceID, & internetConnected);
            processRegistrationReply (receivingDeviceID, apDeviceID, clDeviceID, & internetConnected);
            break;
        case TYPE_DATA:
            recData (newPacket, & apDeviceID, & destDeviceID, & sourceDeviceID, & messageTotalLength, & messageFragmentLength, & hash, & pMessageBody);
            processData (receivingDeviceID, apDeviceID, destDeviceID, sourceDeviceID, messageTotalLength, messageFragmentLength, hash, pMessageBody);
            break;
        case TYPE_MULTI:
            recMultiData (newPacket, & apDeviceID, & destDeviceID, & sourceDeviceID, & messageFragmentLength, & hash, & sequenceNumber, & pMessageBody);
            processMulti (receivingDeviceID, apDeviceID, destDeviceID, sourceDeviceID, messageFragmentLength, hash, sequenceNumber, pMessageBody);
            break;
        case TYPE_ACK:
            recDataAck (newPacket, & apDeviceID, & destDeviceID, & sourceDeviceID, & hash, & sequenceNumber);
            processAck (receivingDeviceID, apDeviceID, destDeviceID, sourceDeviceID, hash, sequenceNumber);
            break;

        default:
            printf("ERROR: Received a packet with an unknown packet type\n");
            break;
    }
}

// client device receives this packet from AP's and repeaters
void recHeartbeat  (packets * pHeartbeatPacket, DEVICE_ID * apDeviceID, BYTE * pReceiveSignalStrength, BYTE * deviceCount, DEVICE_ID * pArrayOfDeviceIDs)
{
    *pReceiveSignalStrength = 0;    // get RSSI of the AP/repeater from radio here
    
    CopyDeviceID (apDeviceID, pHeartbeatPacket->heartbeat.accessPointDeviceID);
    *deviceCount = pHeartbeatPacket->heartbeat.deviceCount;
    *deviceCount = (*deviceCount < DEVICE_LIST_MAX) ? *deviceCount : DEVICE_LIST_MAX;
    CopyDeviceIDs (pArrayOfDeviceIDs, pHeartbeatPacket->heartbeat.registeredDevices, *deviceCount);
}

// AP's and repeaters receive this packet from clients, AP's and repeaters
void recHeartbeatReply (packets * pHeartbeatReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * pReceiveSignalStrength)
{
    *pReceiveSignalStrength = 0;    // get RSSI of the client, or other AP's and repeaters
    
    CopyDeviceID (apDeviceID, pHeartbeatReplyPacket->heartbeatReply.accessPointDeviceID);
    CopyDeviceID (clDeviceID, pHeartbeatReplyPacket->heartbeatReply.clientDeviceID);
}

// register a client with a repeater or AP
void recRegistration (packets * pRegistrationPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID)
{
    CopyDeviceID (apDeviceID, pRegistrationPacket->registration.accessPointDeviceID);
    CopyDeviceID (clDeviceID, pRegistrationPacket->registration.clientDeviceID);
}

// repeater or AP replies to a client registration request
void recRegistrationReply (packets * pRegistrationReplyPacket, DEVICE_ID * apDeviceID, DEVICE_ID * clDeviceID, BYTE * internetConnected)
{
    CopyDeviceID (apDeviceID, pRegistrationReplyPacket->registrationReply.accessPointDeviceID);
    CopyDeviceID (clDeviceID, pRegistrationReplyPacket->registrationReply.clientDeviceID);
    *internetConnected = pRegistrationReplyPacket->registrationReply.internetConnected;
}

// send the header and whole message body or first fragment of message body
void recData (packets * pDataPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * messageTotalLength, BYTE * messageFragmentLength, DWORD * hash, BYTE * * ppMessageBody)
{
    CopyDeviceID (apDeviceID,     pDataPacket->data.accessPointDeviceID);
    CopyDeviceID (destDeviceID,   pDataPacket->data.destinationDeviceID);
    CopyDeviceID (sourceDeviceID, pDataPacket->data.sourceDeviceID);
    *hash = pDataPacket->data.hash;
    *messageTotalLength = pDataPacket->data.messageTotalLength;
    *messageFragmentLength = ((*messageTotalLength) > MAX_MESSAGE_LENGTH) ? MAX_MESSAGE_LENGTH : *messageTotalLength;
    *ppMessageBody = & pDataPacket->data.messageBody[0];
}

// send additional message  fragments
void recMultiData (packets * pMultiPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, BYTE * messageFragmentLength, DWORD * hash, WORD * sequenceNumber, BYTE * * ppMessageBody)
{
    CopyDeviceID (apDeviceID,     pMultiPacket->multi.accessPointDeviceID);
    CopyDeviceID (destDeviceID,   pMultiPacket->multi.destinationDeviceID);
    CopyDeviceID (sourceDeviceID, pMultiPacket->multi.sourceDeviceID);
    *hash = pMultiPacket->multi.hash;
    *sequenceNumber = pMultiPacket->multi.sequenceNumber;
    *messageFragmentLength = pMultiPacket->multi.messageFragmentLength;
    *messageFragmentLength = ((*messageFragmentLength) > MAX_MESSAGE_LENGTH) ? MAX_MESSAGE_LENGTH : *messageFragmentLength;
    *ppMessageBody = & pMultiPacket->multi.messageBody[0];
}

// receiver of message replies to sender with ack for each data or multi packet received
void recDataAck (packets * pAckPacket, DEVICE_ID * apDeviceID, DEVICE_ID * destDeviceID, DEVICE_ID * sourceDeviceID, DWORD * hash, WORD * sequenceNumber)
{
    CopyDeviceID (apDeviceID,     pAckPacket->ack.accessPointDeviceID);
    CopyDeviceID (destDeviceID,   pAckPacket->ack.destinationDeviceID);
    CopyDeviceID (sourceDeviceID, pAckPacket->ack.sourceDeviceID);
    *hash = pAckPacket->ack.hash;
    *sequenceNumber = pAckPacket->ack.sequenceNumber;
}

//----------------------------------------------------------------------------------
DWORD generateHash(BYTE * pMessageBody, WORD messageLength)
{
    // set hash to the system time plus CRC32 of message content to ensure reasonable uniqueness
    return (GetMilliCount() + rc_crc32(0, (const char *)pMessageBody, messageLength) );
}

//----------------------------------------------------------------------------------
// CRC32 calculator
// from https://rosettacode.org/wiki/CRC-32#Implementation
uint32_t rc_crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;
 
	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}
 
	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}
 

//----------------------------------------------------------------------------------
// From: http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm

uint32_t GetMilliCount (void)
{
  // Something like GetTickCount but portable
  // It rolls over every ~ 12.1 days (0x100000/24/60/60)
  // Use GetMilliSpan to correct for rollover
  timeb tb;
  ftime( &tb );
  uint32_t nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
  return nCount;
}

uint32_t GetMilliSpan (uint32_t nTimeStart)
{
  int32_t nSpan = GetMilliCount() - nTimeStart;
  if ( nSpan < 0 )
    nSpan += 0x100000 * 1000;
  return nSpan;
}

