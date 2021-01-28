File initializeSD(uint8_t CS)  {

  File logFile;

  if (!SD.begin(CS)) {  // see if the card is present and can be initialized:
    Serial.println("Card failed, or not present.");
    stat = stat | 0x01; // set bit 1 if SD error
    //while (1);
  }
  else  {
    Serial.println("card initialized.");
    char filename[] = "LOG0000.TXT";  // create a new file//increase 0000
    for (uint16_t i = 0; i <= 9999; i++) {
      filename[3] = i / 1000 + '0';    //48 ascii character value of 0 //integer division by
      filename[4] = i / 100 - 10 * (i / 1000) + '0';
      filename[5] = i / 10 - 10 * (i / 100) + '0'; //integer division 100
      filename[6] = i % 10 + '0';   //modulo 10
      //Serial.println(filename);
      if (!SD.exists(filename)) {  // only open a new file if it doesn't exist
        logFile = SD.open(filename, FILE_WRITE);
        break;  // leave the loop
      }
    }
    if (!logFile) {
      Serial.println("Couldn't create file");
      stat = stat | 0x02;
      //while (1);
    }
    else  {
      Serial.print("Logging to file: ");
      Serial.println(filename);
    }
  }
  return logFile;
}
