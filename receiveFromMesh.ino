String receiveFromMesh()
{
  String response = "NULL";
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager->recvfromAck(buf, &len, &from))
  {
    #ifdef debug
    //Serial.print("receiveFromMesh() : 0x");
    //Serial.print(from, HEX);
    //Serial.print(": ");
    //Serial.println((char*)buf);
    #endif
    response = String((char*)buf);
  }
  return response;
}
