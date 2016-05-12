//-----------------------------------//

//      Main RadioModule.cpp

//-----------------------------------//
#include <jni.h>

#define DEVICE_DEBUG


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

extern "C" {
JNIEXPORT jstring JNICALL
Java_com_xband_MainActivity_testInitSetup(JNIEnv *env, jobject thiz)
{
    testInit();
    return (env)->NewStringUTF("Where is this message !..... i found uuuuu");
}

JNIEXPORT jbyteArray JNICALL
Java_com_xband_MainActivity_testByteArray(JNIEnv *env, jobject jobj)
{
    jbyteArray framedData;
    int i;

    const char *message[5] = {"first",
                              "second",
                              "third",
                              "fourth",
                              "fifth"};

    framedData = (jbyteArray) env->NewByteArray(5);

    for (i = 0; i < 5; i++)
    {
        framedData, i, env->NewStringUTF(message[i]);
    }
    return (framedData);

}

JNIEXPORT jstring JNICALL
Java_com_xband_MainActivity_testSendMessage(JNIEnv * env, jobject thiz)
{
    const char * messageData = "lol what messageData";
    testSend(messageData);
    return (env)->NewStringUTF(" message data from testsend");
}


}




//-------protocol.cpp-------//
DEVICE_ID nullDeviceID;


//-------interface.cpp-------//
sManager * pSendManagerList;
sManager * pReceiveManagerList;


//-------device.cpp-------//
sDeviceInfo * pDevices[10];
WORD          deviceCount;


//-------test.cpp-------//
sTest * pTestData;




//-----------------------------------//

//          protocol.cpp

//-----------------------------------//


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






//-----------------------------------//

//          interface.cpp

//-----------------------------------//




//
//  interface.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//



//----------------------------------------------------------------------------------

void getDeviceInfo (sDeviceInfo * pDeviceInfo)
{
    ;
}
void getRegionDeviceInfo (sDeviceInfo * pDeviceInfo, sRegionDeviceInfo * pRegionDeviceInfo)
{
    ;
}
void registerWithAp (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID)  // void (* pRegisterCallBackFunction)() )
{
    ;
}
void unregisterWithAp (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID)  // void (* pUnregisterCallBackFunction)() )
{
    ;
}
void registerRequest (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID)  // void (* pRegisterRequestCallBackFunction)() )
{
    ;
}
void unregisterRequest (sDeviceInfo * pDeviceInfo, DEVICE_ID apDeviceID)  // void (* pUnregisterRequestCallBackFunction)() )
{
    ;
}
void sendMessage (sDeviceInfo * pDeviceInfo, DEVICE_ID destinationDeviceID, BYTE * pMessageBody)  // void (* pSendMessageCallBackFunction)() )
{
    sendHeaderAndData (pDeviceInfo, destinationDeviceID, pMessageBody);
}
void sendMessageCallBack (sDeviceInfo * pDeviceInfo, sMessage * pMessage)
{
    ;
}
void getMessage (sDeviceInfo * pDeviceInfo, sMessage * * ppMessage)
{
    WORD index;
    WORD length;
    sManager * pManager;

    *ppMessage = NULL;

    if (0 == (length = listLength (pReceiveManagerList) ) )
        return;

    pManager = pReceiveManagerList;

    for (index = 0; index < length; index++)
    {
        if (NULL != pManager->puManager)
        {
            if (RECEIVE_SESSION_GOT_IT_ALL == pManager->puManager->pReceiveManagerList.sessionState)
            {
                *ppMessage = (sMessage *) malloc (sizeof(sMessage));
                // NTR check for NULL
                CopyDeviceID (& (*ppMessage)->destinationDeviceID, pManager->puManager->pReceiveManagerList.destinationDeviceID);
                CopyDeviceID (& (*ppMessage)->sourceDeviceID,      pManager->puManager->pReceiveManagerList.sourceDeviceID);
                (*ppMessage)->messageTotalLength                 = pManager->puManager->pReceiveManagerList.messageTotalLength;

                // Note: return pMessageBody memory block to be freed by the caller after use
                (*ppMessage)->pMessageBody                       = pManager->puManager->pReceiveManagerList.pMessageBody;

                // remove this session from the ReceiveManagerList.
                listDeleteNode (& pReceiveManagerList, pManager);

                return;
            }
        }
        pManager = pManager->pNextManager;
    }
}


