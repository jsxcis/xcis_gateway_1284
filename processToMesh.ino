void processToMesh(String loraID, String deviceType)
{
  byte destination = 0x00;
 
  Serial.print("processToMesh():");
  Serial.println(loraID);
  digitalWrite(LORA,1);
  destination = loraID.toInt();
  xcisMessage.createCommandPayload(SENSOR_DATA_REQUEST,nodeId );
  // Meed tp check device type to send
  if (deviceType == "RainGauge")
  {
    xcisMessage.createMessage(data,locationID, RAIN_GAUGE, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else if (deviceType = "FlowMeter")
  {
    xcisMessage.createMessage(data,locationID, FLOW_METER, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else
  {
    Serial.println("Unknown device type");
    return;
  }
  // Send a message to a rf22_mesh_server
  // A route to the destination will be automatically discovered.
  
  if (manager->sendtoWait(data, sizeof(data), destination) == RH_ROUTER_ERROR_NONE)
  {
    // Status update for successful send
   
  }
  else
  {
    // Status update for send error
    
  }
  Serial.println("sendToMesh():done");
  digitalWrite(LORA,0);
}
