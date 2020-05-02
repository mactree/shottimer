void btn3(){
        if(inSetupMode){
        // in setup mode
        int value[] = {0,1,99,7,7,20,1};
          if (settingSet[setting] < value[setting]) {
          settingSet[setting]++;
           }
          else {
          settingSet[setting] = value[setting];
          }
          //settingSet[setting] = settingSet[setting] -1;
          int adr = (setting);
          EEPROM.update(adr, settingSet[setting]);   
          }
                display.clear();
}
