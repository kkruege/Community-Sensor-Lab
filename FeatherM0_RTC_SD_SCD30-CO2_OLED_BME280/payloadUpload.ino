/*
  Write to Google Sheets through a Wifi POST HTTP1.1 request to a Google script.
*/
void payloadUpload(String payload) {

  if (!client.connected()) {
    initializeClient();
  }

  payload = payload_base + "\"" + payload + "\"}";
  Serial.print("payload: "); Serial.println(payload);

  // Make a HTTP request:
  client.println("POST /macros/s/AKfycbzLiMdE-5IlsuBNTyAcxUdHkxCu7B1Sl_mVJF2-Tge5Y8Zv66d9/exec?value=Hello HTTP/1.1"); // for ricado's googlesheet
  client.println("Host: script.google.com");
  client.println("Content-Type: application/x-www-form-urlencoded");
  //client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);
  client.println();
  delay(200);

  Serial.println("Response: ");
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  Serial.println();

  client.stop();
  if (!client.connected()) {
    Serial.println("disconnected from server.");
  }
}


void initializeClient() {
  Serial.print("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {      // if you get a connection, report back via serial:
    Serial.print("connected to ");
    Serial.println(server);
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
  else {
    Serial.print("No connection to server: "); Serial.println(server);
    //while (1);
  }
}
