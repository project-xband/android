
//
//  test.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

extern sManager * pSendManagerList;
extern sManager * pReceiveManagerList;

sTest * pTestData;

void testEncoders (void);

//----------------------------------------------------------------------------------
void test (void)
{
    testInit();
    
    testEncoders ();
    
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
    WORD   consumedBytes;
    
    extractPacket (& pPacketData, & packetLength, pFramedPacketData, framedPacketLength, & consumedBytes);

    *(pPacketData + packetLength) = 0;     // hack in null on binary byte datat that could have zero byte in it...  arg

    printf ("Packet Data =[%s]\n", pPacketData);
    
    
    
    WORD length;
    WORD consumed;
    BYTE pMessagePlus[] = "{ab%%cd%{efg%}hi!}{foooooffff";
    length = strlen ((const char *)pMessagePlus);
    extractPacket (& pPacketData, & packetLength, pMessagePlus, length, & consumed);

    *(pPacketData + packetLength) = 0;     // hack in null on binary byte datat that could have zero byte in it...  arg

    printf ("Packet Data =[%s]  consumed bytes = %d\n", pPacketData, consumed);
    
    ;
}


void testInit (void)
{
    protocolInitialize ();
    __android_log_print(ANDROID_LOG_DEBUG, "TESTINIT", "The value of 1 + 1 is blank blank blank");
    sManagerInit ();
    
    pTestData = (sTest *) malloc (sizeof(sTest));
    
    pTestData->deviceStartingID = 5;
    pTestData->deviceCount      = 0;
    pTestData->testTime         = GetMilliCount();
}


//----------------------------------------------------------------------------------
void testRun (void)
{
    DEVICE_ID apDeviceID;
    DEVICE_ID cl1DeviceID;
    DEVICE_ID cl2DeviceID;
    
    CreateDeviceID (& apDeviceID, pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(nullDeviceID, apDeviceID, ROLE_ACCESS_POINT);
    pTestData->deviceCount++;

    CreateDeviceID (& cl1DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo(apDeviceID, cl1DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;
    
    CreateDeviceID (& cl2DeviceID, 100 + pTestData->deviceStartingID++);
    pTestData->deviceInfo[pTestData->deviceCount] = initializeDeviceInfo (apDeviceID, cl2DeviceID, ROLE_CLIENT);
    pTestData->deviceCount++;
        
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
            
            
            
            if ((userTestTime + 2) < GetMilliCount())
            {
                BYTE pMessage[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-_-zyxwvutsrqponmlkjihgfedcba9876543210ZYXWVUTSRQPONMLKJIHGFEDCBA!";
                BYTE pMessage2[] = "Now is the time for all good Make School students to program on the xBAND project until the entire world uses XBAND for all their needs";
                BYTE pMessage3[] = "Now to sent from the access point to a client";
                
                printf ("Test time => %d\n", userTestTime);
                
                sendMessage (pTestData->deviceInfo[1], apDeviceID, pMessage);

                sendMessage (pTestData->deviceInfo[2], apDeviceID, pMessage2);
                
                sendMessage (pTestData->deviceInfo[0], cl1DeviceID, pMessage3);

                userTestTime = GetMilliCount();
            }
            
            testStepSimulation ();
            
            
            sMessage * pReceivedMessage;
            getMessage (pTestData->deviceInfo[0], & pReceivedMessage);
            if (NULL != pReceivedMessage)
            {
                printf ("Got device 0 message: %s\n", pReceivedMessage->pMessageBody);
                free(pReceivedMessage->pMessageBody);
                free(pReceivedMessage);
            }

            getMessage (pTestData->deviceInfo[0], & pReceivedMessage);
            if (NULL != pReceivedMessage)
            {
                printf ("Got device 1 message: %s\n", pReceivedMessage->pMessageBody);
                free(pReceivedMessage->pMessageBody);
                free(pReceivedMessage);
            }

            getMessage (pTestData->deviceInfo[1], & pReceivedMessage);
            if (NULL != pReceivedMessage)
            {
                printf ("Got device 2 message: %s\n", pReceivedMessage->pMessageBody);
                free(pReceivedMessage->pMessageBody);
                free(pReceivedMessage);
            }

            
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

