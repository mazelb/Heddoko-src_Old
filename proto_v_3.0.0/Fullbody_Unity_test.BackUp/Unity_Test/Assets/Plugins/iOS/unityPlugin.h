//
//  unityPlugin.h
//  iOSUnity
//
//  Created by Khwaab Dave on 12/9/14.
//  Copyright (c) 2014 Khwaab Dave. All rights reserved.
//
#include "OpenSpatialBluetooth.h"
#include "pluginHelper.h"

extern "C"
{
    typedef char nodBool;
    
    OpenSpatialBluetooth* openSpatial;
    pluginHelper* delegate;

    struct NodUniqueID
    {
        char byte0;
        char byte1;
        char byte2;
    };

    nodBool NodInitialize(void);
    nodBool NodShutdown(void);
    
    int  NodNumRings(void);
    const char* NodGetRingName(int ringID);
    
    nodBool NodSubscribeToButton(int ringID);
    nodBool NodSubscribeToPose6D(int ringID);
    nodBool NodSubscribeToGesture(int ringID);
    nodBool NodSubscribeToPosition2D(int ringID);
    nodBool NodSubscribeToGameControl(int ringID);

    nodBool NodUnsubscribeToButton(int ringID);
    nodBool NodUnsubscribeToPose6D(int ringID);
    nodBool NodUnsubscribeToGesture(int ringID);
    nodBool NodUnsubscribeToPosition2D(int ringID);
    nodBool NodUnSubscribeToGameControl(int ringID);
    
    int NodGetButtonState(int ringID);
    NodEulerOrientation NodGetEulerOrientation(int ringID);
    NodQuaternionOrientation NodGetQuaternionOrientation(int ringID);
    int NodGetGesture(int ringID);
    NodPointer NodGetPosition2D(int ringID);
    NodPointer NodGetGamePosition(int ringID);
    int NodGetTrigger(int ringID);

    int NodRequestBatteryPercentage(int ringID);
    int NodGetBatteryPercentage(int ringID);
}