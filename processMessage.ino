void processMessage()
{
  // Check if processing an outbound control - skip this call if one is in progress
  if (controlOutboundFlag == true)
  {
    return;
  }
  // check for data request
  Serial.print("processMessage():");
  Serial.println(command);
  String getdatafrom = getValue(command,"getdata");
  String device = getValue(command,"d");
  String scanNumber = getValue(command,"s");
  String lid = getValue(command,"l");
  String ver = getValue(command,"v");
  String controlto = getValue(command,"ctl");
  String controlVal = getValue(command,"a");
  String pulseLen = getValue(command,"x");

  if (getdatafrom != "NULL")
  {
    //Serial.print("Got getdata request for:");
    //Serial.println(getdatafrom);
    //Serial.print("Device Type:");
    //Serial.println(device);
    loraID = getdatafrom;
    deviceType = device;
  }
  else if (controlto != "NULL")
  {
    Serial.print("Got control message=");
    loraID = controlto;
    deviceType = device;
    controlAction = controlVal;
    pulseDuration = pulseLen;
    //Serial.print(loraID + ",");
    //Serial.print(deviceType + ",");
    //Serial.print(pulseDuration + ",");
    //Serial.println(controlAction);
    // Set the flag to execute the outbound control next cycle
    controlOutboundFlag = true;
  }
  else if (command == "restart")
  {
    restart();
  }
  else if (command == "storeConfig")
  {
    storeConfig();
  }
  else if (scanNumber != "NULL")
  {
    scan = false;
    Serial.print("Adding device:");
    Serial.print(scanNumber + ",");
    Serial.print(device + ",");
    Serial.print(lid + ",");
    Serial.println(ver);
    sensors.addSensor(scanNumber.toInt(),lid,device,ver,"true");  
  }
}
