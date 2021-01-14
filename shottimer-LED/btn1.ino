void btn1(){
  if(inSetupMode){
    //setup mode
      if (setting < SET6) {
        setting++;
      }
      else {
        setting = SET1;
      }  
  }
  display.clear();
}
