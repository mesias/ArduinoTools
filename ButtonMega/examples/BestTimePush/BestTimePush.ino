#include <ButtonMega.h>

uint8_t pins[] = {
  34, 35, 36, 37};
ButtonMega bm = ButtonMega(pins, 4);
long btime[4] = {
  0,0,0,0,};
unsigned int best = 999;

void setup(){
  Serial.begin(38400);
  bm.configButtonPushed(buttonPushed);
  bm.configStateChanged(stateChanged);
  bm.begin();
}

void loop(){
  bm.process();
  delay(10);
} 

void stateChanged(uint8_t button, uint8_t state){
  Serial.print("Change: ");
  Serial.print(button);
  Serial.print(" > ");
  Serial.print(state);
  if(state == 1 && btime[button] == 0){
    btime[button] = millis();
  } 
  else {
    long ttime = millis() - btime[button];
    btime[button] = 0;
    Serial.print(" t:");
    Serial.print(ttime);
    if(ttime < best){
      best = ttime;
    }
    Serial.print(" B:");
    Serial.print(best);

  } 
  Serial.println();
}

void buttonPushed(uint8_t button){
  Serial.print("Pushed: ");
  Serial.println(button);
}


