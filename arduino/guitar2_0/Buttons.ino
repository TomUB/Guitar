int buttonpress(){
  int pressed = 0;
  int buttonNumber = 0;
  
  button[0].Update();
 
  if (button[0].clicks != 0)
  {
    buttonNumber = 1;//right
  }
  button[3].Update();
  
  if (button[3].clicks != 0)
  {
    buttonNumber = 2;//down
  }
  button[2].Update();
  
  if (button[2].clicks != 0)
  {
    buttonNumber = 3;//left
  }
  button[1].Update();
  
  if (button[1].clicks != 0)
  {
    buttonNumber = 4;//up
  }
  
  return buttonNumber;
}
