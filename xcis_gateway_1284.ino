// Beta Version 14/12/2020

#include <XcisSensor.h>
#include <RHRouter.h>
#include <RHMesh.h>
#include <RH_RF95.h>
#include <Wire.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include "EEPROMAnything.h"

uint8_t nodeId = 98; // Gateway LoraID

#include <EEPROM.h>

#define debug

String board = "__AVR_ATmega1284P__";
#define RFM95_CS 4
#define RFM95_RST 3
#define RFM95_INT 2


#define XCIS_RH_MESH_MAX_MESSAGE_LEN 32

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

XcisSensor sensors;

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

String meshCommand = "";
String loraID ="";
String deviceType = "";


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
  Serial.println("Scan sensor:");
    int sensorToScan = sensors.scanNextSensor();
    if (sensorToScan != -1)
    {
      String req = sensors.getSensorLoraID(sensorToScan) + ":REQUEST:D25:EOM";
      Serial.print("Scan sensor:");
      Serial.println(req);
      //digitalWrite(UPLINK, LOW);//ON
      processCommand(req);
     // digitalWrite(UPLINK, HIGH);//OFF
    }
  //digitalWrite(UPLINK, LOW);//ON
}
char rx_byte = 0;
String rx_str = "";
SoftwareSerial mySerial(10, 11); // RX, TX
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

    //setScanListLength(3); // Number of nodes to scan
    //int numberActiveSensors = 0;
   // numberActiveSensors = getScanListLength();
    //Serial.println();
    //Serial.print("Scan list length is:");
    //Serial.println(numberActiveSensors);
    //initaliseScanList(numberActiveSensors);
    //sensors.listSensors(true);
    
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
  
  if (scanLoopCount == 10000)
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
  heartBeat++;
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
  
  checkOnlineLoopCount++;
  processResponse(receiveFromMesh());
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
  Serial.println("Storing config***********");
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
  String responseBrief = sensors.getSensorDataBrief(loraID,deviceType) + sensors.getDeviceMode(loraID) + ",";
  //Serial.println(responseBrief);
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
