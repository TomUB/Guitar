void toggleRandom(){
  toggleLED(random(0,22),random(0,6));  
}

void fillRandom(){
  setLED(random(0,22),random(0,6));
}

void clearRandom(){
  clearLED(random(0,22),random(0,6));
}

void rain(){
  static float thres = 5;
  int i = 0;
  
   data[0] = data[0] << 1;
   data[1] = data[1] << 1;
   data[2] = data[2] << 1;
   data[3] = data[3] << 1;
   data[4] = data[4] << 1;
   data[5] = data[5] << 1;
   
  if(random(0,50) <= thres)
    data[0] += 1;
  if(random(0,50) <= thres)
    data[1] += 1;
  if(random(0,50) <= thres)
    data[2] += 1;
  if(random(0,50) <= thres)
    data[3] += 1;
  if(random(0,50) <= thres)
    data[4] += 1;
  if(random(0,50) <= thres)
    data[5] += 1; //0x200000;
   
   for(int i = 0; i <= 100; i++)
   {
     while(timestamp == prevTimestamp){
       delayMicroseconds(1);
     }
     prevTimestamp = timestamp;
   }
   //thres += .1;
}
