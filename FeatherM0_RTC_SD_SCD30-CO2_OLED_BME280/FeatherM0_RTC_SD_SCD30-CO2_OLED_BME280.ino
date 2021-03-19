/*
   SENSOR NODE: featherM0-Wifi + featherwing adalogger-SD-RTC + SCD30-CO2 + BME280-TPRH + OLED display.
   The SCD30 has a minimum power consumption of 5mA and cannot be stop-started. It's set to 55s (30s nominal)
   sampling period and the featherM0 sleeps for 2 x 16 =32s, wakes and waits for data available.
   Button A toggles display on/off but must be held down for 16s max and then wait 16s to toggle again.

   Logs: date, time, co2, t, rh, t, press, rh, battery voltage, status

   RICARDO TOLEDO-CROW NGENS, ESI, ASRC, CUNY, SEPTEMBER 2020

*/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_SleepyDog.h>
#include "RTClib.h"
#include "SparkFun_SCD30_Arduino_Library.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" // wifi name and password in .h file. see tab

#define VBATPIN A7 // this is also D9 button A disable pullup to read analog
#define BUTTON_A  9  // Oled button also A7 enable pullup to read button
#define BUTTON_B  6  // oled button
#define BUTTON_C  5  // oled button


char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
char server[] = "script.google.com"; // name address for Google scripts as we are communicationg with the scripg (using DNS)
int loopCount = 0;
float C; // CO2
float T; // temperature
float P; // pressure
float H; // humidity

WiFiSSLClient client; // make SSL client
// these are the commands to be sent to the google script: namely add a row to last in Sheet1 with the values TBD
String payload_base =  "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
String payload = "";

RTC_PCF8523 rtc; // Real Time Clock for RevB Adafruit logger shield
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire); // the oled display
Adafruit_BME280 bme; // the bme tprh sensor
File logfile;  // the logging file
SCD30 airSensor; // sensirion scd30 ndir

const int SD_CS = 10; // Chip select for SD card default for Adalogger
uint8_t stat = 0; // status byte

void setup(void) {

  pinMode(VBATPIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  delay(5000); Serial.println(__FILE__);

  initializeWiFi();
  initializeOLED();
  logfile = initializeSD(SD_CS);
  initializeBME();
  initializeAirSensor();

  Wire.begin();  // connect to RTC
  if (!rtc.begin()) {
    Serial.println("RTC failed");
    stat = stat | 0x04; // 3rd bit set 'rtc not started'
    //while (1);
  }
  else
    Serial.println("RTC ok");
  Serial.println();
  // TO SET TIME at compile: run once to syncro then run again with line commented out
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  delay(2000);
  //Serial.println("Date______\tTime____\tCO2ppm\tTempC\tRH%\tTempC\tP_mBar\tRH%\tVbatMV\tstatus");
  logfile.println("Date______\tTime____\tCO2ppm\tTempC\tRH%\tTempC\tP_mBar\tRH%\tVbatMV\tstatus");
  
}

char outstr[100];
int32_t Tsleep = 0;
bool displayState = true;
bool buttonAstate = true;
int lastTimeToggle = 0;
int timeDebounce = 100;

void loop(void)  {
  loopCount++;
  uint8_t ctr = 0;
  stat = stat & 0xEF; // clear bit 4 for CO2 sensor

  T = bme.readTemperature();
  P = bme.readPressure() / 100; // for hPa
  H = bme.readHumidity();
  airSensor.setAmbientPressure(P); // update CO2 sensor to current pressure
  
  // wait for data avail on CO2 sensor
  while (!airSensor.dataAvailable()) {
    delay(1000);
    if (ctr > 31) {  // timeout is 31s
      stat = stat | 0x10; // set bit 4 timeout
      break;
    }
    else
      ctr += 1;
  }

  // read from co2 sensor
  digitalWrite(LED_BUILTIN, HIGH);
  uint16_t co2 = airSensor.getCO2();
  C = co2; // this step converts ot float
  float temp = airSensor.getTemperature();
  float rh = airSensor.getHumidity();
  digitalWrite(LED_BUILTIN, LOW);

  //client must disconnect in order to make a new post request
  if (!client.connected()) {
    initializeClient();
    payloadUpload();
  }
  
  DateTime now;
  now = rtc.now(); // fetch the date + time
  year = now.year();
  month = now.month();
  day = now.day();
  hour = now.hour();
  minute = now.minute();
  second = now.second();

  pinMode(VBATPIN, INPUT); // read battery voltage
  float measuredvbat = analogRead(VBATPIN) * 0.006445;
  pinMode(BUTTON_A, INPUT_PULLUP);

  sprintf(outstr, "%02u:%02u:%02u,%02u:%02u:%02u,%.2d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%x",
          now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
          co2, temp, rh, T, P, H, measuredvbat, stat);
  //Serial.println("Date    \tTime      \tCO2\tTemp(C)\tRH(%)\tvbat(mV)\tstatus");
  //Serial.println(outstr);
  logfile.println(outstr);
  logfile.flush();   // Write to disk. Uses 2048 bytes of I/O to SD card, power and takes time
  

  payloadUpload(outstr);

  for (int i = 1; i <= 2; i++)  {  // 32s =2x16s sleep
    displayState = toggleButton(BUTTON_A, displayState, buttonAstate, lastTimeToggle, timeDebounce);
    if (displayState)  { // turn display on with data
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("CO2 ppm "); display.print(co2);
      display.print("  V "); display.println(measuredvbat);
      display.print("T C     "); display.println(T);
      display.print("P mBar  "); display.println(P);
      display.print("RH%     "); display.println(H);
      display.display();
    }
    else  {  // turn display off
      display.clearDisplay();
      display.display();
    }
    int sleepMS = Watchdog.sleep();// remove comment after final push
    //delay(16000);
  }
}
