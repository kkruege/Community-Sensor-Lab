void initializeClient() {
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
