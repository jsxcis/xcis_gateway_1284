void sendToMesh(String outgoing, byte destination)
{
 
  uint8_t data[XCIS_RH_MESH_MAX_MESSAGE_LEN];
  outgoing.toCharArray((char*)data,sizeof(data));
  // Dont put this on the stack:
  uint8_t buf[XCIS_RH_MESH_MAX_MESSAGE_LEN];
  digitalWrite(LORA,1);
  Serial.print("sendToMesh():");
  Serial.print(destination);
  Serial.print(",");
  Serial.println(outgoing);
    
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
