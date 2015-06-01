#include "unityPlugin.h"


extern "C"
{
    nodBool NodInitialize(void)
    {
        //Initialize SDK classes
        openSpatial = [OpenSpatialBluetooth sharedBluetoothServ];
        delegate = [[pluginHelper alloc] initWithBluetooth:openSpatial];
        ringControl = [BluetoothConnection sharedBluetoothServ];
        
        return (openSpatial != NULL) && (delegate != NULL);
    }
    
    nodBool NodShutdown(void)
    {
        return true;
    }
    
    int NodNumRings(void)
    {
        return (int) [delegate.nods count];
    }
    
    const char* NodGetRingName(int ringID)
    {
        if(ringID >= [delegate.nods count])
        {
            return "index outside range";
        }
            
        return [[[delegate.nods objectAtIndex:ringID] name]
                cStringUsingEncoding:NSUTF8StringEncoding];
    }
    
    nodBool NodSetMode(int ringID, int mode)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial writeMode:mode forName:name];
        return true;
    }
    
    nodBool NodSubscribeToButton(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial subscribeToButtonEvents:name];
        return true;
    }
    
    nodBool NodSubscribeToPose6D(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial subscribeToRotationEvents:name];
        return true;
    }
    
    nodBool NodSubscribeToGesture(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial subscribeToGestureEvents:name];
        return true;
    }
    
    nodBool NodSubscribeToPosition2D(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial subscribeToPointerEvents:name];
        return true;
    }
    
    int NodGetButtonState(int ringID)
    {
        return [delegate getButtonState:ringID];
    }
    
    NodEulerOrientation NodGetEulerOrientation(int ringID)
    {
        return [delegate getOrientation:ringID];
    }
    
    NodQuaternionOrientation NodGetQuaternionOrientation(int ringID)
    {
        NodEulerOrientation euler = [delegate getOrientation:ringID];
        NSLog(@"iOS Plugin Queried Euler: %f", euler.pitch);
        NodQuaternionOrientation quat = [pluginHelper eulerToQuaternionYaw:euler.yaw Pitch:euler.pitch Roll:euler.roll];
        NSLog(@"iOS Plugin Queried Quat: %f", quat.x);
        return quat;
    }
    
    NodPointer NodGetPosition2D(int ringID)
    {
        return [delegate getPointerPosition:ringID];
    }
    
    int NodGetGesture(int ringID)
    {
        return [delegate getMostRecentGesture:ringID];
    }
    
     float NodBatteryPercent(int ringID)
    {
        return 0.5f;
    }
    
     nodBool NodExistsFirmwareUpdate()
    {
        return false;
    }
    
     char * NodAvaliableUpgradeVersion()
    {
        return "000.000.000";
    }
    
     nodBool NodBeingDownloadFirmwareUpdate()
    {
        return false;
    }
    
     nodBool NodFirmwareDownloadComplete()
    {
        return false;
    }
    
     nodBool NodReadyToOTA()
    {
        return false;
    }
    
     nodBool NodBeginOTA(int ringID)
    {
        return false;
    }
    
     float NodOTAProgress(int ringID)
    {
        return 0.5f;
    }
    
     int NodCheckOTAErrorStatus(int ringID)
    {
        return 0;
    }
    
     int NodReadMode(int ringID)
    {
        return 0;
    }
    
     nodBool NodShutdownRing(int ringID)
    {
        return false;
    }
    
    const  char* NodCurrentFirmwareVersion(int ringID)
    {
        return "000.000.000";
    }
    
    const  char* NodDeviceAddress(int ringID)
    {
        return "00:AA:BB:CC:DD:EE:FF";
    }
    
     nodBool NodRecalibrate(int ringID)
    {
        return true;
    }
    
     nodBool NodRecenter(int ringID)
    {
        return true;
    }
    
     nodBool NodFlipY(int ringID)
    {
        return true;
    }
    
     nodBool NodUnsubscribeToButton(int ringID)
    {
        return true;
    }
    
     nodBool NodUnsubscribeToPose6D(int ringID)
    {
        return true;
    }
    
     nodBool NodUnsubscribeToGesture(int ringID)
    {
        return true;
    }
    
     nodBool NodUnsubscribeToPosition2D(int ringID)
    {
        return true;
    }
}