//----------------------------------------------------------------------------------
// process all types of received packet types
void processHeartbeat (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, BYTE receiveSignalStrength, BYTE deviceCount, DEVICE_ID * pArrayOfDeviceIDs)
{
    packets heartBeatReplyPacket;

    if ( ! CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
        sendHeartbeatReply (& heartBeatReplyPacket, apDeviceID, receivingDeviceID, rand() % 10);
        transmitPacket(& heartBeatReplyPacket);
    }
}

void processHeartbeatReply (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE receiveSignalStrength)
{
    if ( CompareDeviceID (receivingDeviceID, apDeviceID) )
    {
        // maintain the APs client ID list of info and broadcast it in heartbeat packets
        ;
    }
}

void processRegistration (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID)
{
    ;
}

void processRegistrationReply (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID clDeviceID, BYTE * pInternetConnected)
{
    ;
}

void processData (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID,
                  DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody)
{
    if ( (CompareDeviceID (receivingDeviceID, apDeviceID)) || (CompareDeviceID(receivingDeviceID, destinationDeviceID)) )
    {
        receiveHeaderAndData(apDeviceID, destinationDeviceID, sourceDeviceID, messageTotalLength, messageFragmentLength, hash, pMessageBody);
    }
}


void processMulti (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID,
                   BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody)
{
    if ( (CompareDeviceID (receivingDeviceID, apDeviceID)) || (CompareDeviceID(receivingDeviceID, destinationDeviceID)) )
    {
        receiveSessionData (apDeviceID, destinationDeviceID, sourceDeviceID, messageFragmentLength, hash, sequenceNumber, pMessageBody);
    }
}

void processAck (DEVICE_ID receivingDeviceID, DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID,
                 DWORD hash, WORD sequenceNumber)
{
    sSendManager * pSendManager;

    if ( (CompareDeviceID (receivingDeviceID, apDeviceID)) || (CompareDeviceID(receivingDeviceID, sourceDeviceID)) )
    {
        listWalkSendManager (pSendManagerList, & pSendManager, hash, sourceDeviceID);
        if (pSendManager != NULL)
        {
            sendSessionData (pSendManager);
        }
    }
}


//----------------------------------------------------------------------------------
// setup and process send and receive message sessions

void sManagerInit (void)
{
    pSendManagerList    = NULL;
    pReceiveManagerList = NULL;
}


// setup a send message session
void sendHeaderAndData (sDeviceInfo * pDeviceInfo, DEVICE_ID destinationDeviceID, BYTE * pMessageBody)
{
    sManager     * pManager;
    sSendManager * pSendManager;
    packets        dataPacket;
    DWORD          hashValue;
    BYTE           bytesSent;
    DWORD          messageLength;

    messageLength = (DWORD) strlen((const char *)pMessageBody);

    sendData(& dataPacket, pDeviceInfo->accessPointDeviceID, destinationDeviceID, pDeviceInfo->myDeviceID, messageLength, & hashValue, pMessageBody, & bytesSent);

    // NTR:  check for allocator failure
    pManager     = (sManager *) malloc (sizeof(sManager));
    pSendManager = (sSendManager *) malloc (sizeof(sSendManager));

    pManager->puManager = (uMessageManager *) pSendManager;

    listAppendNode (& pSendManagerList, pManager);

    CopyDeviceID (& pSendManager->apDeviceID,          pDeviceInfo->accessPointDeviceID);
    CopyDeviceID (& pSendManager->destinationDeviceID, destinationDeviceID);
    CopyDeviceID (& pSendManager->sourceDeviceID,      pDeviceInfo->myDeviceID);
    pSendManager->hash                  = hashValue;
    pSendManager->messageTotalLength    = messageLength;
    pSendManager->messageFragmentLength = bytesSent;
    pSendManager->sequenceNumber        = 0;
    pSendManager->pMessageBody          = pMessageBody;

    if (bytesSent < messageLength)
    {
        pSendManager->sessionState = SEND_SESSION_WAITING_FOR_HEADER_ACK;
    } else {
        pSendManager->sessionState = SEND_SESSION_WAITING_FOR_COMPLETE_ACK;
    }

    transmitPacket(& dataPacket);
}


