// Beta Version 13/13/2022

#include <XcisSensor.h>
#include <XcisMessage.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include "EEPROMAnything.h"

const char compile_date[] = __DATE__ " " __TIME__;

uint8_t nodeId = 0x94; // Gateway LoraID
uint8_t locationID = 0x01; // Location ID of this gateway

#pragma pack(push, 1)
typedef struct
{
    uint8_t farmID;
    uint8_t sensorType;
    uint8_t command;
    uint8_t payload[28];
    uint8_t crc;
} message, *pmessage;

#pragma pack(pop)

#include <EEPROM.h>

#define debug

String board = "__AVR_ATmega1284P__";
#define RFM95_CS 4
#define RFM95_RST 3
#define RFM95_INT 2


#define XCIS_RH_MESH_MAX_MESSAGE_LEN 32
//uint8_t paydata[28] = {0x4a, 0x4f, 0x4e, 0x41, 0x54, 0x48, 0x41, 0x4e, 0x20, 0x45, 0x44, 0x47, 0x41, 0x52, 0x20, 0x53, 0x48, 0x41, 0x52, 0x50, 0x20, 0x30, 0x33, 0x30, 0x33, 0x36, 0x37, 0x41};
uint8_t paydata[28] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t data[XCIS_RH_MESH_MAX_MESSAGE_LEN];

#define RH_HAVE_SERIAL

// Singleton instance of the radio driver

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHMesh *manager;

// message buffer
uint8_t buf[XCIS_RH_MESH_MAX_MESSAGE_LEN];

unsigned long scanLoopCount = 0;
unsigned long checkOnlineLoopCount = 0;
unsigned long checkI2CCount = 0;
unsigned long heartBeat = 0;
unsigned long broadcastLoopCount = 0;

bool heartBeatOn = false;
String command; // Command from the PI
bool scan = false;
bool scanListStored = false;

// Analog PIN for Reset switch
#define RESET_SW A3
// Analog PIN for Sensor On Indicator
#define SENSOR_ON 0
// Analog PIN for LORA Active
//#define LORA A2
#define LORA 1

// Analog PIN for Battery input
const int BATTERY_VOLTAGE = A0;

void(* resetFunc)(void) = 0;

unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
unsigned long DELAY_TIME = 5000; // Scan Rate
unsigned long delayOnlineCheckStart = 0; // the time the delay started
bool delayOnlineCheck = false;
unsigned long CHECK_ONLINE_TIME = 60000; // 1 * 60000msec
bool controlOutboundFlag = false;

XcisSensor sensors;
XcisMessage xcisMessage;

struct sensor
{
  char loraID[16];
  char deviceType[16];
};

struct config_t
{
    sensor scanList[64];
    int scanListLength;
    unsigned long scanRate;
} configuration;

struct scanlist_t
{
    sensor scanList[64];
    int scanListLength;
} scanlist;

// For global controls and data requests
String meshCommand = "";
String loraID ="";
String deviceType = "";
String controlAction = "";
String pulseDuration = "2000"; // Default value
uint16_t pulseDuration_int = 2000;

char rx_byte = 0;
String rx_str = "";
SoftwareSerial mySerial(10, 11); // RX, TX

