void processMessage()
{
  // check for data request
  //Serial.println(command);
  String instruction = getValue(command,"getdata");
  String device = getValue(command,"dev");
  String scanNumber = getValue(command,"sn");
  String lid = getValue(command,"lid");

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
    Serial.println(lid);
   sensors.addSensor(scanNumber.toInt(),lid,device);  
  }
}
