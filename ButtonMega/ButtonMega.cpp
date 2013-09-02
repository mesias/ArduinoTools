#include "Arduino.h"
#include "ButtonMega.h"

ButtonMega::ButtonMega(uint8_t* button_pins, uint8_t size){
    size_t uint8_size = sizeof(uint8_t);
    bsize = size;
    _button_pins = (uint8_t*) malloc(bsize*uint8_size);
    _buttonState = (uint8_t*) malloc(bsize*uint8_size);
    for(int i=0;i<bsize;i++){
        _button_pins[i] = button_pins[i];
        _buttonState[i] = 0;
    }
    
}

void ButtonMega::begin(){
    for(int i=0;i<bsize;i++){
        pinMode(_button_pins[i], INPUT);
    }
}

void ButtonMega::configStateChanged(void (*stateChanged)(uint8_t, uint8_t)){
    _stateChanged = stateChanged;
}

void ButtonMega::configButtonPushed(void (*buttonPushed)(uint8_t)){
    _buttonPushed = buttonPushed;
}

void ButtonMega::process(){  
  uint8_t reading;
  uint8_t i;
  for(i=0;i<bsize;i++){
    reading = digitalRead(_button_pins[i]);
    if (reading != _buttonState[i]) {
      // Button changed
      if(_stateChanged != NULL){
        _stateChanged(i, reading);
      }
    }
    if(reading == HIGH && _buttonState[i] == LOW){
      if(_buttonPushed!=NULL){
        _buttonPushed(i);
      }
      lastButtonPushed = _button_pins[i];
    }
    _buttonState[i] = reading;
  }
}
uint8_t* ButtonMega::buttonStates(){
    return _buttonState;
}