// process each portion of the send session state and transmit message fragments, receive acks and handle errors
void sendSessionData (sSendManager * pSendManager)
{
    packets        dataPacket;
    DEVICE_ID      apDeviceID;
    DEVICE_ID      destinationDeviceID;
    DEVICE_ID      sourceDeviceID;
    DWORD          hashValue;
    WORD           sequenceNumber;
    DWORD          messageLength;
    BYTE           messageFragmentLength;
    DWORD          messageSendOffset;
    BYTE         * pMessageBody;

    switch (pSendManager->sessionState) {
        case SEND_SESSION_WAITING_FOR_HEADER_ACK:
        MULTI:
            CopyDeviceID (& apDeviceID, pSendManager->apDeviceID);
            CopyDeviceID (& destinationDeviceID, pSendManager->destinationDeviceID);
            CopyDeviceID (& sourceDeviceID, pSendManager->sourceDeviceID);
            hashValue             = pSendManager->hash;
            messageLength         = pSendManager->messageTotalLength;
            messageFragmentLength = pSendManager->messageFragmentLength;

            sequenceNumber      = ++pSendManager->sequenceNumber;

            messageSendOffset     = sequenceNumber * MAX_MESSAGE_LENGTH;
            messageLength        -= messageSendOffset;
            pMessageBody          = pSendManager->pMessageBody + messageSendOffset;

            sendMulti (& dataPacket, apDeviceID, destinationDeviceID, sourceDeviceID, messageLength, hashValue, sequenceNumber, pMessageBody, & messageFragmentLength);

            if (messageFragmentLength < messageLength)
            {
                pSendManager->sessionState = SEND_SESSION_WAITING_FOR_MULTI_ACK;
            } else {
                pSendManager->sessionState = SEND_SESSION_WAITING_FOR_COMPLETE_ACK;
            }

            transmitPacket(& dataPacket);
            break;

        case SEND_SESSION_WAITING_FOR_MULTI_ACK:
            goto MULTI;
            break;

        case SEND_SESSION_WAITING_FOR_COMPLETE_ACK:
            pSendManager->sessionState = SEND_SESSION_NULL;

            WORD index;
            WORD length;
            sManager * pManager;

            if (0 == (length = listLength (pSendManagerList) ) )
                return;

            pManager = pSendManagerList;

            for (index = 0; index < length; index++)
            {
                if (pSendManager == & (pManager->puManager->pSendManagerList) )
                {
                    listDeleteNode (& pSendManagerList, pManager);
                    break;
                }
                pManager = pManager->pNextManager;
            }
            break;

        default:
            break;
    }
}


// setup a receive message session
void receiveHeaderAndData (DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, DWORD messageTotalLength, BYTE messageFragmentLength, DWORD hash, BYTE * pMessageBody)
{
    packets           ackPacket;
    sManager        * pManager;
    sReceiveManager * pReceiveManager;

    // NTR:  check for allocator failure
    pManager        = (sManager *) malloc (sizeof(sManager));
    pReceiveManager = (sReceiveManager *) malloc (sizeof(sReceiveManager));

    pManager->puManager = (uMessageManager *) pReceiveManager;

    listAppendNode (& pReceiveManagerList, pManager);

    CopyDeviceID (& pReceiveManager->apDeviceID,          apDeviceID);
    CopyDeviceID (& pReceiveManager->destinationDeviceID, destinationDeviceID);
    CopyDeviceID (& pReceiveManager->sourceDeviceID,      sourceDeviceID);
    pReceiveManager->hash                  = hash;
    pReceiveManager->messageTotalLength    = messageTotalLength;
    pReceiveManager->messageFragmentLength = messageFragmentLength;
    pReceiveManager->sequenceNumber        = 0;

    pReceiveManager->pMessageBody          = (BYTE *) malloc (messageTotalLength + 2);

    if (messageFragmentLength == messageTotalLength)
    {
        pReceiveManager->sessionState = RECEIVE_SESSION_GOT_IT_ALL;
    } else {
        pReceiveManager->sessionState = RECEIVE_SESSION_GET_MULTI;
    }

    memcpy (pReceiveManager->pMessageBody, pMessageBody, messageFragmentLength);
    pReceiveManager->pMessageBody[messageFragmentLength] = 0;

#ifdef DEVICE_DEBUG
    printf ("Message Head => %s\n", pReceiveManager->pMessageBody);
#endif
    sendDataAck (& ackPacket, apDeviceID, destinationDeviceID, sourceDeviceID, hash, 0);

    transmitPacket(& ackPacket);
}


