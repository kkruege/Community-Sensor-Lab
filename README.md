# citizen-sensors
/*
  Real time clock & SD card from datalogger + LCD + K30 (on i2c).
  Avoiding Arduinos Leonardos because of strange choking on SD logfile.print().
  The i2c address on the K30 had to be changed because it was conflicting with address of RTC (0x68). 
  Must be set to 0x30 using Gaslab software from co2meter.com. 
  Based on Arduino_to_I2C.ino by Marv Kausch from co2Meter.com
  Ricardo Toledo-Crow, NGENS ASRC CUNY, 2.6.2020
*/
