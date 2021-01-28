bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce )  {

  if (digitalRead(button))  {
    buttonState = true;
//    Serial.print("buttonState "); Serial.println(buttonState);
    return state;
  }
  else if (buttonState && millis() - prevTime > debounce) {
    buttonState = false;
    prevTime = millis();
//    Serial.print("buttonState "); Serial.println(buttonState);
    return !state;
  }
  else
    return state;
}
