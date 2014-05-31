int amountOfFiles(){
  int count = 0;
  while (file.openNext(sd.vwd(), O_READ)) {
    file.printName(&Serial);
    cout << ' ';
    file.printModifyDateTime(&Serial);
    cout << endl;
    file.close();
  }
}

void readInFile(const char* filename) {
  const int line_buffer_size = 23;
  char buffer[line_buffer_size];
  ifstream sdin(filename);
  int line_number = 0;
  
  int tempo = 0;
  
  while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
    int count = sdin.gcount();
    if (sdin.fail()) {
      //cout << "Partial long line";
      sdin.clear(sdin.rdstate() & ~ios_base::failbit);
    } else if (sdin.eof()) {
      //cout << "Partial final line";  // sdin.fail() is false
    } else {
      //count--;  // Don’t include newline in count
      //cout << "Line " << ++line_number;
      ++line_number;
    }
    //cout << " (" << count << " chars): " << buffer << endl;
    if (line_number == 1){
      tempo = atoi(buffer);
      continue;
    } else if (line_number == 2){
      int length = atoi(buffer) - 3;
      unsigned char playBuffer[length][7]; // setup buffer size
      int index = 0;
      char smallBuffer[4];
        while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
          int count = sdin.gcount();
          if (sdin.fail()) {
            //cout << "Partial long line";
            sdin.clear(sdin.rdstate() & ~ios_base::failbit);
          } else if (sdin.eof()) {
            //cout << "Partial final line";  // sdin.fail() is false
          } else {
            //count--;  // Don’t include newline in count
            //cout << "Line " << ++line_number;
            ++line_number;
          }
          
          int smallIndex = 0;
          int carryThrough = 0;
          
          for(int a = 0; buffer[a] != ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            //Serial.println(a);
            carryThrough = a;
            //Serial.println
          }
          carryThrough++;
          playBuffer[line_number - 3][0] = atoi(smallBuffer);
          
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
          
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            //Serial.println(buffer[a]);
            smallIndex++; 
            carryThrough = a;
          }
          
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][1] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][1] = 30;
          
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
          
          
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            carryThrough = a;
          }
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][2] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][2] = 30;
          
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
          
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            carryThrough = a;
          }
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][3] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][3] = 30;
          
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
          
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            carryThrough = a;
          }
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][4] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][4] = 30;
          
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
                   
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            carryThrough = a;
          }
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][5] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][5] = 30;
            
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
                   
          smallIndex = 0;
          for(int a = carryThrough + 1; buffer[a]!= ','; a++){
            smallBuffer[smallIndex] = buffer[a]; 
            smallIndex++; 
            carryThrough = a;
          }
          carryThrough++;
          
          if(smallBuffer[0] == '0' || smallBuffer[0] == '1' || smallBuffer[0] == '2' || smallBuffer[0] == '3' || smallBuffer[0] == '4' || 
             smallBuffer[0] == '5' || smallBuffer[0] == '6' || smallBuffer[0] == '7' || smallBuffer[0] == '8' || smallBuffer[0] == '9')
            playBuffer[line_number - 3][6] = atoi(smallBuffer);
          else
            playBuffer[line_number - 3][6] = 30;   
            
          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';
          smallBuffer[4] = ' ';
          smallBuffer[5] = ' ';
     
          for(int i = 0; i != 6; i++){
            //Serial.println((int)((float)line_number/(float)line_number*22.0));
             setLED(22-(int)((float)line_number/(float)length*22.0),i); 
             delayMicroseconds(500);
          }
        }
      playSong(playBuffer, tempo, length); 
    }
    break;
  }
}
