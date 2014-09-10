void playSong(unsigned long playBuffer[], unsigned char lengthOfNote[], int tempo, int length){
  clearLEDs();
  delay(1000);
  fillLEDs();
  delay(1000);
  clearLEDs();
  delay(1000);
  double timeBase = (7.5/(double)tempo) * 1000;
  //long startMillis = millis();
  long nextMillis = millis();
  for(int i = 0; i != length; i++){
    //displayFingering(i);
    Serial.print( (int)((playBuffer[i] >> 25) & 31));  
    Serial.print(" ");
    Serial.print( (int)((playBuffer[i] >> 20) & 31));  
    Serial.print(" ");
    Serial.print( (int)((playBuffer[i] >> 15) & 31));  
    Serial.print(" ");
    Serial.print( (int)((playBuffer[i] >> 10) & 31));  
    Serial.print(" ");
    Serial.print( (int)((playBuffer[i] >> 5) & 31));  
    Serial.print(" ");
    Serial.print( (int)((playBuffer[i]) & 31));  
    Serial.println(" ");

    data[5] = 1 << (int)((playBuffer[i] >> 25) & 31);
    data[4] = 1 << (int)((playBuffer[i] >> 20) & 31);
    data[3] = 1 << (int)((playBuffer[i] >> 15) & 31);
    data[2] = 1 << (int)((playBuffer[i] >> 10) & 31);
    data[1] = 1 << (int)((playBuffer[i] >>  5) & 31);
    data[0] = 1 << (int)((playBuffer[i]) &       31);

    nextMillis = nextMillis + (timeBase * lengthOfNote[i]);
    while(millis() <= nextMillis){
    } 
    
     if (buttonpress() == 2) {
       break;
       clearLEDs();
     }
  }
}

