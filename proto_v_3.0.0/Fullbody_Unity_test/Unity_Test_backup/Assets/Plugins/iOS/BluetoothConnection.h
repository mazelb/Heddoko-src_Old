/*
 * Copyright 2014 Nod Labs
 */

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#define OTASERVICEUUID @"00000001-0000-1000-8000-A0E5E9000000"
#define OTADATAUUID @"00000103-0000-1000-8000-A0E5E9000000"
#define OTACONTROLUUID @"00000102-0000-1000-8000-A0E5E9000000"
#define OTASTATUSUUID @"00000109-0000-1000-8000-A0E5E9000000"
#define NODCONTROLSERVUUID @"00000004-0000-1000-8000-A0E5E9000000"
#define NODCONTROLUUID @"00000402-0000-1000-8000-A0E5E9000000"
#define MODE_SWITCH_CHAR_UUID @"00000400-0000-1000-8000-A0E5E9000000"
#define DEVICE_INFO_SERV_UUID @"180A"
#define FIRMWARE_VERSION_CHAR_UUID @"2A26"

@protocol BluetoothConnectionProtocol <NSObject>

-(void) didFindScannedDevice:(CBPeripheral*) peripheral;
-(void) didFindPairedDevice:(CBPeripheral*) peripheral;
-(void) didFindOTADataChar:(CBCharacteristic*) chara;
-(void) didFindOTAControlChar:(CBCharacteristic*) chara;
-(void) didFindOTAStatusChar:(CBCharacteristic*) chara;
-(void) doneSettingUpConnection;
-(void) didConnectToPeripheral:(CBPeripheral*) peripheral;
-(void) didDisconnectFromPeripheral;
-(void) didFindNodControlChar:(CBCharacteristic*) chara;
-(void) didFindFirmwareVersion:(NSString*) firmwareString;
-(void) didFindModeSwitchChar:(CBCharacteristic*) chara;
-(void) didFindInitialMode: (uint8_t) modeNum;

@end

@interface BluetoothConnection : NSObject
<CBCentralManagerDelegate,CBPeripheralDelegate>

@property CBCentralManager *centralManager;
@property NSMutableArray *scannedPeripherals;
@property NSMutableArray *pairedPeripherals;
@property CBPeripheral *connectedPeripheral;
@property id<BluetoothConnectionProtocol> delegate;
@property NSString *connectTo;

-(void) scanForPeripherals;
-(void) connectToPeripheral: (CBPeripheral *) peripheral;
+(id) sharedBluetoothServ;

@end