//=======================================================================
void checkOnline()
{
  Serial.println("CheckOnline-------------------------------------");
  //Serial.println(displayRoutes());
  sensors.checkSensorsOnline();

}
//String response = "NULL";
void scanSensors()
{
    //Serial.println("scanSensors");
    int sensorToScan = sensors.scanNextSensor();
    if (sensorToScan != -1)
    {
      String ver = sensors.getSensorVersion(sensorToScan);
      if (ver == "3.0")
      {
        //Serial.println("Using new protocol");
        String loraID = sensors.getSensorLoraID(sensorToScan);
        String deviceType = sensors.getSensorDeviceType(sensorToScan);
        Serial.print("Scan sensor:");
        Serial.print(loraID + ",");
        Serial.print(deviceType + ",");
        Serial.println(ver);
        processToMesh(loraID,deviceType);
      }
      else
      {
        String req = sensors.getSensorLoraID(sensorToScan) + ":REQUEST:D25:EOM";
        Serial.print("Scan sensor:");
        Serial.print(req + ",");
        Serial.println(ver);
        processCommand(req);
      }
    }
}
void setup()
{
    pinMode(RESET_SW,INPUT); // A3 Reset Switch
   //pinMode(PULSE,INPUT); // Pulse input
  
   pinMode(SENSOR_ON,OUTPUT);// A1, PC11 output LOW = ON SENSOR
   digitalWrite(SENSOR_ON,1);
   
   pinMode(LORA,OUTPUT); // A2, PCI2 output LOW = ON LORA
   digitalWrite(LORA,0);
    
   pinMode(RFM95_RST, OUTPUT); // LORA RESET

    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    delay(100);
    Serial.begin(9600);
   
    mySerial.begin(9600);
    Serial.print("XCIS MESH GW Compiled on:");
    Serial.println(compile_date);
    //setDefaultScanlist();
    //readConfiguration();
 
    displayConfiguration();
    manager = new RHMesh(rf95, nodeId);
    
    if (!manager->init()) {
      Serial.println(F("Radio init failed"));
    } else {
    Serial.println("Mesh initialisation done");
    }
    rf95.setTxPower(23, false);
    rf95.setFrequency(915.0);
    rf95.setCADTimeout(500);

    Wire.setClock(10000);
    Wire.begin(8);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);

    
    Serial.println("RHMesh LoRa Gateway init succeeded.");
    Serial.println("Ready");
    Serial.println();
    //mySerial.println("Hi from ATMEL");
    //mySerial.print("XCIS:");
    Serial.println("Hi from ATMEL");
    Serial.print("XCIS:");
    scan = false;
    scanListStored = false;
    wdt_enable(WDTO_8S);

    //test code
  
   
    //uint8_t recvdata[32] = {0x22,0x23,0x24,0x4a, 0x4f, 0x4e, 0x41, 0x54, 0x48, 0x41, 0x4e, 0x20, 0x45, 0x44, 0xFF, 0x41, 0xFF, 0x20, 0xFF, 0x48, 0x41, 0x52, 0x50, 0x20, 0x30, 0x33, 0x30, 0x33, 0x36, 0x37, 0x41 ,0xFF};   
    //xcisMessage.processMessage(recvdata);
    //Serial.println(xcisMessage.getLocationID(),HEX);
    //Serial.println(xcisMessage.getDeviceType(),HEX);
    //Serial.println(xcisMessage.getCommand(),HEX);
    //uint8_t recvPayload[28];
    //xcisMessage.getPayload(recvPayload);
    //xcisMessage.dumpHex(recvPayload,28);
    // end test code
}
void loop()
{
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    if (rx_byte != '\r') {
      // a character of the string was received
      rx_str += rx_byte;
    }
    else {
      // end of string
      Serial.print("xcis:");
      Serial.println(rx_str);
      //Serial.println(sensors.listSensors(true));
      //Serial.println(manager->printRoutingTableToString());
      processMon(rx_str);
      rx_str = "";                // clear the string for reuse
      Serial.println("");
      Serial.print("xcis:");
    }
  } // end: if (Serial.available() > 0)
  
  if (scanLoopCount == 50000)// normally 100,000
  {
    scanLoopCount = 0;
    if (scanListStored == true)
    {
      scanSensors();
    }
    //Serial.println(sensors.listSensors(true));
  }
  scanLoopCount++;
  if (checkOnlineLoopCount == 120000) // MAXINT is 4294967295
  {
    checkOnlineLoopCount = 0;
    if (scan == true)
    {
      checkOnline();
    }
  }
  checkOnlineLoopCount++;
  
  if (heartBeat == 1500) // MAXINT is 4294967295
  {
    heartBeat = 0;
    if (heartBeatOn == false)
    {
      digitalWrite(SENSOR_ON,1);
      heartBeatOn = true;
    }
    else if (heartBeatOn == true)
    {
       digitalWrite(SENSOR_ON,0);
       heartBeatOn = false;
    }
  }
  heartBeat++;
  
  //if (broadcastLoopCount == 40000) // MAXINT is 4294967295
  //{
  //  broadcastLoopCount = 0;
   
   //sendMeshBroadcast();
  //}
  //broadcastLoopCount++;
  
  processResponse(receiveFromMesh());
  processControls();
  checkI2CCount++;
  if (checkI2CCount == 100000)
  {
    // Counter expired
    checkI2CCount = 0;
    // Autorestart
    restart(); 
  }
  wdt_reset();
}
void restart()
{
  Serial.println("Restart called");
  {
    noInterrupts();
    wdt_enable(WDTO_15MS);
    while(1);
  }
}
void storeConfig()
{
  scan = false;
  //Serial.println("Storing config***********");
  //writeConfiguration();
  scanListStored = true;
  scan = true;
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {

  //Wire.write(command.c_str()); // respond with message of 6 bytes
  checkI2CCount = 0; // Got a message so still on the bus
  if (scanListStored == false)
  {
    // there is no scan list - return error
    Serial.println("NOSCANLIST,0");
    Wire.write("NOSCANLIST,0");
    return;
  }
  //String response = sensors.getSensorData(loraID) + ",Status=" + sensors.getDeviceMode(loraID) + ",";
  String sensorVersion = sensors.getSensorVersion(sensors.getSensorScanNumber(loraID));
  
  //Serial.print("Sensor Version:");
  //Serial.println(sensorVersion);
  String responseBrief = "";
  if (sensorVersion == "2.0")
  {
    responseBrief = sensors.getSensorDataBrief_v2(loraID) + sensors.getDeviceMode(loraID) + ",";
  }
  else if (sensorVersion == "3.0")
  {
    responseBrief = sensors.getSensorDataBrief_v3(loraID) + sensors.getDeviceMode(loraID) + ",";
  }
  else
  {
    // In this case likely that the scan list is incorrect - so reload
    scanListStored = false;
    return;
  }
  Serial.print("Retuning:");
  Serial.println(responseBrief);
  Wire.write(responseBrief.c_str());
}
void receiveEvent(int howMany)
{
  checkI2CCount = 0; // Got a message so still on the bus
  (void)howMany;  // cast unused parameter to void to avoid compiler warning
  command = "";
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    command += c;
    //Serial.print(c);         // print the character
  }
  processMessage();
  int x = Wire.read();    // receive byte as an integer
} 
