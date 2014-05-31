#include <SdFat.h>            //Library for SD card
#include <LiquidCrystal595.h> //Library for using a 595 shift register to control LCD
//#include <ShiftLCD.h>

// SD chip select pin
const uint8_t chipSelect = SS; // Chip Select Pin (used by SDFAT). SS is defined in library to be 13

// file system object
SdFat sd;

int dataPin = 3;  // Shift register pins for the LED output
int latchPin = 2;
int clockPin = 1;

unsigned long data[6]; // display buffer

//unsigned int pixel[6][[22] = 0;
//double decay = decayAmount; 

IntervalTimer updateLED; // The object which is used to setup a timer for the consistent flashing of the LEDs

LiquidCrystal595 lcd(16, 15, 14); // Display setup
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
  
  
  
  readInFile("get_back.PTB");
  clearLEDs();
  delay(100000); 
}