// process each portion of the receive session state and receive message fragments, send acks and handle timeout errors waiting for new message fragments
void receiveSessionData (DEVICE_ID apDeviceID, DEVICE_ID destinationDeviceID, DEVICE_ID sourceDeviceID, BYTE messageFragmentLength, DWORD hash, WORD sequenceNumber, BYTE * pMessageBody)
{
    sReceiveManager * pReceiveManager;
    packets ackPacket;
    DWORD   messageTotalLength;
    DWORD   messageSendOffset;

    listWalkReceiveManager (pReceiveManagerList, & pReceiveManager, hash, sourceDeviceID);
    if (pReceiveManager == NULL)
    {
        printf ("ERROR: received a packet that did not match to a session, dropping it\n");
    }

    messageTotalLength = pReceiveManager->messageTotalLength;
    messageSendOffset  = sequenceNumber * MAX_MESSAGE_LENGTH;

    if (messageTotalLength == (messageSendOffset + messageFragmentLength) )
    {
        pReceiveManager->sessionState = RECEIVE_SESSION_GOT_IT_ALL;
    }

    memcpy (pReceiveManager->pMessageBody + messageSendOffset, pMessageBody, messageFragmentLength);
    pReceiveManager->pMessageBody[messageSendOffset + messageFragmentLength] = 0;

#ifdef DEVICE_DEBUG
    printf ("Message Multi => %s\n", pReceiveManager->pMessageBody);
#endif
    sendDataAck (& ackPacket, apDeviceID, destinationDeviceID, sourceDeviceID, hash, sequenceNumber);

    transmitPacket(& ackPacket);
}


//----------------------------------------------------------------------------------
// the basis is from: http://www.cprogramming.com/snippets/source-code/singly-linked-list-insert-remove-add-count

void listAppendNode (sManager * * ppManagerList, sManager * pNewManager)
{
    sManager * pRight;

    pRight = * ppManagerList;

    if (pRight == NULL)
    {
        *ppManagerList = pNewManager;
        pNewManager->pNextManager = NULL;
        return;
    }

    while ( pRight->pNextManager != NULL )
        pRight = pRight->pNextManager;

    pRight->pNextManager = pNewManager;
    pRight = pNewManager;
    pRight->pNextManager = NULL;
}

int listDeleteNode (sManager * * ppManagerList, sManager * pManagerToDelete)
{
    sManager * pTemp;
    sManager * pPrev = NULL;

    pTemp = * ppManagerList;

    while ( pTemp != NULL)
    {
        if ( pTemp == pManagerToDelete)
        {
            if ( pTemp == * ppManagerList)
            {
                * ppManagerList = pTemp->pNextManager;  // if pTemp is the only node, the list head is set to NULL here
                if (NULL != pTemp->puManager )
                    free ( pTemp->puManager );
                free ( pTemp );
                return 1;
            }
            else
            {
                pPrev->pNextManager = pTemp->pNextManager;  // ignore warning about pPrev maybe not be set before use
                if (NULL != pTemp->puManager )
                    free ( pTemp->puManager );
                free( pTemp );
                return 1;
            }
        }
        else
        {
            pPrev = pTemp;
            pTemp = pTemp->pNextManager;
        }
    }
    return 0;
}


void listWalkSendManager (sManager * pManagerList, sSendManager * * ppSendManager, DWORD hash, DEVICE_ID sourceID)
{
    sManager * pTemp;

    * ppSendManager = NULL;

    pTemp = pManagerList;
    if (pTemp == NULL)
    {
        return;
    }
    while (pTemp != NULL)
    {
        if (hash == pTemp->puManager->pSendManagerList.hash)
        {
            if (CompareDeviceID(sourceID, pTemp->puManager->pSendManagerList.sourceDeviceID))
            {
                * ppSendManager = & pTemp->puManager->pSendManagerList;
                return;
            }
        }
        pTemp = pTemp->pNextManager;
    }
}


void listWalkReceiveManager (sManager * pManagerList, sReceiveManager * * ppReceiveManager, DWORD hash, DEVICE_ID sourceID)
{
    sManager * pTemp;

    * ppReceiveManager = NULL;

    pTemp = pManagerList;
    if (pTemp == NULL)
    {
        return;
    }
    while (pTemp != NULL)
    {
        if (hash == pTemp->puManager->pReceiveManagerList.hash)
        {
            if (CompareDeviceID(sourceID, pTemp->puManager->pReceiveManagerList.sourceDeviceID))
            {
                * ppReceiveManager = & pTemp->puManager->pReceiveManagerList;
                return;
            }
        }
        pTemp = pTemp->pNextManager;
    }
}


