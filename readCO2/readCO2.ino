int readCO2()
{ int co2_value = 0;  // We will store the CO2 value inside this variable.

  Wire.beginTransmission(K30Addr);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);
  Wire.endTransmission();
 
  delay(10); // wait 10ms for the sensor to process command

  // read 4 bytes: payload two bytes, checksum, status
  Wire.requestFrom(K30Addr, 4);

  byte i = 0;
  byte buffer[4] = {0, 0, 0, 0};

  while (Wire.available())
  { buffer[i] = Wire.read();
    i++;
  }

  //shift buffer into integer for general consumption
  co2_value = 0;
  co2_value |= buffer[1] & 0xFF;
  co2_value = co2_value << 8;
  co2_value |= buffer[2] & 0xFF;

  byte sum = 0; //Checksum Byte
  sum = buffer[0] + buffer[1] + buffer[2]; //Byte addition uses overflow

  if (sum == buffer[3])
  { return co2_value; // Success!
  }
  else
  { return 0; // Failure!
  }
}
