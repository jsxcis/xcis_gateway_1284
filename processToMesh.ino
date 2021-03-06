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
  else if (deviceType == "FlowMeter")
  {
    xcisMessage.createMessage(data,locationID, FLOW_METER, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else if (deviceType == "Tank")
  {
    xcisMessage.createMessage(data,locationID, TANK, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else if (deviceType == "Trough")
  {
    xcisMessage.createMessage(data,locationID, TROUGH, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else if (deviceType == "Fence")
  {
    xcisMessage.createMessage(data,locationID, FENCE, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else if (deviceType == "Bore")
  {
    xcisMessage.createMessage(data,locationID, BORE_CONTROLLER, SENSOR_DATA_REQUEST);
    xcisMessage.dumpHex(data,sizeof(data));
  }
  else
  {
    Serial.println("Unknown device type");
    return;
  }
  // Send a message to a rf22_mesh_server
  // A route to the destination will be automatically discovered.
  //Serial.print("Message size:");
  //Serial.println(sizeof(data));
  if (manager->sendtoWait(data, sizeof(data), destination) == RH_ROUTER_ERROR_NONE)
  {
    // Status update for successful send
   
  }
  else
  {
    // Status update for send error
    
  }
  //Serial.println("processToMesh():done");
  digitalWrite(LORA,0);
}
