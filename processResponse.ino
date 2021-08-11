void processResponse(String sensorData)
{
  if (sensorData != "NULL")
  {
        String loraID = sensorData.substring(3,5);
        int scanID = sensors.getSensorScanNumber(loraID);
        sensors.setSensorData(scanID, sensorData);
        sensors.setDeviceMode(scanID,"ONLINE",millis());
        sensorData = "";
  } 
  
}
