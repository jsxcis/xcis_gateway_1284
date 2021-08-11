String processRequest()
{
   #ifdef debug
  //Serial.println("processRequest");
  #endif
  String loraID;
  String request = "";
  String response = "";
  /*
  if (server->args() >= 1)
  {
      //digitalWrite(UPLINK, LOW);//ON
      #ifdef debug
      //Serial.print("HTTP Req Data:");
      //Serial.println(server->arg(0));
      #endif
      request = server->arg(0) + "\n";
      //Serial.println(server->arg(1));
      String loraID;
      loraID = request.substring(0,2);
      //Serial.print("Request LoraID:");
      //Serial.println(loraID);
      response = sensors.getSensorData(loraID) + ",Status=" + sensors.getDeviceMode(loraID) + ",";
      //digitalWrite(UPLINK, HIGH);//OFF
      return(response);
  }
  */
  return "NOT FOUND";
}
