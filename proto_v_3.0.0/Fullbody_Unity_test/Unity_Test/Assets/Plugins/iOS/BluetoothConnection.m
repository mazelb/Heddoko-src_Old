/*
 * Copyright 2014 Nod Labs
 */

#import "BluetoothConnection.h"


@implementation BluetoothConnection

+ (id)sharedBluetoothServ {
    static BluetoothConnection *sharedBluetoothServ = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedBluetoothServ = [[self alloc] init];
    });
    return sharedBluetoothServ;
}

- (void) scanForPeripherals
{
    self.scannedPeripherals = [[NSMutableArray alloc] init];
    self.pairedPeripherals = [[NSMutableArray alloc] init];
    self.centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil options:nil];
}

-(void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    if(central.state == CBCentralManagerStatePoweredOn)
    {
        [self.centralManager scanForPeripheralsWithServices:nil options:nil];
        [self.centralManager retrieveConnectedPeripherals];
    }
    else
    {
        NSLog(@"Bluetooth Off");
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI
{
    if(![self.scannedPeripherals containsObject:peripheral])
    {
        [self.scannedPeripherals addObject:peripheral];
        NSLog(@"Found Peripheral: %@",peripheral.name);
        if(self.delegate)
        {
            [self.delegate didFindScannedDevice:peripheral];
        }
    }
}

-(void)centralManager:(CBCentralManager *)central didRetrieveConnectedPeripherals:(NSArray *)peripherals
{
    [self.pairedPeripherals addObjectsFromArray:peripherals];
    for(CBPeripheral* peripheral in self.pairedPeripherals)
    {
        if(self.delegate)
        {
            [self.delegate didFindPairedDevice:peripheral];
        }
        if([peripheral.name isEqualToString:self.connectTo])
        {
            [self connectToPeripheral:peripheral];
        }
    }
}

-(void) connectToPeripheral: (CBPeripheral *) peripheral
{
    NSLog(@"connecting");
    [self.centralManager stopScan];
    self.connectedPeripheral = peripheral;
    [self.centralManager connectPeripheral:self.connectedPeripheral options:nil];
    self.connectedPeripheral.delegate = self;
}

-(void) centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    NSLog(@"connected to: %@",peripheral.name);
    [self.connectedPeripheral discoverServices:nil];
}

-(void) peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
    NSLog(@"Discovering Services");
    for(CBService* serv in peripheral.services)
    {
        [self.connectedPeripheral discoverCharacteristics:nil forService:serv];
    }
}

-(void) peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service
             error:(NSError *)error
{
    bool foundData = false;
    bool foundControl = false;
    bool foundStatus = false;
    bool foundNodControl = false;
    if([service.UUID.UUIDString isEqualToString:OTASERVICEUUID])
    {
        for(CBCharacteristic* chara in service.characteristics)
        {
            if([chara.UUID.UUIDString isEqualToString:OTACONTROLUUID])
            {
                NSLog(@"Found Control");
                [self.delegate didFindOTAControlChar:chara];
                foundControl = true;
            }
            if([chara.UUID.UUIDString isEqualToString:OTADATAUUID])
            {
                NSLog(@"Found Data");
                [self.delegate didFindOTADataChar:chara];
                foundData = true;
            }
            if([chara.UUID.UUIDString isEqualToString:OTASTATUSUUID])
            {
                NSLog(@"Found Status");
                [self.delegate didFindOTAStatusChar:chara];
                foundStatus = true;
            }
        }
    }
    else if([service.UUID.UUIDString isEqualToString:NODCONTROLSERVUUID])
    {
        for(CBCharacteristic* chara in service.characteristics)
        {
            if([chara.UUID.UUIDString isEqualToString:NODCONTROLUUID])
            {
                NSLog(@"Found Nod Control");
                [self.delegate didFindNodControlChar:chara];
                foundNodControl = true;
            }
        }
    }
    else if([service.UUID.UUIDString isEqualToString:DEVICE_INFO_SERV_UUID])
    {
        for(CBCharacteristic* chara in service.characteristics)
        {
            if([chara.UUID.UUIDString isEqualToString:FIRMWARE_VERSION_CHAR_UUID])
            {
                NSLog(@"Found Firmware Version");
                [peripheral readValueForCharacteristic:chara];
            }
        }
    }
    if (foundData && foundControl && foundStatus)
    {
        [self.delegate doneSettingUpConnection];
    }
}

-(void) peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:
(CBCharacteristic *)characteristic error:(NSError *)error
{
    if([characteristic.UUID.UUIDString isEqualToString:FIRMWARE_VERSION_CHAR_UUID])
    {
        NSData* val = characteristic.value;
        NSString* firmwareVersion = [[NSString alloc] initWithData:val encoding:NSUTF8StringEncoding];
        [self.delegate didFindFirmwareVersion:firmwareVersion];
    }
    if([characteristic.UUID.UUIDString isEqualToString:MODE_SWITCH_CHAR_UUID])
    {
        NSData* val = characteristic.value;
        uint8_t* modeP = val.bytes;
        uint8_t mode = modeP[0];
        [self.delegate didFindInitialMode:mode];
    }
}

-(void) centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"disconnect");
    [self.delegate didDisconnectFromPeripheral];
    [self performSelector:@selector(reconnect) withObject:nil afterDelay:5.0];
}

-(void) reconnect
{
    [self connectToPeripheral:self.connectedPeripheral];
}

-(void) peripheral:(CBPeripheral *)peripheral didModifyServices:(NSArray *)invalidatedServices
{
    NSLog(@"services modified");
}

@end
