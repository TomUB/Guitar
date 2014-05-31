#include <SdFat.h>
#include <LiquidCrystal595.h>
//#include <ShiftLCD.h>

// SD chip select pin
const uint8_t chipSelect = SS;

// file system object
SdFat sd;

int dataPin = 3;
int latchPin = 2;
int clockPin = 1;

unsigned long data[6]; // display buffer

//unsigned int pixel[6][[22] = 0;
//double decay = decayAmount; 

IntervalTimer updateLED;

LiquidCrystal595 lcd(16, 15, 14);
//ShiftLCD lcd(2, 4, 3);

volatile long timestamp = micros();
long prevTimestamp = 0;

void setup(){
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, World!");
  
  Serial.begin(9600);
  
  if (!sd.begin(chipSelect, SPI_FULL_SPEED)) sd.initErrorHalt();
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  updateLED.begin(updateBuffer, 1000);
}

void loop(){
  int amountOfFiles = countPTBFiles();
  char files[amountOfFiles][13];
  getFileList(&(files[amountOfFiles][13]));
  
  readInFile("get_back.PTB");
  clearLEDs();
  delay(100000); 
}
