//
//  OpenSpatialBluetooth.h
//  Open Spatial iOS SDK
//
//  Copyright (c) 2014 Nod Labs. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreBluetooth/CoreBluetooth.h>
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

#define OTASERVICEUUID @"00000001-0000-1000-8000-A0E5E9000000"
#define OTADATAUUID @"00000103-0000-1000-8000-A0E5E9000000"
#define OTACONTROLUUID @"00000102-0000-1000-8000-A0E5E9000000"
#define OTASTATUSUUID @"00000109-0000-1000-8000-A0E5E9000000"
#define OTA_IMAGE_INFO_CHAR_UUID @"00000107-0000-1000-8000-A0E5E9000000"
#define NODCONTROLSERVUUID @"00000004-0000-1000-8000-A0E5E9000000"
#define NODCONTROLUUID @"00000402-0000-1000-8000-A0E5E9000000"
#define MODE_SWITCH_CHAR_UUID @"00000400-0000-1000-8000-A0E5E9000000"
#define DEVICE_INFO_SERV_UUID @"180F"
#define FIRMWARE_VERSION_CHAR_UUID @"2A26"
#define BATTERY_STATUS_CHAR_UUID @"2A19"

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
    NOD_CONTROL_SCREEN_RESOLUTION = 1, // u16.x u16.y
    NOD_CONTROL_INPUT_QUEUE_DEPTH = 2, // u8.depth
    NOD_CONTROL_CALIBRATE_ACCEL = 3, // <none>
    NOD_CONTROL_RECALIBRATE = 4, // <none>
    NOD_CONTROL_SHUTDOWN = 5, // <none>
    NOD_CONTROL_FLIP_Y = 6, // <none>
    NOD_CONTROL_FLIP_CW_CCW = 7, // <none>
    NOD_CONTROL_TAP_TIME = 8, // u16.ms
    NOD_CONTROL_DOUBLE_TAP_TIME = 9, // u16.ms
    NOD_CONTROL_READ_BATTERY = 10, // <none>
    NOD_CONTROL_TOUCH_SENSE = 11, // u8.data
    NOD_CONTROL_MODE_CHANGE = 12, // u8.data
    NOD_CONTROL_READ_SIGNATURE = 13, // <none>
    NOD_CONTROL_TOUCH_DEBUG = 14, // <none>
    NOD_CONTROL_SET_LEDS = 15, // u8.red u8.white u8.blue
    // 0-100: percent brightness, 0xff:auto
    NOD_CONTROL_FIND_TAG = 16, // u8[4] tag name, returns address & length
    NOD_CONTROL_READ_MEMORY = 17, // u32 address, u8 length
    NOD_CONTROL_SINGLE_CALIBRATE = 18, // u8.orientation, u16.samples
    NOD_CONTROL_FORGET_CLIENT = 19, // u8[6].bdaddr
    NOD_CONTROL_FORGET_ALL_CLIENTS = 20, // <none>
    NOD_CONTROL_REPORT_MODE = 21, // <none>
    NOD_CONTROL_COMPASS_CALIBRATED = 22, // <none>
    NOD_CONTROL_READ_SENSOR_BIASES = 23, // u16.samples
    
    NOD_CONTROL_NEXT_UNUSED,
    
    // The original idea was to split the NOD_CONTROL number space into
    // a low bank for incoming messages and a high bank for outgoing
    // messages, but that proved difficult to manage.  The current plan
    // is to use only the low bank.  NOD_CONTROL_DEV_SIGNATURE is the
    // sole exception, and it might be moved eventually.
    NOD_CONTROL_DEV_SIGNATURE = 0xfc,
    
    // NOD_CONTROL_TOUCH_DEBUG_DATA = 0xfd, // Deprecated; Use NOD_CONTROL_TOUCH_DEBUG
    // NOD_CONTROL_READ_CONTROL     = 0xfe, // Deprecated; use NOD_CONTROL_REPORT_MODE
    // NOD_CONTROL_UNUSED           = 0xff,
};

@interface NodDevice : NSObject

@property CBPeripheral* BTPeripheral;
@property CBCharacteristic* gestureCharacteristic;
@property CBCharacteristic* pose6DCharacteristic;
@property CBCharacteristic* pointerCharacteristic;
@property CBCharacteristic* buttonCharacteristic;
@property CBCharacteristic* OTADataCharacteristic;
@property CBCharacteristic* OTAControlCharacteristic;
@property CBCharacteristic* OTAStatusCharacteristic;
@property CBCharacteristic* OTAImageInfo;
@property CBCharacteristic* nControlCharacteristic;
@property CBCharacteristic* batteryCharacteristic;
@property NSMutableDictionary* subscribedTo;
@property bool foundOSChars;
@property bool foundOTAChars;
@property bool foundBatteryChars;
@property bool foundnControlChars;

@end
/*!
 Delegate for the OpenSpatialBluetooth object implementing classes will
 recieve OpenSpatialEvents
 */
