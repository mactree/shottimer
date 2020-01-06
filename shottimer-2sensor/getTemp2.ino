void getTemp2() {

  if (Sensor2.getTemperature(&temperature2)) {
    uint16_t tmp2 = temperature2;

    int r2 = tmp2 - previousTMP2;
    if ( r2 < 0) {
      r2 = -r2;
    }

    if (r2 < 50 || error2 >= 5) {
      previousTMP2 = tmp2;
      error2 = 0;
    }
    else {
      tmp2 = previousTMP2;
      error2++;
    }

    Serial.print(tmp2);
    Serial.print(" - ");
    Serial.println(r2);

    total2 = total2 - readings2[readIndex2];
    readings2[readIndex2] = tmp2;
    total2 = total2 + readings2[readIndex2];

    readIndex2 = readIndex2 + 1;

    if (readIndex2 >= numReadings) {
      readIndex2 = 0;
    }

    temperature2 = total2 / numReadings;
  }
  TEMP2 = Sensor2.calc_Celsius(&temperature2);

}
