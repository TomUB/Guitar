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

int menu;
int mode=0;

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
    digitalWrite(20, HIGH);//disable led output so leds don't flash when first powered on

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
    digitalWrite(20,LOW);//enable leds
    //  minorPentatonic(2);
    lcd.setCursor(0,0);
    lcd.print("Choose Mode:");
    lcd.setCursor(0,1);
    lcd.print("Songs");
  }



void loop(){
  while (1) {
    const int modes = 4;
    char* secondLine[modes] = {
      "Songs","Chords","Scales","Animations"};
    int buttonPressed = buttonpress(); 
    if (buttonPressed == 1) {
      mode++;
      if (mode>modes-1)
        mode=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choose Mode:");
      lcd.setCursor(0,1);
      lcd.print(secondLine[mode]);
    }
    if (buttonPressed == 3) {
      mode--;
      if (mode < 0)
        mode = modes-1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Choose Mode:");
      lcd.setCursor(0,1);
      lcd.print(secondLine[mode]);
    }
    if (buttonPressed == 4) {
      lcd.clear();
      menu = mode;
      break;
    }
  }
  while (menu == 0) {
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

      if (buttonPressed == 3){
        startPosition = 0;
        initialPreScrollDelay = 2000; // reset it to a normal delay during scroll
        fileNumber++;
        if (fileNumber == amountOfFiles)
          fileNumber = 0;
      }

      if (buttonPressed == 4){
        readInFile(fileName[fileNumber]);
        clearLEDs();
      }
      
      if (buttonPressed == 2){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Mode:");
        lcd.setCursor(0,1);
        lcd.print("Songs");
        menu = 3;
        break;
      }

    }
    
  }

  while (menu == 1) {//chords
    static int key=0;
    static int chord = 0;
    static boolean changeKey = 0;
    if (!changeKey) {
    lcd.setCursor(0,0);
    lcd.print("Select a Chord:");
    lcd.setCursor(0,1);
    lcd.print("Major");
    }
    while(1) {
      if (changeKey)
        break;
      const int num = 5;//number of chords
      char* secondLine[num] = {
        "Major","Minor","Major Seven","Minor Seven","Dominant Seven"};
      int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        chord++;
        if (chord > num-1)
          chord=0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Chord:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[chord]);
      }
      if (buttonPressed == 3) {
        chord--;
        if (chord < 0)
          chord = num - 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Chord:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[chord]);
      }
      if (buttonPressed == 4) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Key:");
        lcd.setCursor(0,1);
        lcd.print("E");
        changeKey = 1;
        break;
      }
      if (buttonPressed == 2) {
        changeKey = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Mode:");
        lcd.setCursor(0,1);
        lcd.print("Chords");
        menu = 3;
        break;
      }
      
    }
    while (changeKey == 1) {
      const int num = 12;
      char* secondLine[num] = {"E","F","F#","G","Ab","A","Bb","B","C","C#","D","Eb"};
    int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        key++;
        if (key > num-1)
          key=0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Key:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[key]);
      }
      if (buttonPressed == 3) {
        key--;
        if (key < 0)
          key = num-1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Key:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[key]);
      }
      
      if (buttonPressed == 4) {
        lcd.clear();
        displayChord(chord,key,0);
        lcd.setCursor(0,0);
        lcd.print("Choose Version:");
        lcd.setCursor(0,1);
        lcd.print("0");
        break;
      }
      if (buttonPressed == 2) {
        clearLEDs();
        lcd.clear();
        changeKey = 0;
        break;
      }
    }
    
    while(changeKey == 1) {
      const int num = 2;
      static int inversion = 0;
      char* secondLine[num] = {"0","1"};
      int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        inversion++;
        if (inversion > num-1)
          inversion=0;
        clearLEDs();
        displayChord(chord,key,inversion);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Version:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[inversion]);
      }
      if (buttonPressed == 3) {
        inversion--;
        if (inversion < 0)
          inversion = num-1;
        clearLEDs();
        displayChord(chord,key,inversion);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Version:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[inversion]);
      }
      if (buttonPressed == 2) {
        clearLEDs();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Key:");
        lcd.setCursor(0,1);
        lcd.print("E");
        key = 0;
        changeKey = 1;
        break;
      }
    }

  }

  while (menu == 2) {//scales
    int scale = 0;
    boolean changeKey = 0;
    lcd.setCursor(0,0);
    lcd.print("Select a Scale:");
    lcd.setCursor(0,1);
    lcd.print("Major");
    while(1) {
      const int num = 4;
      char* secondLine[num] = {
        "Major","Minor","Major Pentatonic","Minor Pentatonic"};
      int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        scale++;
        if (scale > num-1)
          scale=0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Scale:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[scale]);
      }
      if (buttonPressed == 3) {
        scale--;
        if (scale < 0)
          scale = num - 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Select a Scale:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[scale]);
      }
      if (buttonPressed == 4) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Key:");
        lcd.setCursor(0,1);
        lcd.print("E");
        displayScale(scale,0);
        changeKey = 1;
        break;
      }
      if (buttonPressed == 2) {
        changeKey = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Mode:");
        lcd.setCursor(0,1);
        lcd.print("Scales");
        menu++;
        break;
      }
      
    }
    while (changeKey) {
      const int num = 12;
      char* secondLine[num] = {"E","F","F#","G","Ab","A","Bb","B","C","C#","D","Eb"};
    static int key=0;
    int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        key++;
        if (key > num-1)
          key=0;
        clearLEDs();
        displayScale(scale,key);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Key:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[key]);
      }
      if (buttonPressed == 3) {
        key--;
        if (key < 0)
          key = num-1;
        clearLEDs();
        displayScale(scale,key);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Key:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[key]);
      }
      if (buttonPressed == 2) {
        clearLEDs();
        lcd.clear();
        mode = 4;
        break;
      }
    }
  }
  while (menu == 3) {//animations
    int animation = 0;
    boolean play = 0;
    lcd.setCursor(0,0);
    lcd.print("Choose Animation:");
    lcd.setCursor(0,1);
    lcd.print("Rain");
    while(1) {
      const int num = 3;
      char* secondLine[num] = {
        "Rain","Fill","Toggle"};
      int buttonPressed = buttonpress(); 
      if (buttonPressed == 1) {
        animation++;
        if (animation > num-1)
          animation=0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Animation:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[animation]);
      }
      if (buttonPressed == 3) {
        animation--;
        if (animation < 0)
          animation= num - 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Animation:");
        lcd.setCursor(0,1);
        lcd.print(secondLine[animation]);
      }
      if (buttonPressed == 4) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(animation);
        lcd.setCursor(0,1);
        lcd.print("");
        while (buttonpress() != 2)
          playAnimation(animation);
        clearLEDs();
        break;
      }
      if (buttonPressed == 2) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Choose Mode:");
        lcd.setCursor(0,1);
        lcd.print("Animations");
        menu++;
        break;
      }
    }
  }
}

