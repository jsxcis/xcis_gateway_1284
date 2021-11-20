void initaliseScanList(int scanListLength)
{

  //for (int i = 0; i < configuration.scanListLength; i++)
  for (int i = 0; i < scanListLength; i++)
  {
    Serial.print("Adding sensor:");
    Serial.print(configuration.scanList[i].loraID);
    Serial.print(",");
    Serial.println(configuration.scanList[i].deviceType);
    sensors.addSensor(i, configuration.scanList[i].loraID, configuration.scanList[i].deviceType,"true");
  }
}