int listLength (sManager * pManagerList)
{
    sManager * pTemp;
    int count = 0;

    pTemp = pManagerList;
    while (pTemp != NULL)
    {
        pTemp = pTemp->pNextManager;
        count++;
    }
    return count;
}






//-----------------------------------//

//            device.cpp

//-----------------------------------//




//
//  device.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/24/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//



//----------------------------------------------------------------------------------
// Physical and simulated device initialization, state machine operation and packet communication
sDeviceInfo * initializeDeviceInfo (DEVICE_ID accessPointDeviceID, DEVICE_ID deviceID, BYTE initialDeviceRole)
{
    sDeviceInfo * pDeviceInfo;

    if (NULL == (pDeviceInfo = (sDeviceInfo *) malloc (sizeof(sDeviceInfo)) ) )
        return (NULL);

    pDeviceInfo->pRegionDeviceInfo = NULL;

    CopyDeviceID (& pDeviceInfo->myDeviceID, deviceID);
    CopyDeviceID (& pDeviceInfo->accessPointDeviceID, accessPointDeviceID);

    pDeviceInfo->heartbeatLastTime = GetMilliCount();

    pDeviceInfo->deviceTypeID  = 1;
    pDeviceInfo->DeviceVersion = 1;
    pDeviceInfo->deviceRole    = initialDeviceRole;
    pDeviceInfo->deviceState   = DEVICE_INITIALIZED;

    pDevices[deviceCount] = pDeviceInfo;
    deviceCount++;

    return (pDeviceInfo);
}


void runDeviceStateMachine (void)
{
    WORD index;
    sDeviceInfo * pDeviceInfo;

    packets heartbeatPacket;

    for (index = 0; index < deviceCount; index++)
    {
        pDeviceInfo = pDevices[index];

        if (pDeviceInfo->deviceRole == ROLE_ACCESS_POINT)
        {
            if ((pDeviceInfo->heartbeatLastTime + 15000) < GetMilliCount() )
            {
                sendHeartbeat(& heartbeatPacket, pDeviceInfo->myDeviceID, 0, NULL);
                transmitPacket (& heartbeatPacket);

                pDeviceInfo->heartbeatLastTime = GetMilliCount();
            }
        }



    }
}

// transmit a packet from a device to all others in range
void transmitPacket (packets * packet)
{
#ifdef DEVICE_DEBUG
    if ((MAX_PACKETS_IN_QUEUE - 1) > pTestData->transmittedPacketQueueDepth)
    {
        memcpy ((void *)(& pTestData->transmittedPackets[pTestData->transmittedPacketQueueDepth]), (const void *)packet, sizeof(packets) );
        pTestData->transmittedPacketQueueDepth++;
    } else {
        printf("ERROR: Dropping transmitted packet due to testing queue being full\n");
    }
#else
    ;
#endif


    //-----------------------------Java node edit start----------------------------------------

    BYTE * pFramedData;
    WORD framedLength;

    {
        framePacket (& pFramedData, & framedLength, (BYTE *) packet, sizeof(packets));

        //----------------------------- recent-edit --//

        char * literal = new char[framedLength*3];
        int literalLength = 0;
        for (int i = 0; i < framedLength; i++)
            literalLength += sprintf(literal + literalLength, "%d ", pFramedData[i]);

//        printf ("%s\n", literal);

        __android_log_print(ANDROID_LOG_DEBUG, "FRAMEDDATA_TAG", "\n hi i packet with info: %s \n", literal);

//
//        const char * newTestData = "st\0ring Literal";
//
//        //const char *newTestData = ( const char * )pFramedData;
//
//// them learnings below
//        const char * charBuffer = ( const char * ) pFramedData;
//
//        const char *buffer = "\0ABCD\0hello";
//        const char *s = buffer;
//        int n = framedLength;
//
//        while (n--)
//            printf("%c ", putchar(*charBuffer++));
//        printf ("\n");
//
//        for (int i = 0; i < framedLength; i++)
//            printf("%d ", charBuffer[i]);
//
//        printf ("\n");


    }

    //-----------------------------Java node edit end----------------------------------------
}

