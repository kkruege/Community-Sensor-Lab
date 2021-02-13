/*
Based on WiFiSSLClient.ino in Wifi101 examples written by Arturo Guadalupi last revision November 2015
*/

void wifiSetup(){
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
  
  
  }
