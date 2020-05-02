void btn2(){
        if(inSetupMode){
        // in setup mode
          if (settingSet[setting] > 0) {
          settingSet[setting]--;
           }
          else {
          settingSet[setting] = 0;
          }
          //settingSet[setting] = settingSet[setting] -1;
          int adr = (setting);
          EEPROM.update(adr, settingSet[setting]);   
          }
            display.clear();    
}
