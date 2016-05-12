//
//  interface.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/18/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef interface_hpp
#define interface_hpp

#define HEARTBEAT_TIME 15000

typedef struct sMessage_t
{
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    WORD      messageTotalLength;
    BYTE    * pMessageBody;
} sMessage;


enum sendSessionStates
{
    SEND_SESSION_NULL                     = 0,
    SEND_SESSION_WAITING_FOR_HEADER_ACK   = 1,
    SEND_SESSION_WAITING_FOR_MULTI_ACK    = 2,
    SEND_SESSION_WAITING_FOR_COMPLETE_ACK = 3
};

enum receiveSessionStates
{
    RECEIVE_SESSION_NULL            = 0,
    RECEIVE_SESSION_GET_MULTI       = 1,
    RECEIVE_SESSION_GOT_IT_ALL      = 2,
    RECEIVE_SESSION_CALLED_APP_CODE = 3,
    RECEIVE_SESSION_WAITING_TO_END  = 4
};

typedef struct sSendManager_t
{
    BYTE      sessionState;
    DEVICE_ID apDeviceID;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      messageTotalLength;
    WORD      messageFragmentLength;
    WORD      sequenceNumber;
    BYTE    * pMessageBody;
}sSendManager;


typedef struct sReceiveManager_t
{
    BYTE      sessionState;
    DEVICE_ID apDeviceID;
    DEVICE_ID destinationDeviceID;
    DEVICE_ID sourceDeviceID;
    DWORD     hash;
    WORD      messageTotalLength;
    WORD      messageFragmentLength;
    WORD      sequenceNumber;
    BYTE    * pMessageBody;
}sReceiveManager;

typedef union uMessageManager_t
{
    sSendManager    pSendManagerList;
    sReceiveManager pReceiveManagerList;
}uMessageManager;

typedef struct sManager_t
{
    struct sManager_t * pNextManager;
    uMessageManager * puManager;
}sManager;


void listAppendNode (sManager * * ppManagerList, sManager * pNewManager);
int  listDeleteNode (sManager * * ppManagerList, sManager * pManagerToDelete);
void listWalkSendManager (sManager * pManagerList, sSendManager * * ppSendManager, DWORD hash, DEVICE_ID sourceID);
void listWalkReceiveManager (sManager * pManagerList, sReceiveManager * * ppReceiveManager, DWORD hash, DEVICE_ID sourceID);
int  listLength (sManager * pManagerList);


#endif /* interface_hpp */


// MAX_MESSAGE_LENGTH