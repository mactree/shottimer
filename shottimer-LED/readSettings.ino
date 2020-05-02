void readSettings() {

  
  // version check
  bool readFromEEPROM = EEPROM.read(0) == EEPROM_VALUE_VERSION;
  if (readFromEEPROM) {
    settingSet[SET1] = EEPROM.read(1);
    settingSet[SET2] = EEPROM.read(2);
    settingSet[SET3] = EEPROM.read(3);
    settingSet[SET4] = EEPROM.read(4);
    settingSet[SET5] = EEPROM.read(5);
    settingSet[SET6] = EEPROM.read(6);


  }
  else {
    // first time init
    
    // leading zero
    settingSet[SET1] = 0;
    EEPROM.put(1, settingSet[SET1]);
    
    // temp offset
    settingSet[SET2] = 0;
    EEPROM.put(2, settingSet[SET2]);

    // brigntness for temp
    settingSet[SET3] = 3;
    EEPROM.put(3, settingSet[SET3]);

    // brightness for counting
    settingSet[SET4] = 6;
    EEPROM.put(4, settingSet[SET4]);

    // how long last shot will shown
    settingSet[SET5] = 8;
    EEPROM.put(5, settingSet[SET5]);

    // e61 mode 
    settingSet[SET6] = 0;
    EEPROM.put(6, settingSet[SET6]);
        
    EEPROM.write(0, EEPROM_VALUE_VERSION);
  }
}