// receive a packet from a device
void receivePacket (void)
{
#ifdef DEVICE_DEBUG
    WORD index;

    // iterate through all devices, allowing them to decode and act upon the packet
    if (0 < pTestData->transmittedPacketQueueDepth)
    {
        for (index = 0; index < pTestData->deviceCount; index++)
        {
            recPacket (pTestData->deviceInfo[index]->myDeviceID, & pTestData->transmittedPackets[0]);
        }
        removeFirstPacketFromQueue ();
    }
#else
    ;
#endif
}

// remove a packet after all devices have had a chance to receive it
void removeFirstPacketFromQueue (void)
{
#ifdef DEVICE_DEBUG
    if (0 == pTestData->transmittedPacketQueueDepth)
    {
        printf ("ERROR: attempting to remove transmitted packet from empty buffer\n");
        return;
    }

    if (1 < pTestData->transmittedPacketQueueDepth)
    {
        memcpy (& pTestData->transmittedPackets[0], & pTestData->transmittedPackets[1], sizeof(packets) * pTestData->transmittedPacketQueueDepth - 1);
    }
    pTestData->transmittedPacketQueueDepth--;
#endif
}

//----------------------------------------------------------------------------------
// receive packet data and length and use '%' as escape character for '%' '{' and '}' while
// while using '{' as start of frame andn '}' as end of frame character values
// first scan packet data for characters that require escaping, add those to start and end frame character total
// and allocate a new buffer for the total.  Then copy the start frame character, followed by the
// bytes that don't require escaping, and special copy for escaped characters.  Lastly close with final end of frame character.

void framePacket (BYTE * * ppFramedData, WORD * pFramedLength, BYTE * pPacketData, BYTE packetLength)
{
    WORD index;
    BYTE * pFramedData;
    BYTE * pData;
    WORD calculatedLength;

    for (index = 0, calculatedLength = 0, pData = pPacketData; index < packetLength; index++, pData++)
    {
        switch (*pData)
        {
            case '%':
            case '{':
            case '}':
                calculatedLength += 2;
                break;
            default:
                calculatedLength++;
        }
    }

    calculatedLength += 2;  // add length of opening and closing curly braces

    pFramedData = (BYTE *) malloc (calculatedLength + 2);

    *ppFramedData = pFramedData ;

    *pFramedData++ = '{';

    for (index = 0, pData = pPacketData; index < packetLength; index++, pData++)
    {
        switch (*pData)
        {
            case '%':
                *pFramedData++ = '%';
                *pFramedData++ = '%';
                break;
            case '{':
                *pFramedData++ = '%';
                *pFramedData++ = '{';
                break;
            case '}':
                *pFramedData++ = '%';
                *pFramedData++ = '}';
                break;
            default:
                *pFramedData++ = *pData;
                break;
        }
    }

    *pFramedData++ = '}';
    *pFramedLength = calculatedLength;
}

void extractPacket (BYTE * * ppPacketData, BYTE * pPacketLength, BYTE * pFramedPacketData, WORD framePacketLength)
{
    WORD index;
    BYTE * pFramedData;
    BYTE * pEndOfFramedData;
    WORD calculatedLength = 0;
    BYTE * pUnframedData;

    *ppPacketData = NULL;

    pEndOfFramedData = pFramedPacketData + framePacketLength;

    if ('{' == *pFramedPacketData)
    {
        pFramedPacketData++;

        pFramedData = pFramedPacketData;

        for ( ; pFramedData < pEndOfFramedData; pFramedData++)
        {
            if ('%' == *pFramedData)
            {
                pFramedData++;
            }
            else
            {
                if ('}' == *pFramedData)
                    goto FoundClosingFrame;
            }
            calculatedLength++;
        }
        // if we exit the for loop without finding the closing brace (frame) this is an error
        return;


        FoundClosingFrame:

        // NTR check for alloc failure
        pUnframedData = (BYTE *) malloc (calculatedLength + 2);
        *ppPacketData = pUnframedData;
        *pPacketLength = calculatedLength;

        pFramedData   = pFramedPacketData;  // start after opening brace, advanced above

        for (index = 0; index < calculatedLength; index++)
        {
            if ('%' == *pFramedData)
            {
                pFramedData++;
            }
            *pUnframedData++ = *pFramedData++;
        }
    }

}


