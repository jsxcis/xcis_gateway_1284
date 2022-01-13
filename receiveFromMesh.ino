String receiveFromMesh()
{
  String response = "NULL";
  uint8_t len = sizeof(buf);
  uint8_t from;
  String inbound_loraID = "";
  char dt[10];
  if (manager->recvfromAck(buf, &len, &from))
  {
    Serial.print("receiveFromMesh() : 0x");
    Serial.print(from,HEX);
    Serial.print(",0x");
    sprintf(dt,"%02X",from);
    //inbound_loraID = String(dt);
    inbound_loraID = convertLoraID(from);
    
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
        Serial.println(xcisMessage.getCommand(),HEX);
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
            Serial.println(pcm.accumulatedDataToken);
            response = "ID=" +inbound_loraID +  ",V=" + String(pcm.value) + ",T=" + String(pcm.accumulatedDataToken) + ",B=" + String(batVoltage) + ",";
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
            Serial.println(pcm.accumulatedDataToken);
            Serial.println(from);
            response = "ID=" + inbound_loraID +  ",V=" + String(pcm.value) + ",T=" + String(pcm.accumulatedDataToken)  + ",B=" + String(batVoltage) + ",";
            Serial.println(response);
            break;
          }
          case TROUGH:
          {
            Serial.println("TROUGH");
            //uint8_t recvPayload[28];
            //xcisMessage.getPayload(recvPayload);
            //xcisMessage.dumpHex(recvPayload,28);
            distance dist;
            xcisMessage.processDistancePayload(dist);
            Serial.println(dist.battery); // Need to div by 100.
            float batVoltage = (float) dist.battery/100.00;
            Serial.println(dist.value);
            Serial.println(from);
            response = "ID=" + inbound_loraID  + ",V=" + String(dist.value) + ",B=" + String(batVoltage)+ ",";
            Serial.println(response);
            break;
          }
          case TANK:
          {
            Serial.println("TANK");
            //uint8_t recvPayload[28];
            //xcisMessage.getPayload(recvPayload);
            //xcisMessage.dumpHex(recvPayload,28);
            distance dist;
            xcisMessage.processDistancePayload(dist);
            Serial.println(dist.battery); // Need to div by 100.
            float batVoltage = (float) dist.battery/100.00;
            Serial.println(dist.value);
            Serial.println(from);
            response = "ID=" + inbound_loraID + ",V=" + String(dist.value) +  ",B=" + String(batVoltage) + ",";
            Serial.println(response);
            break;
          }
          case FENCE:
          {
            Serial.println("FENCE");
            //uint8_t recvPayload[28];
            //xcisMessage.getPayload(recvPayload);
            //xcisMessage.dumpHex(recvPayload,28);
            voltage volts;
            xcisMessage.processVoltagePayload(volts);
            Serial.println(volts.battery); // Need to div by 100.
            float batVoltage = (float) volts.battery/100.00;
            Serial.println(volts.value);
            Serial.println(from);
            response = "ID=" + inbound_loraID + ",V=" + String(volts.value)   + ",B=" + String(batVoltage)+ ",";
            Serial.println(response);
            break;
          }
          case BORE_CONTROLLER:
          {
            Serial.println("BORE_CONTROLLER");
            uint8_t recvPayload[28];
            xcisMessage.getPayload(recvPayload);
            xcisMessage.dumpHex(recvPayload,28);
            boreStatus status;
            xcisMessage.processBorePayload(status);
            Serial.println(status.battery); // Need to div by 100.
            float batVoltage = (float) status.battery/100.00;
            Serial.println(status.currentValue);
            Serial.println(status.boreState);
            Serial.println(status.accumulatedPulses);
            Serial.println(status.accumulatedDataToken);
            Serial.println(from);
            response = "ID=" + inbound_loraID + ",V=" + String(status.currentValue) + 
                        ",P=" + String(status.accumulatedPulses)+
                        ",T=" + String(status.accumulatedDataToken)+
                         ",S=" + String(status.boreState)  +
                         ",B=" + String(batVoltage)+ ",";
            Serial.println(response);
            break;
          }
          default:
            Serial.println("UNKNOWN PAYLOAD"); 
        }
    }
    else if (ver == "2.0")
    {
        Serial.print("Old protocol message:");
        Serial.println(String((char*)buf));
        response = String((char*)buf); 
    }
    else
    {
        Serial.println("!!!!!Unknown Sensor - check for new");
        // Check this is a valid message
        String message =  String((char*)buf); 
        if (message.substring(0,2) == "ID")
        {
          Serial.println("!!!!!Invalid 3.0 message");
          return response;
        }
       // char out[2];
       // sprintf(out,"%02X",from);
       // String lid = String(out);
        //Serial.println(lid);
        // This is a new sensor - add to the list
        // Work out device type
        // Work out scanNumber to use - end of the list
        // Assign a Lora ID (use highest number + 1)
        // Version default is 3.0
        // Status is initally set to NEW
        xcisMessage.processMessage(buf);
        Serial.print(xcisMessage.getLocationID(),HEX);
        Serial.print(":");
        Serial.print(xcisMessage.getDeviceType(),HEX);
        Serial.print(":");
        Serial.println(xcisMessage.getCommand(),HEX);
        if (xcisMessage.getCommand() == STATUS_RESPONSE)
        {
              //sensors.addSensor(scanNumber.toInt(),lid,device,ver);
              Serial.print("DeviceType:");
              Serial.println(xcisMessage.convertDeviceTypeToString(xcisMessage.getDeviceType()));
              Serial.print("Adding record at:");
              Serial.println(sensors.getSensorListLength());
              sensors.addSensor(sensors.getSensorListLength(),inbound_loraID,xcisMessage.convertDeviceTypeToString(xcisMessage.getDeviceType()),"3.0","new");
        }
        
        sensor_status status;
        xcisMessage.processStatusPayload(status);
        Serial.println(status.uid,HEX);
       
        response = "ID=" + inbound_loraID + ",UID=" + String(status.uid) + ",";
        
    }
  }
  return response;
}
// Conversion function to a two digtial number as the main server expects a 2 digit format loraID
String convertLoraID(uint8_t from)
{
  char buf[10];
  String lid = "";
  if (from < 10)
  {
    sprintf(buf,"0%u",from);
    lid = (char*)buf;
  }
  else
  {
    sprintf(buf,"%u",from);  
    lid = (char*)buf;
  }
  return lid;
}
