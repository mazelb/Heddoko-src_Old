//
//  OpenSpatialBluetooth.h
//  Open Spatial iOS SDK
//
//  Copyright (c) 2014 Nod Labs. All rights reserved.
//

#import <Foundation/Foundation.h>
#import  <CoreBluetooth/CoreBluetooth.h>
#import "OpenSpatialDecoder.h"

#import "ButtonEvent.h"
#import "PointerEvent.h"
#import "RotationEvent.h"
#import "GestureEvent.h"

#define OS_UUID @"00000002-0000-1000-8000-A0E5E9000000"
#define POS2D_UUID @"00000206-0000-1000-8000-A0E5E9000000"
#define TRANS3D_UUID @"00000205-0000-1000-8000-A0E5E9000000"
#define GEST_UUID @"00000208-0000-1000-8000-A0E5E9000000"
#define BUTTON_UUID @"00000207-0000-1000-8000-A0E5E9000000"

#define NODCONTROLSERVUUID @"00000004-0000-1000-8000-A0E5E9000000"
#define NODCONTROLUUID @"00000402-0000-1000-8000-A0E5E9000000"

#define BUTTON @"button"
#define POINTER @"pointer"
#define GESTURE @"gesture"
#define ROTATION @"rotation"

#define POS2D_SIZE 4
#define TRANS3D_SIZE 12
#define GEST_SIZE 5
#define BUTTON_SIZE 2

#define POINTER_MODE 0x00
#define GAME_MODE 0x01
#define THREE_D_MODE 0x02
#define FREE_POINTER_MODE 0x03

enum nod_control {
    NOD_CONTROL_NONE = 0,
    NOD_CONTROL_SCREEN_RESOLUTION,
    NOD_CONTROL_INPUT_QUEUE_DEPTH,
    NOD_CONTROL_RECENTER_POINTER,
    NOD_CONTROL_RECALIBRATE,
    NOD_CONTROL_SHUTDOWN,
    NOD_CONTROL_FLIP_Y,
    NOD_CONTROL_FLIP_CW_CCW,
    NOD_CONTROL_TAP_TIME,
    NOD_CONTROL_DOUBLE_TAP_TIME,
    NOD_CONTROL_READ_BATTERY_LEVEL,
    NOD_CONTROL_TOUCH_SENSE,
    NOD_CONTROL_MODE,
    NOD_CONTROL_READ = 0xfe,
    NOD_CONTROL_UNUSED = 0xff,
};


@protocol OpenSpatialBluetoothDelegate <NSObject>

-(ButtonEvent *)buttonEventFired: (ButtonEvent *) buttonEvent;
-(PointerEvent *)pointerEventFired: (PointerEvent *) pointerEvent;
-(RotationEvent *)rotationEventFired: (RotationEvent *) rotationEvent;
-(GestureEvent *)gestureEventFired: (GestureEvent *) gestureEvent;

- (void) didConnectToNod: (CBPeripheral*) peripheral;
- (void) didFindNewDevice: (NSArray*) peripherals;
@end

@interface OpenSpatialBluetooth: NSObject

@property NSMutableDictionary* modeMapping;

@property CBCentralManager *centralManager;
@property NSMutableArray *foundPeripherals;

@property NSMutableDictionary *connectedPeripherals;
@property NSMutableDictionary *nControlChars;

@property NSMutableArray *ConnectedServices;
@property id<OpenSpatialBluetoothDelegate> delegate;
@property NSMutableArray* peripheralsToConnect;

+(id) sharedBluetoothServ;
-(id) delegate;
-(void) setDelegate:(id<OpenSpatialBluetoothDelegate>)newDelegate;

/*
 * Scans for for only peripherals with the Open Spatial UUID adding all peripherals to the peripherals array
 */
-(void) scanForPeripherals;

/*
 * Connect to a peripheral device store as connected device, also stops scan
 * @param peripheral - the peripheral that the central manager will connect to
 */
-(void) connectToPeripheral: (CBPeripheral *) peripheral;

/*
 * Returns an Array Containing the names of all the services associated with a device
 * @param peripheral
 */
-(void) getServicesForConnectedDevice:(CBPeripheral *)peripheral;

/*
 * Checks to see if user subscribed to certain set of events
 * @param type - the string that is used to check for a certain event
 * @param peripheralName - the name of the peripheral that is compared to in the dictionary
 */
-(BOOL)isSubscribedToEvent:(NSString *)type forPeripheral:(NSString *)peripheralName;


/*
 * Method used in unit tests to ensure that characteristic and data being sent by a peripheral device is being captured
 *
 * @param characteristic - the characteristic that is passed through the function to determine which events to execute upon
 * @param peripheral - the peripheral that is passed through the function so that user can know which peripheral device executed which events
 */
-(NSArray *)testBluetoothCharacteristic:(CBCharacteristic *)characteristic andPeripheral:(CBPeripheral *)peripheral;

/*
 * Subscribes the specified peripheral device to the rotation events
 *
 * @param peripheralName - the name of the peripheral that will connect to rotation events
 */
-(void)subscribeToRotationEvents:(NSString *)peripheralName;

/*
 * Subscribes the specified peripheral device to gesture events
 *
 * @param peripheralName - the name of the peripheral that will connect to gesture events
 */
-(void)subscribeToGestureEvents:(NSString *)peripheralName;

/*
 * Subscribes the specified peripheral device to button events
 *
 * @param peripheralName - the name of the peripheral that will connect to button events
 */
-(void)subscribeToButtonEvents:(NSString *)peripheralName;

/*
 * Subscribes the specified peripheral device to pointer events
 *
 * @param peripheralName - the name of the peripheral that will connect to pointer events
 */
-(void)subscribeToPointerEvents:(NSString *)peripheralName;

-(void) writeShutdown: (NSString*) name;
-(void) writeRecalibrate: (NSString*) name;
-(void) writeLeftySwitch: (NSString*) name;
-(void) writeInvertY: (NSString*) name;
//-(void) writeScreenRes: (CGPoint) res forName: (NSString*) name;
-(void) writeRecenter: (NSString*) name;
-(void) writeMode: (uint8_t) mode forName: (NSString*) name;


@end