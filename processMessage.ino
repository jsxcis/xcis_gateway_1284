void processMessage()
{
  // check for data request
  Serial.print("processMessage():");
  Serial.println(command);
  String instruction = getValue(command,"getdata");
  String device = getValue(command,"d");
  String scanNumber = getValue(command,"s");
  String lid = getValue(command,"l");
  String ver = getValue(command,"v");

  if (instruction != "NULL")
  {
    //Serial.print("Got getdata request for:");
    //Serial.println(instruction);
    //Serial.print("Device Type:");
    //Serial.println(device);
    loraID = instruction;
    deviceType = device;
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
