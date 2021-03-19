// Adafruit OLED display 

void initializeOLED()  {
  Serial.print("OLED starting  ");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    // while(1);
  }
  else  {
    Serial.println("OLED ok");
    //display.display();
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    //display.clearDisplay();
    display.setCursor(0, 0);
    display.display();
  }
}
