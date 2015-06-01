#import "pluginHelper.h"



@implementation pluginHelper

-(id) initWithBluetooth: (OpenSpatialBluetooth*) blue
{
    self = [super init];
    if(self)
    {
        self.bluetooth = blue;
        self.bluetooth.delegate = self;
        self.nods = [[NSMutableArray alloc] init];
        self.pos2Ds = [[NSMutableArray alloc] init];
        self.gestures = [[NSMutableArray alloc] init];
        self.orientations = [[NSMutableArray alloc] init];
        self.buttonStates = [[NSMutableArray alloc] init];

    }
    return self;
}

-(void) didFindNewDevice:(NSArray *)peripherals
{
    NSLog(@"found new device");
    for(CBPeripheral* perph in peripherals)
    {
        NSLog(@"calling connect to %@", perph.name);
        [self.bluetooth connectToPeripheral:perph];
    }
}

-(void) didConnectToNod:(CBPeripheral *)peripheral
{
    NSLog(@"Connected");
    [self.nods addObject:peripheral];
    PointerEvent* pointerEvent = [[PointerEvent alloc] init];
    RotationEvent* rotationEvent = [[RotationEvent alloc] init];
    GestureEvent* gestureEvent = [[GestureEvent alloc] init];
    ButtonEvent* buttonEvent = [[ButtonEvent alloc] init];
    [self.pos2Ds addObject:pointerEvent];
    [self.gestures addObject:gestureEvent];
    [self.orientations addObject:rotationEvent];
    [self.buttonStates addObject:buttonEvent];
    NSLog(@"num connected devices:%d", [self.nods count]);
}

-(NSString*) getNameFromId:(int)id
{
    return [[self.nods objectAtIndex:id] name];
}

-(int) indexFromName: (NSString*) name
{
    for(int i = 0; i < [self.nods count]; i++)
    {
        if([[[self.nods objectAtIndex:i] name] isEqualToString:name])
        {
            return i;
        }
    }
    return -1;
}

-(PointerEvent*) pointerEventFired:(PointerEvent *)pointerEvent
{
    int index = [self indexFromName:pointerEvent.peripheral.name];
    [self.pos2Ds replaceObjectAtIndex:index withObject:pointerEvent];
    return pointerEvent;
}

-(GestureEvent*) gestureEventFired:(GestureEvent *)gestureEvent
{
    int index = [self indexFromName:gestureEvent.peripheral.name];
    [self.gestures replaceObjectAtIndex:index withObject:gestureEvent];
    return gestureEvent;
}

-(RotationEvent*) rotationEventFired:(RotationEvent *)rotationEvent
{
    int index = [self indexFromName:rotationEvent.peripheral.name];
    [self.orientations replaceObjectAtIndex:index withObject:rotationEvent];
    return rotationEvent;
}

-(ButtonEvent*) buttonEventFired:(ButtonEvent *)buttonEvent
{
    int index = [self indexFromName:buttonEvent.peripheral.name];
    [self.buttonStates replaceObjectAtIndex:index withObject:buttonEvent];
    return buttonEvent;
}

-(NodPointer) getPointerPosition:(int)ringID
{
    PointerEvent* pEvent = [self.pos2Ds objectAtIndex:ringID];
    int x = pEvent.xVal;
    int y = pEvent.yVal;
    if(x != 0 && y != 0)
    {
        pEvent.xVal = 0;
        pEvent.yVal = 0;
    }
    NodPointer pointer;
    pointer.x = x;
    pointer.y = y;
    return pointer;
}

-(NodEulerOrientation) getOrientation:(int)ringID
{
    RotationEvent* rEvent = [self.orientations objectAtIndex:ringID];
    NodEulerOrientation orientation;
    orientation.pitch = rEvent.pitch;
    orientation.yaw = rEvent.yaw;
    orientation.roll = rEvent.roll;
    return orientation;
}

-(int) getButtonState:(int)ringID
{
    ButtonEvent* event = [self.buttonStates objectAtIndex:ringID];
    int state = event.myButtonEventNum;
    switch (state) {
        case TOUCH0_DOWN:
            state |= 1 << 0;
            break;
        case TOUCH0_UP:
            state &= ~(1 << 0);
            break;
        case TOUCH1_DOWN:
            state |= 1 << 1;
            break;
        case TOUCH1_UP:
            state &= ~(1 << 1);
            break;
        case TOUCH2_DOWN:
            state |= 1 << 2;
            break;
        case TOUCH2_UP:
            state &= ~(1 << 2);
            break;
        case TACTILE0_DOWN:
            state |= 1 << 3;
            break;
        case TACTILE0_UP:
            state &= ~(1 << 3);
            break;
        case TACTILE1_DOWN:
            state |= 1 << 4;
            break;
        case TACTILE1_UP:
            state &= ~(1 << 4);
            break;
        default:
            //PAW this shouldn't happen
            break;
    }
    return state;
}

-(int) getMostRecentGesture:(int)ringID
{
    GestureEvent* gEvent = [self.gestures objectAtIndex:ringID];
    if(gEvent.myGestureEventNum == NO_GESTURE)
    {
        return -1;
    }
    else
    {
        return gEvent.myGestureEventNum;
    }
}

+(NodQuaternionOrientation) eulerToQuaternionYaw:(float)yaw Pitch:(float)pitch Roll:(float)roll
{
    float sinHalfYaw = sin(yaw / 2.0f);
    float cosHalfYaw = cos(yaw / 2.0f);
    float sinHalfPitch = sin(pitch / 2.0f);
    float cosHalfPitch = cos(pitch / 2.0f);
    float sinHalfRoll = sin(roll / 2.0f);
    float cosHalfRoll = cos(roll / 2.0f);
    
    NodQuaternionOrientation result;
    result.x = -cosHalfRoll * sinHalfPitch * sinHalfYaw
    + cosHalfPitch * cosHalfYaw * sinHalfRoll;
    result.y = cosHalfRoll * cosHalfYaw * sinHalfPitch
    + sinHalfRoll * cosHalfPitch * sinHalfYaw;
    result.z = cosHalfRoll * cosHalfPitch * sinHalfYaw
    - sinHalfRoll * cosHalfYaw * sinHalfPitch;
    result.w = cosHalfRoll * cosHalfPitch * cosHalfYaw
    + sinHalfRoll * sinHalfPitch * sinHalfYaw;
    
    return result;
}


@end