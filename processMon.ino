void processMon(String command)
{
  //Serial.print("CommandEvent:");
  //Serial.println(command);
  // Check for command line request
  if (command == "lsr")
  {
    Serial.println("Displaying Routing Table");
   manager->printRoutingTable();
  }
  if (command == "lss")
  {
    Serial.println("Sensor List");
    Serial.println(sensors.listSensors(true));
  }
  if (command == "reset")
  {
    Serial.println("Resetting...");
    restart();
  }
  // check for data request
  String instruction = getValue(command,"getdata");
  //String deviceType = getValue(command,"dev");
  if (instruction != NULL)
  {
    Serial.print("Got getdata request for:");
    Serial.print(instruction);
    meshCommand = "getData";
    loraID = instruction;
  }
}
String getValue(String message,String name)
{
     // Returns first token 
    char *tk = strtok((char*)message.c_str(), ",");
    String token;
    String value;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (tk != NULL)
    {
        //Serial.println(tk);
        token = tk;
        int found = token.indexOf(name);
       
        if (found!=-1)
        {
            int nameLength = name.length();
            // allow for '='sign
            nameLength++;
            value = token.substring(nameLength,token.length());
            //Serial.print("Value is :");
            //Serial.println(value);
            return value;
        }
        tk = strtok(NULL, ",");
    }
    return "NULL";
}