//----------------------------------------------------------------------------------
// DEVICE_ID helper functions
void CreateDeviceID (DEVICE_ID * newDeviceID, DWORD value)
{
    *(DWORD *) newDeviceID = value;
    *newDeviceID[4] = 0;    // just ignore the first 2 bytes
    *newDeviceID[5] = 0;
}

BYTE CompareDeviceID (DEVICE_ID aDeviceID, DEVICE_ID bDeviceID)
{
    if ((*(DWORD *) aDeviceID) == (*(DWORD *) bDeviceID) )
    {
        if ((*(WORD *) & (aDeviceID[4]) ) == (*(WORD *) & bDeviceID[4]) )
        {
            return true;
        }
    }
    return false;
}

void CopyDeviceID (DEVICE_ID * pDestination, DEVICE_ID source)
{
    memcpy (pDestination, source, DEVICE_ID_LENGTH);
}

void CopyDeviceIDs (DEVICE_ID * pDestination, DEVICE_ID * pSource, BYTE count)
{
    if (count > 0)
    {
        memcpy (pDestination, pSource, DEVICE_ID_LENGTH * count);
    }
}






//-----------------------------------//

//             test.cpp

//-----------------------------------//





//
//  test.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

//----------------------------------------------------------------------------------
void test (void)
{
    testInit();

//    testEncoders ();

    testRun();
}


void testEncoders (void)
{

    BYTE * pFramedPacketData;
    WORD   framedPacketLength;

    //BYTE pMessage[] = "abcdefghijklmnopqrst%%uvwxyz012345%{6789ABCDEFGHIJKLMNOPQRSTUVWXYZ-_-zyxwvutsrqponmlkjihgfedcba987654%}3210ZYXWVUTSRQPONMLKJIHGFEDCBA!";
    BYTE pMessage[] = "ab%%cd%{efg%}hi!";
    BYTE messageLength = strlen((const char *)pMessage);

    framePacket (& pFramedPacketData, & framedPacketLength, pMessage, messageLength);

    *(pFramedPacketData + framedPacketLength) = 0;    // hack in null on binary byte datat that could have zero byte in it...  arg

    printf ("Framed Data =[%s]\n", pFramedPacketData);

    BYTE * pPacketData;
    BYTE   packetLength;
    extractPacket (& pPacketData, & packetLength, pFramedPacketData, framedPacketLength);

    *(pPacketData + packetLength) = 0;     // hack in null on binary byte datat that could have zero byte in it...  arg

    printf ("Packet Data =[%s]\n", pPacketData);

}

//--------------------- java node edit start ---------
DEVICE_ID apDeviceID;
DEVICE_ID cl1DeviceID;
DEVICE_ID cl2DeviceID;
//--------------------- java node edit end ---------


