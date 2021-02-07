/*
   Write to Google Seets through a Wifi POST HTTP1.1 request to a Google script.

  Based on WiFiSSLClient.ino in Wifi101 examples written by Arturo Guadalupi last revision November 2015

*/

#include <SPI.h>
#include <WiFi101.h>

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "script.google.com";    // name address for Google scripts as we are communicationg with the scripg (using DNS)

WiFiSSLClient client; // make SSL client

// these are the commands to be sent to the google script: namely add a row to last in Sheet1 with the values TBD
String payload_base =  "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
String payload = "";

void setup() {
  WiFi.setPins(8, 7, 4, 2);   //Configure pins for Adafruit ATWINC1500 Feather
  Serial.begin(9600);   //Initialize serial and wait for port to open:
  delay(3000);

  while (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    //while (true);
    delay(500);
  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
   
    status = WiFi.begin(ssid, pass);   // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(10000);

    
    // output some diagnostic info
    Serial.println(WiFi.status());
  }
  printWiFiStatus();

  Serial.print("\nStarting connection to server "); 
  if (client.connectSSL(server, 443)) {      // if you get a connection, report back via serial:
    Serial.print("connected to server: ");
    Serial.println(server);
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
  else {
    Serial.print("No connection to server: "); Serial.println(server);
    while (1);
  }
  
  payload = payload_base + "\"" + 1234 + "," + 5678 + "\"}";

  Serial.printf("\npayload: %s\n", payload.c_str());
  Serial.println(payload);

  // Make a HTTP request:
  client.println("POST /macros/s/AKfycbxvedWZKcWNE3P0dnJwhIConXjVyRa-D9R2zARd1QUHiWlHTKksBU1i3w/exec?value=Hello HTTP/1.1");
  client.println("Host: script.google.com");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);
  client.println();

  Serial.printf("\n Response \n\n");
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Connected to WiFi ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
