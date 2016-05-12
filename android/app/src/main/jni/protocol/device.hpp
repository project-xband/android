//
//  device.hpp
//  protocol
//
//  Created by Shannon T Bailey on 4/24/16.
//  Copyright © 2016 Shannon T Bailey. All rights reserved.
//

#ifndef device_hpp
#define device_hpp

enum deviceRoles
{
    ROLE_NULL         = 0,
    ROLE_CLIENT       = 1,
    ROLE_REPEATER     = 2,
    ROLE_ACCESS_POINT = 3
};

enum deviceState
{
    DEVICE_NULL            = 0,
    DEVICE_INITIALIZED     = 1,
    DEVICE_SEARCHING       = 2,
    DEVICE_REGISTERING     = 3,
    DEVICE_UNREGISTERING   = 4,
    DEVICE_CONNECTED       = 5,
    DEVICE_LOST_CONNECTION = 6
};


typedef struct sRegionDeviceInfo_t
{
    struct sRegionDeviceInfo_t * pNextRegionDeviceInfo;
    
    DEVICE_ID deviceID;
    BYTE deviceRole;
    BYTE receiveSignalStrength;
    
} sRegionDeviceInfo;

typedef struct sDeviceInfo_t
{
    sRegionDeviceInfo * pRegionDeviceInfo;
    
    DEVICE_ID myDeviceID;
    DEVICE_ID accessPointDeviceID;
    
    DWORD     heartbeatLastTime;
    
    BYTE deviceTypeID;
    BYTE DeviceVersion;
    BYTE deviceRole;
    BYTE deviceState;
} sDeviceInfo;


#endif /* device_hpp */
