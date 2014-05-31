int countPTBFiles(){
  // open next file in root.  The volume working directory, vwd, is root
  char fileName[12];
  //sd.chdir("/",true);
  int fileCount = 0;
  while (file.openNext(sd.vwd(), O_READ)) {
    file.getFilename(fileName);
    int i = 0;
    while(fileName[i] != '.' && i <= 8)
      i++;
    if(fileName[i] == '.' && fileName[i + 1] == 'P' && fileName[i + 2] == 'T' && fileName[i + 3] == 'B'){
      fileCount++;
    } 
    file.close();
  }
  return fileCount;
}

void readInFile(const char* filename) {
  Serial.println(filename);
  const int line_buffer_size = 30;
  char buffer[line_buffer_size];
  ifstream sdin(filename);
  int line_number = 0;

  int tempo = 0;

  while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
    int count = sdin.gcount();
    if (sdin.fail()) {
      //cout << "Partial long line";
      sdin.clear(sdin.rdstate() & ~ios_base::failbit);
    } 
    else if (sdin.eof()) {
      //cout << "Partial final line";  // sdin.fail() is false
    } 
    else {
      //count--;  // Don’t include newline in count
      //cout << "Line " << ++line_number;
      line_number++;
    }
    if (line_number == 1){
      continue;
    }
    if (line_number == 2){
      tempo = atoi(buffer);
      continue;
    } 
    else if (line_number == 3){
      int length = atoi(buffer) - 3;
      int measures = 0;

      unsigned long playBuffer[length]; // setup buffer size
      unsigned char lengthOfNote[length];

      //unsigned char playBuffer[length][7]; // setup buffer size
      int index = 0;
      char smallBuffer[5];
      while (sdin.getline(buffer, line_buffer_size, '\n') || sdin.gcount()) {
        int count = sdin.gcount();
        if (sdin.fail()) {
          //cout << "Partial long line";
          sdin.clear(sdin.rdstate() & ~ios_base::failbit);
        } 
        else if (sdin.eof()) {
          //cout << "Partial final line";  // sdin.fail() is false
        } 
        else {
          //count--;  // Don’t include newline in count
          //cout << "Line " << ++line_number;
          ++line_number;
        }
        if (buffer[0] == '-'){
          measures++;
          continue;
        }
        
        int smallIndex = 0;
        int carryThrough = -1;

        playBuffer[line_number - 4 - measures] = 0;

        for (int i = 0; i <= 6; i++){

          smallIndex = 0;
          
          if(line_number - 1 > length)
            line_number = length - 1;
          
          for(int a = carryThrough + 1; a <= carryThrough + 4; a++){
            if(buffer[a] == ',')
              break;
            smallBuffer[smallIndex] = buffer[a]; 
            
            smallIndex++; 
            carryThrough = a;
          }
          
          //smallBuffer[4] = 0;
          Serial.println(smallBuffer); // *********************ASK JOE OR MACK WTH IS UP ***********************
          
          carryThrough++;

          if (i == 0){
            lengthOfNote[line_number - 4 - measures] = atoi(smallBuffer);
          }
          else {
            if(smallBuffer[0] >= 48 && smallBuffer[0] <= 57){ // 0 - 9 characters
              playBuffer[line_number - 4 - measures] |= atoi(smallBuffer) << (5*(i-1));
              //Serial.println(buffer);
            } 
            else
              playBuffer[line_number - 4 - measures] |= 25 << (5*(i-1));
          }

          //Serial.println(playBuffer[line_number - 4]);

          //playBuffer[line_number - 4][0] = atoi(smallBuffer);

          smallBuffer[0] = ' ';
          smallBuffer[1] = ' ';
          smallBuffer[2] = ' ';
          smallBuffer[3] = ' ';

        }

        for(int i = 0; i != 6; i++){
          //Serial.println((int)((float)line_number/(float)line_number*22.0));
          setLED(22-(int)((float)line_number/(float)length*22.0),i); 
          delayMicroseconds(100);
        }
      }
      playSong(playBuffer, lengthOfNote, tempo, length); 
    }
    break;
  }
}


