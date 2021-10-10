String receiveFromMesh()
{
  String response = "NULL";
  uint8_t len = sizeof(buf);
  uint8_t from;
  if (manager->recvfromAck(buf, &len, &from))
  {
    Serial.print("receiveFromMesh() : 0x");
    Serial.print(from,HEX);
    Serial.print(",0x");
    xcisMessage.dumpHex(buf,sizeof(buf));
    String ver = sensors.getSensorVersion(sensors.getSensorScanNumber(String(from)));
    if (ver == "3.0")
    {
        Serial.println("New protocol message:");
        xcisMessage.processMessage(buf);
        Serial.print(xcisMessage.getLocationID(),HEX);
        Serial.print(":");
        Serial.print(xcisMessage.getDeviceType(),HEX);
        Serial.print(":");
        Serial.print(xcisMessage.getCommand(),HEX);
        switch(xcisMessage.getDeviceType())
        {
          case RAIN_GAUGE:
          {
            Serial.println("RAIN_GAUGE:");
            //uint8_t recvPayload[28];
            //xcisMessage.getPayload(recvPayload);
            //xcisMessage.dumpHex(recvPayload,28);
            pulse_counter pcm;
            xcisMessage.processPulseCounterPayload(pcm);
            Serial.println(pcm.battery); // Need to div by 100.
            float batVoltage = (float) pcm.battery/100.00;
            Serial.println(pcm.value);
            Serial.println(pcm.timestamp);
            response = "ID=" + convertLoraID(from) + ",B=" + String(batVoltage) + ",V=" + String(pcm.value) + ",T=" + String(pcm.timestamp)  + ",";
            Serial.println(response);
            break;
          }
          case FLOW_METER:
          {
            Serial.println("FLOW_METER:");
            //uint8_t recvPayload[28];
            //xcisMessage.getPayload(recvPayload);
            //xcisMessage.dumpHex(recvPayload,28);
            pulse_counter pcm;
            xcisMessage.processPulseCounterPayload(pcm);
            Serial.println(pcm.battery); // Need to div by 100.
            float batVoltage = (float) pcm.battery/100.00;
            Serial.println(pcm.value);
            Serial.println(pcm.timestamp);
            response = "ID=" + convertLoraID(from) + ",B=" + String(batVoltage) + ",V=" + String(pcm.value) + ",T=" + String(pcm.timestamp)  + ",";
            Serial.println(response);
            break;
          }
          default:
            Serial.println("UNKNOWN PAYLOAD"); 
        }
    }
    else
    {
        Serial.print("Old protocol message:");
        Serial.println(String((char*)buf));
        response = String((char*)buf); 
    }
  }
 // Serial.println(response);
  return response;
}
// Conversion function to a two digtial number as the main server expects an 2 digit format loraID
String convertLoraID(uint8_t from)
{
  String loraId = "";
  if (from < 10)
  {
    loraId = "0" + String(from);
  }
  else
  {
    loraId = String(from);
  }
  return loraID;
}
