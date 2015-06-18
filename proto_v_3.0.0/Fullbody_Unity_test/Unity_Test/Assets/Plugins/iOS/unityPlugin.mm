#include "unityPlugin.h"


extern "C"
{
    nodBool NodInitialize(void)
    {
        //Initialize SDK classes
        NSLog(@"initialize");
        openSpatial = [OpenSpatialBluetooth sharedBluetoothServ];
        delegate = [[pluginHelper alloc] initWithBluetooth:openSpatial];
        
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
    
    nodBool NodSubscribeToGameControl(int ringID)
    {
        return true;
    }

    nodBool NodUnsubscribeToButton(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial unsubscribeFromButtonEvents:name];
        return true;
    }
    
    nodBool NodUnsubscribeToPose6D(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial unsubscribeFromRotationEvents:name];
        return true;
    }
    
    nodBool NodUnsubscribeToGesture(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial unsubscribeFromGestureEvents:name];
        return true;
    }
    
    nodBool NodUnsubscribeToPosition2D(int ringID)
    {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial unsubscribeFromPointerEvents:name];
        return true;
    }
    
    nodBool NodUnSubscribeToGameControl(int ringID)
    {
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
    
    NodPointer NodGetGamePosition(int ringID)
    {
        NodPointer p;
        p.x = 0;
        p.y = 0;
        return p;
    }
    
    int NodGetTrigger(int ringID)
    {
        return 0;
    }

    //Request and Get for Ring info
    int NodRequestBatteryPercentage(int ringID) {
        NSString* name = [delegate getNameFromId:ringID];
        [openSpatial readBatteryLevel:name];
        return 0;
    }
    int NodGetBatteryPercentage(int ringID) {
        return [[delegate.batteryPercentages objectAtIndex:ringID] intValue];
    }
    
}


