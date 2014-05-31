void playSong(unsigned char playBuffer[][7], int tempo, int length){
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
    Serial.print( playBuffer[i][1]);  
    Serial.print(" ");
    Serial.print( playBuffer[i][2]);  
    Serial.print(" ");
    Serial.print( playBuffer[i][3]);  
    Serial.print(" ");
    Serial.print( playBuffer[i][4]);  
    Serial.print(" ");
    Serial.print( playBuffer[i][5]);  
    Serial.print(" ");
    Serial.print( playBuffer[i][6]);  
    Serial.println(" ");

    data[0] = 1 << (int)playBuffer[i][1];
    data[1] = 1 << (int)playBuffer[i][2];
    data[2] = 1 << (int)playBuffer[i][3];
    data[3] = 1 << (int)playBuffer[i][4];
    data[4] = 1 << (int)playBuffer[i][5];
    data[5] = 1 << (int)playBuffer[i][6];

    nextMillis = nextMillis + (timeBase * playBuffer[i][0]);
    while(millis() <= nextMillis){
    } 
  }
}

