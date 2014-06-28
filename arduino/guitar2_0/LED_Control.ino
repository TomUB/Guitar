void updateBuffer(){
  static int c = 0;
  
   c++;
   if(c == 6) c = 0;
    
   /*digitalWriteFast(8, HIGH);
   digitalWriteFast(9, HIGH);
   digitalWriteFast(4, HIGH);
   digitalWriteFast(5, HIGH);
   digitalWriteFast(6, HIGH);
   digitalWriteFast(7, HIGH);*/ 
   
   digitalWriteFast(latchPin, 0);
   
   shiftOutS(dataPin, clockPin, MSBFIRST, 0);
   shiftOutS(dataPin, clockPin, MSBFIRST, (128>>c));  
   shiftOutS(dataPin, clockPin, MSBFIRST, (data[c] >> 16));
   shiftOutS(dataPin, clockPin, MSBFIRST, (data[c] >> 8));
   shiftOutS(dataPin, clockPin, MSBFIRST, (data[c]));
   
   digitalWriteFast(20,HIGH);
   delayMicroseconds(75);
   
   digitalWriteFast(latchPin, 1);
 
   delayMicroseconds(75);
   digitalWriteFast(20,LOW);
   
   //digitalWriteFast(9-c, LOW);
   
   timestamp = micros();
}

void shiftOutS(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
      uint8_t i;

      for (i = 0; i < 8; i++)  {
            if (bitOrder == LSBFIRST)
                  digitalWriteFast(dataPin, !!(val & (1 << i)));
            else      
                  digitalWriteFast(dataPin, !!(val & (1 << (7 - i))));
                  
            digitalWriteFast(clockPin, HIGH);
            digitalWriteFast(clockPin, LOW);            
      }
}

void setLED(int x, int y){
  data[y] |= (1 << x); 
}

void clearLED(int x, int y){
   data[y] &= ~(1 << x);
}

void toggleLED(int x, int y){
   data[y] ^= 1 << x;
}

void fillLEDs(){
   data[0] = ~(1 << 23);
   data[1] = ~(1 << 23);
   data[2] = ~(1 << 23);
   data[3] = ~(1 << 23);
   data[4] = ~(1 << 23);
   data[5] = ~(1 << 23); 
}

void clearLEDs(){
   data[0] = 0;
   data[1] = 0;
   data[2] = 0;
   data[3] = 0;
   data[4] = 0;
   data[5] = 0; 
}
