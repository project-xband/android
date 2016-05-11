//
//  test.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/17/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//
//  The protocol utilizes client, repeater and access point devices to effect a mesh with acess points
//  providing internet connectivity, access point bridging and access to the device ID to user contact list, etc.

#ifndef test_hpp
#define test_hpp


#define MAX_DEVICES 24
#define MAX_PACKETS_IN_QUEUE 100


//----------------------------------------------------------------------------------
typedef struct sTest_t
{
    DWORD         deviceStartingID;
    DWORD         deviceCount;
    DWORD         testTime;
    sDeviceInfo * deviceInfo [MAX_DEVICES];
    DWORD         transmittedPacketQueueDepth;
    packets       transmittedPackets [MAX_PACKETS_IN_QUEUE];
} sTest ;

//----------------------------------------------------------------------------------
void test (void);

void testInit (void);
void testRun (void);

void testStepSimulation (void);

#endif /* test_hpp */