void testInit (void)
{
    __android_log_print(ANDROID_LOG_DEBUG, "TESTINIT_TAG", "\n testinit ran success \n");
    protocolInitialize ();
    sManagerInit ();

    pTestData = (sTest *) malloc (sizeof(sTest));

    pTestData->deviceStartingID = 5;
    pTestData->deviceCount      = 0;
    pTestData->testTime         = GetMilliCount();

    //--------------------------------java node edit start-----------------------------------

    CreateDeviceID (& apDeviceID, pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(nullDeviceID, apDeviceID, ROLE_ACCESS_POINT);
    pTestData->deviceCount++;

    CreateDeviceID (& cl1DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(apDeviceID, cl1DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;

    CreateDeviceID (& cl2DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo (apDeviceID, cl2DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;

    //--------------------------------java node edit end-----------------------------------

}



//--------------------------------java node edit start-----------------------------------




void testSend(const char * pMessage) {
    __android_log_print(ANDROID_LOG_DEBUG, "TESTSEND_TAG", "\n testsend ran success \n");
    sendMessage (pTestData->deviceInfo[1], apDeviceID, (BYTE *) pMessage);
}


char const * testget(void)
{
    sMessage * pReceivedMessage;

    testStepSimulation ();
    testStepSimulation ();

    getMessage (pTestData->deviceInfo[0], & pReceivedMessage);
    if (NULL != pReceivedMessage)
    {
        printf ("Got device 0 message: %s\n", pReceivedMessage->pMessageBody);
        // free(pReceivedMessage->pMessageBody);
        free(pReceivedMessage);
        return ( ( char const * ) pReceivedMessage->pMessageBody);
    }
    return ( ( char const * ) NULL);
}



//--------------------------------java node edit end-----------------------------------



//----------------------------------------------------------------------------------
void testRun (void)
{

    //-------- java node edit start --------
//    DEVICE_ID apDeviceID;
//    DEVICE_ID cl1DeviceID;
//    DEVICE_ID cl2DeviceID;
//
//    CreateDeviceID (& apDeviceID, pTestData->deviceStartingID++);
//    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(nullDeviceID, apDeviceID, ROLE_ACCESS_POINT);
//    pTestData->deviceCount++;
//
//    CreateDeviceID (& cl1DeviceID, 100 + pTestData->deviceStartingID++);
//    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(apDeviceID, cl1DeviceID, ROLE_CLIENT);
//    pTestData->deviceCount++;
//
//    CreateDeviceID (& cl2DeviceID, 100 + pTestData->deviceStartingID++);
//    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo (apDeviceID, cl2DeviceID, ROLE_CLIENT);
//    pTestData->deviceCount++;

    //-------- java node edit start --------

    // run test loop

    DWORD userTestTime = GetMilliCount();

    while (true)
    {
        if ((pTestData->testTime + 100) < GetMilliCount() )
        {
            pTestData->testTime = GetMilliCount();

            // create additional devices and scheduled events and cause various device states to occur
            //-------------------------
            // ADD TEST CODE HERE ...


//------ java node edit start --------

            if ((userTestTime + 500) < GetMilliCount())
            {
//                BYTE pMessage[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-_-zyxwvutsrqponmlkjihgfedcba9876543210ZYXWVUTSRQPONMLKJIHGFEDCBA!";
//                BYTE pMessage2[] = "Now is the time for all good Make School students to program on the xBAND project until the entire world uses XBAND for all their needs";
//                BYTE pMessage3[] = "Now to sent from the access point to a client";

                printf ("Test time => %d\n", userTestTime);

//                sendMessage (pTestData->deviceInfo[1], apDeviceID, pMessage);
//
//                sendMessage (pTestData->deviceInfo[2], apDeviceID, pMessage2);
//
//                sendMessage (pTestData->deviceInfo[0], cl1DeviceID, pMessage3);

                userTestTime = GetMilliCount();
            }

//------ java node edit start --------

            testStepSimulation ();


            sMessage * pReceivedMessage;
            getMessage (pTestData->deviceInfo[0], & pReceivedMessage);
            if (NULL != pReceivedMessage)
            {
                printf ("Got device 0 message: %s\n", pReceivedMessage->pMessageBody);
                free(pReceivedMessage->pMessageBody);
                free(pReceivedMessage);
            }

            //------ java node edit start ------------

//            getMessage (pTestData->deviceInfo[0], & pReceivedMessage);
//            if (NULL != pReceivedMessage)
//            {
//                printf ("Got device 1 message: %s\n", pReceivedMessage->pMessageBody);
//                free(pReceivedMessage->pMessageBody);
//                free(pReceivedMessage);
//            }
//
//            getMessage (pTestData->deviceInfo[1], & pReceivedMessage);
//            if (NULL != pReceivedMessage)
//            {
//                printf ("Got device 2 message: %s\n", pReceivedMessage->pMessageBody);
//                free(pReceivedMessage->pMessageBody);
//                free(pReceivedMessage);
//            }

            //------ java node edit end ------------

            // END OF TEST CODE SECTION
            //-------------------------

            testStepSimulation ();
        }
    }
}


//----------------------------------------------------------------------------------
void testStepSimulation (void)
{
    // run receive until no sessions or transmitted packets are still in the transmit queue

    //    receivePacket ();
    runDeviceStateMachine ();

    // NTR when session managers are deleted, enable checking
    while ((0 < pTestData->transmittedPacketQueueDepth) )
        //        || (NULL != pSendManagerList) || (NULL != pReceiveManagerList) )
    {
        receivePacket ();
        runDeviceStateMachine ();
    }
}












//--------------//






  //--end-module--//




//-------------------//





//JNIEXPORT jstring JNICALL
//Java_com_xband_MainActivity_testSendMessage(JNIEnv *env, jobject instance) {
//
//    // TODO
//
//
//    return env->NewStringUTF(returnValue);
//}