@protocol OpenSpatialBluetoothDelegate <NSObject>

/*!
 called when a button event is fired from Nod
 */
-(ButtonEvent *)buttonEventFired: (ButtonEvent *) buttonEvent;
/*!
 called when a pointer event is fired from Nod
 */
-(PointerEvent *)pointerEventFired: (PointerEvent *) pointerEvent;
/*!
 called when a rotation event is fired from Nod
 */
-(RotationEvent *)rotationEventFired: (RotationEvent *) rotationEvent;
/*!
 called when a gesture event is fired from Nod
 */
-(GestureEvent *)gestureEventFired: (GestureEvent *) gestureEvent;
/*!
 called when a Nod is connected to from connectToPeripheral
 */
- (void) didConnectToNod: (CBPeripheral*) peripheral;
/*
 called when a new Nod is found from scanForPeripherals
 */
- (void) didFindNewScannedDevice: (NSArray*) peripherals;
- (void) didFindNewPairedDevice: (NSArray*) peripherals;
- (void) didReadBatteryLevel:(NSInteger) level forRingNamed:(NSString*) name;
- (void) didReadFirmwareVersion: (NSInteger) version forRingNamed:(NSString*) name;

@end

@interface OpenSpatialBluetooth: NSObject

@property CBCentralManager *centralManager;
@property NSMutableArray *foundPeripherals;
@property NSMutableArray *pairedPeripherals;
@property NSMutableDictionary *connectedPeripherals; 
@property NSMutableArray* peripheralsToConnect;
@property id<OpenSpatialBluetoothDelegate> delegate;

/*!
 Singleton constructor method
 */
+(id) sharedBluetoothServ;
-(id) delegate;
/*!
 sets the delegate
 */
-(void) setDelegate:(id<OpenSpatialBluetoothDelegate>)newDelegate;

/*!
 * Scans for for only peripherals with the Open Spatial UUID adding all peripherals to the peripherals array
 */
-(void) scanForPeripherals;

/*!
 * Connect to a peripheral device store as connected device, also stops scan
 * @param peripheral - the peripheral that the central manager will connect to
 */
-(void) connectToPeripheral: (CBPeripheral *) peripheral;

/*!
 * Returns an Array Containing the names of all the services associated with a device
 * @param peripheral
 */
-(void) getServicesForConnectedDevice:(CBPeripheral *)peripheral;

/*!
 * Checks to see if user subscribed to certain set of events
 * @param type - the string that is used to check for a certain event
 * @param peripheralName - the name of the peripheral that is compared to in the dictionary
 */
-(BOOL)isSubscribedToEvent:(NSString *)type forPeripheral:(NSString *)peripheralName;


/*!
 * Method used in unit tests to ensure that characteristic and data being sent by a peripheral device is being captured
 *
 * @param characteristic - the characteristic that is passed through the function to determine which events to execute upon
 * @param peripheral - the peripheral that is passed through the function so that user can know which peripheral device executed which events
 */
-(NSArray *)testBluetoothCharacteristic:(CBCharacteristic *)characteristic andPeripheral:(CBPeripheral *)peripheral;

/*!
 * Subscribes the specified peripheral device to the rotation events
 *
 * @param peripheralName - the name of the peripheral that will connect to rotation events
 */
-(void)subscribeToRotationEvents:(NSString *)peripheralName;
-(void)unsubscribeFromRotationEvents: (NSString *)peripheralName;

/*!
 * Subscribes the specified peripheral device to gesture events
 *
 * @param peripheralName - the name of the peripheral that will connect to gesture events
 */
-(void)subscribeToGestureEvents:(NSString *)peripheralName;
-(void)unsubscribeFromGestureEvents: (NSString *)peripheralName;

/*!
 * Subscribes the specified peripheral device to button events
 *
 * @param peripheralName - the name of the peripheral that will connect to button events
 */
-(void)subscribeToButtonEvents:(NSString *)peripheralName;
-(void)unsubscribeFromButtonEvents: (NSString *)peripheralName;

/*!
 * Subscribes the specified peripheral device to pointer events
 *
 * @param peripheralName - the name of the peripheral that will connect to pointer events
 */
-(void)subscribeToPointerEvents:(NSString *)peripheralName;
-(void)unsubscribeFromPointerEvents: (NSString *)peripheralName;

/*!
 *nControl Methods
 *
 */
-(void) writeShutdown: (NSString*) name;
-(void) writeRecalibrate: (NSString*) name;
-(void) writeLeftySwitch: (NSString*) name;
-(void) writeInvertY: (NSString*) name;
-(void) writeScreenRes: (CGPoint) res name: (NSString*) name;
-(void) writeMode: (uint8_t) mode name: (NSString*) name;

-(void) readBatteryLevel: (NSString*) name;
-(void) readCurrentFirmware:(NSString*) name;

@end


