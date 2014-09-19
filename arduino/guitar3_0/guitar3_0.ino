#include <SdFat.h>
#include <LiquidCrystal595.h>
#include <ClickButton.h>

const uint8_t chipSelect = SS;

// file system object
SdFat sd;

SdFile file;

int dataPin = 4;
int latchPin = 2;
int clockPin = 3;

unsigned long data[6]; // display buffer

unsigned char stringBuffer = 0;

IntervalTimer updateLED;

LiquidCrystal595 lcd(9, 7, 8);

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

    pinMode(20, OUTPUT); //shift registor enable Pin
    digitalWrite(20, HIGH);//disable led output so leds don't flash when first powered on

    lcd.begin(16, 2);
    // Print a message to the LCD.

    Serial.begin(38400);

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
}



void loop(){
  
}


