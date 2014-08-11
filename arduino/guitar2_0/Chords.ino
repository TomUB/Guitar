void displayChord(int chord, int key, int fingering) {
  if (chord == 0)
    majorChord(key,fingering);
  else if (chord == 1) 
    minorChord(key,fingering);
  else if (chord == 2)
    major7Chord(key,fingering);
  else if (chord == 3)
    minor7Chord(key,fingering);
  else if (chord == 4)
    dominant7Chord(key,fingering);
}

void majorChord(int key,int fingering) {
  switch (fingering) {
  case 0:
    setLED(0+key,0);  
    setLED(0+key,1); 
    setLED(1+key,2); 
    setLED(2+key,3); 
    setLED(2+key,4); 
    setLED(0+key,5); 
    break;
  case 1:
    if (key >= 5)
      key += -12;
    setLED(7+key,0);  
    setLED(9+key,1); 
    setLED(9+key,2); 
    setLED(9+key,3); 
    setLED(7+key,4); 
    break;
  }
}

void minorChord(int key,int fingering) {
  switch (fingering) {
  case 0:
    setLED(0+key,0);  
    setLED(0+key,1); 
    setLED(0+key,2); 
    setLED(2+key,3); 
    setLED(2+key,4); 
    setLED(0+key,5); 
    break;
  case 1:
    if (key >= 5)
      key += -12;
    setLED(7+key,0);  
    setLED(8+key,1); 
    setLED(9+key,2); 
    setLED(9+key,3); 
    setLED(7+key,4); 
    break;
  }
}

void major7Chord(int key,int fingering) {
  switch (fingering) {
  case 0:
    setLED(4+key,0);  
    setLED(4+key,1); 
    setLED(4+key,2); 
    setLED(2+key,3); 
//    setLED(2+key,4); 
//    setLED(0+key,5); 
    break;
  case 1:
    if (key >= 5)
      key += -12;
    setLED(7+key,0);  
    setLED(9+key,1); 
    setLED(8+key,2); 
    setLED(9+key,3); 
    setLED(7+key,4); 
    break;
  }
}

void minor7Chord(int key,int fingering) {
  switch (fingering) {
  case 0:
    setLED(0+key,0);  
    setLED(0+key,1); 
    setLED(0+key,2); 
    setLED(0+key,3); 
    setLED(2+key,4); 
    setLED(0+key,5); 
    break;
  case 1:
    if (key >= 5)
      key += -12;
    setLED(7+key,0);  
    setLED(8+key,1); 
    setLED(7+key,2); 
    setLED(9+key,3); 
    setLED(7+key,4); 
    break;
  }
}

void dominant7Chord(int key,int fingering) {
  switch (fingering) {
  case 0:
    setLED(0+key,0);  
    setLED(0+key,1); 
    setLED(1+key,2); 
    setLED(0+key,3); 
    setLED(2+key,4); 
    setLED(0+key,5); 
    break;
  case 1:
    if (key >= 5)
      key += -12;
    setLED(7+key,0);  
    setLED(9+key,1); 
    setLED(7+key,2); 
    setLED(9+key,3); 
    setLED(7+key,4); 
    break;
  }
}


