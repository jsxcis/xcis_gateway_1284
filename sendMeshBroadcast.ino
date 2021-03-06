void sendMeshBroadcast()
{
   byte destination = 0xFE;

  Serial.print("sendMesgBroadcast():");
  digitalWrite(LORA,1);
  xcisMessage.createCommandPayload(STATUS_REQUEST,nodeId );
  // Meed tp check device type to send
  
  xcisMessage.createMessage(data,locationID, GATEWAY, STATUS_REQUEST);
  xcisMessage.dumpHex(data,sizeof(data));
  
  if (manager->sendtoWait(data, sizeof(data), /*RH_BROADCAST_ADDRESS*/ destination) == RH_ROUTER_ERROR_NONE) // Temporary send to 0xFE - new sensor default address
  {
    // Status update for successful send
   
  }
  else
  {
    // Status update for send error
    
  }
  Serial.println("sendMeshBroadcast():done");
  digitalWrite(LORA,0);
}
