#include <SdFat.h>
#include <LiquidCrystal595.h>
#include <ClickButton.h>

// SD chip select pin
const uint8_t chipSelect = SS;

// file system object
SdFat sd;

SdFile file;

int dataPin = 4;
int latchPin = 2;
int clockPin = 3;

unsigned long data[6]; // display buffer
unsigned char stringBuffer = 0;

//unsigned int pixel[6][[22] = 0;
//double decay = decayAmount; 

IntervalTimer updateLED;

LiquidCrystal595 lcd(9, 7, 8);
//ShiftLCD lcd(2, 4, 3);

volatile long timestamp = micros();
long prevTimestamp = 0;

const int buttons = 4;

const int buttonPin1 = 15;
const int buttonPin2 = 16;
const int buttonPin3 = 17;
const int buttonPin4 = 18;
// Instantiate ClickButton objects in an array

ClickButton button[4] = {
  ClickButton (buttonPin1, LOW, CLICKBTN_PULLUP),
  ClickButton (buttonPin2, LOW, CLICKBTN_PULLUP),
  ClickButton (buttonPin3, LOW, CLICKBTN_PULLUP),
  ClickButton (buttonPin4, LOW, CLICKBTN_PULLUP)
};

void setup(){
  
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
  Serial.begin(9600);
  
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  /*pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);*/
  
  for (int i=0; i<buttons; i++)
  {

    // Setup button timers (all in milliseconds / ms)
    // (These are default if not set, but changeable for convenience)
    button[i].debounceTime   = 20;   // Debounce timer in ms
    button[i].multiclickTime = 1;  // Time limit for multi clicks
    button[i].longClickTime  = 10000; // Time until long clicks register
  }

  updateLED.begin(updateBuffer, 1000);
}



void loop(){
  sd.chdir("/",true);
  
  int amountOfFiles = countPTBFiles();
  
  char fileName[amountOfFiles][13];
  
  char shortFileName[12];
  char longFileName[128];

  // open next file in root.  The volume working directory, vwd, is root
  
  sd.chdir("/",true);
  
  int fileCount = 0;
  
  while (file.openNext(sd.vwd(), O_READ)) {
    file.getFilename(shortFileName);
    //file.read(fullFileName, 16);
    int i = 0; // location of '.' in file name string - 1
    while(shortFileName[i] != '.' && i <= 8)
      i++; 
    if(shortFileName[i] == '.' && shortFileName[i + 1] == 'P' && shortFileName[i + 2] == 'T' && shortFileName[i + 3] == 'B'){
      for(int c = 0; c < 13; c++){ // get file short name
        fileName[fileCount][c] = shortFileName[c];
      }
      fileCount++;
    } 
    file.close();
  }
  //return fileCount;
  
  int fileNumber = 0;
  
  lcd.setCursor(0,0);
  lcd.print("Select a file:");
  
  int startPosition = 0;
  int initialPreScrollDelay = 2000;
  int buttonPressed = 0;
  
  while(1){
    lcd.setCursor(0,1);
    
    file.open(sd.vwd(), fileName[fileNumber], O_READ);
    file.read(longFileName, 128);
    file.close();
    
    int length = 0;
    unsigned char clearRest = 0;
    
    for(int i = 0; i < 128; i++){
      if (longFileName[i] == 10 && !clearRest){
         length = i;
         clearRest = 1;
       }
       if (clearRest)
         longFileName[i] = ' '; 
    }
    
    for(int i = startPosition; i < startPosition + 16; i++){
       lcd.print(longFileName[i]);
    }
    //lcd.print("    ");
    //lcd.print("Select a file:");
    
    for (int i = 0; i < initialPreScrollDelay / 10; i++)
    {
      buttonPressed = buttonpress(); // delay for 500 milliseconds while still being responsive
      if (buttonPressed != 0)
        break;
      delay(10);
    }
    
    startPosition++;
    initialPreScrollDelay = 500; // reset it to a normal delay during scroll
    
    if (startPosition + 15 >= length && buttonPressed == 0)
    {
      startPosition = 0;
      for (int i = 0; i < 2000 / 10; i++)
      {
        buttonPressed = buttonpress(); // delay for 500 milliseconds while still being responsive
        if (buttonPressed != 0)
          break;
        delay(10);
      }
      initialPreScrollDelay = 2000; // reset it to a normal delay during scroll
    }
    //int buttonP = buttonpress();
    
    if (buttonPressed == 1){
      startPosition = 0;
      initialPreScrollDelay = 2000; // reset it to a normal delay during scroll
      fileNumber--;
      if (fileNumber == -1)
        fileNumber = fileCount-1;
    }
    
    if (buttonPressed == 2){
      startPosition = 0;
      initialPreScrollDelay = 2000; // reset it to a normal delay during scroll
      fileNumber++;
      if (fileNumber == amountOfFiles)
        fileNumber = 0;
    }
    
    if (buttonPressed == 4){
      break;
    }
      
  }
  Serial.println(fileName[fileNumber]);
  readInFile(fileName[fileNumber]);
  clearLEDs();
}
