//
//  device.cpp
//  protocol
//
//  Created by Shannon T Bailey on 4/24/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#include "common.hpp"

extern DEVICE_ID nullDeviceID;

extern sTest * pTestData;

sDeviceInfo * pDevices[10];
WORD          deviceCount;

#ifdef DEVICE_DEBUG
extern sManager * pSendManagerList;
extern sManager * pReceiveManagerList;
#endif

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
                sendHeartbeat(& heartbeatPacket, pDeviceInfo->myDeviceID, 0, nullptr);
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

