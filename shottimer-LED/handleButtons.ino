byte handleButton1() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(2); // pin low -> pressed

  if (buttonNowPressed) {
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN1_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN1_SHORTPRESS;
  }

  return BTN1_NONE;
}

byte handleButton2() {
  static bool buttonWasPressed; // previous state
  static bool longButtonPress;
  static unsigned long longButtonPressedTime; // press running duration

  bool buttonNowPressed = !digitalRead(3); // pin low -> pressed

  if (buttonNowPressed) {
    if (!buttonWasPressed) {
      longButtonPressedTime = millis() + LONGPRESS_TIME;
      buttonWasPressed = true;
    }
    else if (!longButtonPress && (millis() >= longButtonPressedTime)) {
      longButtonPress = true;
      return BTN2_LONGPRESS;
    }
  }
  else if (buttonWasPressed) {
    buttonWasPressed = false;
    longButtonPress = false;
    if (millis() < longButtonPressedTime)
      return BTN2_SHORTPRESS;
  }

  return BTN2_NONE;
}
