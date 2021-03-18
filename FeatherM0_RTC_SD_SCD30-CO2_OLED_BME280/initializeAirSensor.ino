// Sensirion SCD30 CO2 sensor

void initializeAirSensor()  {
  if (airSensor.begin() == false) {
    Serial.println("SCD30 CO2 sensor not detected");
    stat = stat | 0x08;
    //while (1);
  }
  else  {
    Serial.println("SCD30 ok");
    // Current ambient pressure mBar: 700 to 1200
    // 0=deactivate press. correction. Default 1013.25 mBar
    airSensor.setAmbientPressure(0);
    // de-activate Automatic Self-Calibraton
    airSensor.setAutoSelfCalibration(false);
//    // set forced recalibrationFactor
//    airSensor.setForcedRecalibrationFactor(453);
    // Change measurement interval seconds 2-1800
    airSensor.setMeasurementInterval(30);
  }
}
