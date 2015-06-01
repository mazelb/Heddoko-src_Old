//
//  unityPlugin.h
//  iOSUnity
//
//  Created by Khwaab Dave on 12/9/14.
//  Copyright (c) 2014 Khwaab Dave. All rights reserved.
//
#include "OpenSpatialBluetooth.h"
#include "BluetoothConnection.h"
#include "pluginHelper.h"

extern "C"
{
    typedef char nodBool;
    
    OpenSpatialBluetooth* openSpatial;
    BluetoothConnection* ringControl;
    pluginHelper* delegate;
    
    nodBool NodInitialize(void);
    nodBool NodShutdown(void);
    
    int  NodNumRings(void);
    const char* NodGetRingName(int ringID);
    nodBool NodSetMode(int ringID, int mode);
    
    nodBool NodSubscribeToButton(int ringID);
    nodBool NodSubscribeToPose6D(int ringID);
    nodBool NodSubscribeToGesture(int ringID);
    nodBool NodSubscribeToPosition2D(int ringID);
    
    int NodGetButtonState(int ringID);
    NodEulerOrientation NodGetEulerOrientation(int ringID);
    NodQuaternionOrientation NodGetQuaternionOrientation(int ringID);
    int NodGetGesture(int ringID);
    NodPointer NodGetPosition2D(int ringID);
    
     float NodBatteryPercent(int ringID);
    
     int NodReadMode(int ringID);
     nodBool NodShutdownRing(int ringID);
    
    const  char* NodCurrentFirmwareVersion(int ringID);
    const  char* NodDeviceAddress(int ringID);
    
     nodBool NodRecalibrate(int ringID);
     nodBool NodRecenter(int ringID);
     nodBool NodFlipY(int ringID);
    
     nodBool NodUnsubscribeToButton(int ringID);
     nodBool NodUnsubscribeToPose6D(int ringID);
     nodBool NodUnsubscribeToGesture(int ringID);
     nodBool NodUnsubscribeToPosition2D(int ringID);
    
    //for OTA, not exposing this directly through public SDK
     nodBool NodExistsFirmwareUpdate();
     char * NodAvaliableUpgradeVersion();
     nodBool NodBeingDownloadFirmwareUpdate();
     nodBool NodFirmwareDownloadComplete();
     nodBool NodReadyToOTA();
     nodBool NodBeginOTA(int ringID);
     float NodOTAProgress(int ringID);
     int NodCheckOTAErrorStatus(int ringID);
}