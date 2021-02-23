/*
Write to Google Sheets through a Wifi POST HTTP1.1 request to a Google script.
*/
void payloadUpload(){
  payload = payload_base + "\"" + T + "," + P + "," + H + "," + C + "\"}";

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
  client.stop();
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    
  }

  }
