boolean configUpdate()
{
  //Serial.println("Processing Config change");
 
}
void setDefaults()
{
  //Serial.println("Setting Defaults");
  
  for (int i = 0; i< 3000; i++)
  {
    //EEPROM.write(i,0);
  }
  //EEPROM.commit();
  delay(100);
  
  //EEPROM_writeAnything(0, configuration);
  //EEPROM.commit();
}
void writeConfiguration()
{
  Serial.println("writeConfiguration");
  
  for (int i = 0; i< 3000; i++)
  {
    EEPROM.write(i,0);
  }
  //EEPROM.commit();
  delay(100);
  
  EEPROM_writeAnything(0, configuration);
  //EEPROM.commit();
}
void displayConfiguration() //debug
{
  Serial.println("Configuration");
  EEPROM_readAnything(0, configuration);
  
  for (int i = 0; i < configuration.scanListLength; i++)
  {
    Serial.print(configuration.scanList[i].loraID);
    Serial.print(",");
    Serial.print(configuration.scanList[i].deviceType);
    Serial.print(":");
  }
  Serial.println("End configuration");
  Serial.print("Size of config:");
  Serial.println(sizeof(configuration));
}
void readConfiguration()
{
  EEPROM_readAnything(0, configuration);
}
void setScanListLength(int length )
{
  //readConfiguration();
  //configuration.scanListLength = length;
  //EEPROM_writeAnything(3000, length);
  //EEPROM.commit();
}
int getScanListLength()
{
  int scanLength = 10; // temporarily set default
 // EEPROM_readAnything(3000, scanLength);
  return scanLength;
}
void setDefaultScanlist()
{
  Serial.println("Setting Default ScanList");
  // setup scanlist
  strcpy(configuration.scanList[0].loraID, "02");
  strcpy(configuration.scanList[0].deviceType, "Device");
  strcpy(configuration.scanList[1].loraID, "03");
  strcpy(configuration.scanList[1].deviceType, "Device");
  strcpy(configuration.scanList[2].loraID, "04");
  strcpy(configuration.scanList[2].deviceType, "Device");
  strcpy(configuration.scanList[3].loraID, "05");
  strcpy(configuration.scanList[3].deviceType, "Device");
  strcpy(configuration.scanList[4].loraID, "06");
  strcpy(configuration.scanList[4].deviceType, "Device");
  strcpy(configuration.scanList[5].loraID, "07");
  strcpy(configuration.scanList[5].deviceType, "Device");
  strcpy(configuration.scanList[6].loraID, "08");
  strcpy(configuration.scanList[6].deviceType, "Device");
  strcpy(configuration.scanList[7].loraID, "09");
  strcpy(configuration.scanList[7].deviceType, "Device");
  strcpy(configuration.scanList[8].loraID, "10");
  strcpy(configuration.scanList[8].deviceType, "Device");
  strcpy(configuration.scanList[9].loraID, "11");
  strcpy(configuration.scanList[9].deviceType, "Device");
  strcpy(configuration.scanList[10].loraID, "12");
  strcpy(configuration.scanList[10].deviceType, "Device");
  strcpy(configuration.scanList[11].loraID, "13");
  strcpy(configuration.scanList[11].deviceType, "Device");
  strcpy(configuration.scanList[12].loraID, "14");
  strcpy(configuration.scanList[12].deviceType, "Device");
  strcpy(configuration.scanList[13].loraID, "15");
  strcpy(configuration.scanList[13].deviceType, "Device");
  strcpy(configuration.scanList[14].loraID, "16");
  strcpy(configuration.scanList[14].deviceType, "Device");
  strcpy(configuration.scanList[15].loraID, "17");
  strcpy(configuration.scanList[15].deviceType, "Device");
  strcpy(configuration.scanList[16].loraID, "18");
  strcpy(configuration.scanList[16].deviceType, "Device");
  strcpy(configuration.scanList[17].loraID, "19");
  strcpy(configuration.scanList[17].deviceType, "Device");
  strcpy(configuration.scanList[18].loraID, "20");
  strcpy(configuration.scanList[18].deviceType, "Device");
  strcpy(configuration.scanList[19].loraID, "21");
  strcpy(configuration.scanList[19].deviceType, "Device");
  
 
  //configuration.scanListLength = 1;
  
  //EEPROM_writeAnything(0, configuration);
 //EEPROM .commit();
 
}
boolean setScanLength()
{
  int scanLength;
  String value;
  /*
  if (server->args() >= 1)
  {
          Serial.print("Setting ScanLength = ");
          value = server->arg(0);
          scanLength = value.toInt();
          Serial.println(scanLength);
          setScanListLength(scanLength);
          return true;
  }
  */
  return false;
}
