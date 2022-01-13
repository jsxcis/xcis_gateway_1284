void controlToMesh()
{
  byte destination = 0x00;
 
  Serial.print("controlToMesh():");
  Serial.println(loraID);
  digitalWrite(LORA,1);
  destination = loraID.toInt();
  pulseDuration_int = pulseDuration.toInt();
  Serial.println(pulseDuration_int);
  Serial.println(pulseDuration_int,HEX);
  
  
  xcisMessage.createCommandPayload(CONTROL_ON,pulseDuration_int,nodeId );
  // Meed tp check device type to send
  if (deviceType == "Bore")
  {
    Serial.println("Creating Bore message");
    if (controlAction == "START")
    {
      xcisMessage.createMessage(data,locationID, BORE_CONTROLLER, CONTROL_ON);
    }
    else
    {
      xcisMessage.createMessage(data,locationID, BORE_CONTROLLER, CONTROL_OFF);
    }
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
  Serial.println("controlToMesh():done");
  digitalWrite(LORA,0);
}
