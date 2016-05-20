//
//  interface.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

extern sTest * pTestData;

//sDeviceInfo * pDevices[10];
//WORD          deviceCount;

sManager * pSendManagerList;
sManager * pReceiveManagerList;


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
    sManager * pPrev = nullptr;
    
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

