void processCommand(String command)
{
  String dest_string;
  byte destination = 0x00;
  String messageType;
  
  dest_string = command.substring(0,2);

  messageType = command.substring(3,10);
  //Serial.print("processCommand():");
 // Serial.println(command);
  
  if (messageType == "REQUEST")
  {
    destination = dest_string.toInt();
    sendToMesh(command,destination);
  }
  /*
  else if (messageType == "INFORMA")
  {
    Serial.println(command);
  }
  else if (messageType == "COMMAND")
  {
    Serial.println(command);
  }
  */
}
