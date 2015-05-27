#import "OpenSpatialBluetooth.h"
#import "ButtonEvent.h"
#import "PointerEvent.h"
#import "GestureEvent.h"
#import "BluetoothConnection.h"

#ifdef _cplusplus
extern "C"
{
#endif
    
    typedef struct NodEulerOrientation_
    {
        float pitch;
        float roll;
        float yaw;
    } NodEulerOrientation;
    
    typedef struct NodQuaternionOrientation_
    {
        float x;
        float y;
        float z;
        float w;
    } NodQuaternionOrientation;
    
    typedef struct NodPointer_
    {
        int x;
        int y;
    } NodPointer;
    
    //Not exposing these structure externally, just using them to track if data has been reported to the user yet or not
    typedef struct InternalNodGesture_
    {
        int gestureType;
        bool userInspectedValue;
    } InternalNodGesture;
    
    typedef struct InternalNodPointer_
    {
        int x;
        int y;
        bool userInspectedValue;
    } InternalNodPointer;
#ifdef _cplusplus
}
#endif


@interface pluginHelper : NSObject <OpenSpatialBluetoothDelegate, BluetoothConnectionProtocol>

@property OpenSpatialBluetooth* bluetooth;
@property NSMutableArray* nods;
@property NSMutableArray* buttonStates;
@property NSMutableArray* orientations;
@property NSMutableArray* gestures;
@property NSMutableArray* pos2Ds;

-(id) initWithBluetooth: (OpenSpatialBluetooth*) blue;
-(NSString*) getNameFromId: (int) id;
+(NodQuaternionOrientation) eulerToQuaternionYaw: (float) yaw Pitch:(float) pitch Roll: (float) roll;
-(int) getButtonState: (int) ringID;
-(NodEulerOrientation) getOrientation:(int) ringID;
-(NodPointer) getPointerPosition: (int) ringID;
-(int) getMostRecentGesture: (int) ringID;

@end