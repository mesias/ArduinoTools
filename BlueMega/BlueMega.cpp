#include "Arduino.h"
#include "BlueMega.h"
#include "SoftwareSerial.h"

#define toDigit(c) (c-'0')

static const char BTSTATE_TAG[] = "+BTSTATE:";
static const int btstate_size = 9;
static int btstate_count;

BlueMega::BlueMega(byte rx, byte tx, int baud)
{
    _rx = rx;
    _tx = tx;
    _baud = baud;
}

int BlueMega::state(){
    return _state;
}

void BlueMega::sendBlueToothCommand(char command[])
{
  char a;
  ssp->print(command);
  delay(1000);
  while(ssp->available()) {
    a = ssp->read();
    watchBtstate(a);
  }
}

void BlueMega::watchBtstate(char c){
  if(btstate_count == btstate_size){
    _state = toDigit(c);

    if(_stateChange != NULL){
        _stateChange(_state);
    }
    btstate_count = 0;
  } else if(BTSTATE_TAG[btstate_count] == c){
    btstate_count += 1;
  } else if (btstate_count > 0){
    btstate_count = 0;
  }
}

void BlueMega::clearInputSerial(){
  char a;
  while( (a = ssp->read()) != -1){
  }
}

void BlueMega::configStateChange(void (*stateChange)(int)){
    _stateChange = stateChange;
}

void BlueMega::startInquiry(){
  sendBlueToothCommand("\r\n+INQ=1\r\n");
  delay(2000); // This delay is required.
}

void BlueMega::begin(){
  pinMode(_rx, INPUT);
  pinMode(_tx, OUTPUT);
  ssp = new SoftwareSerial(_rx, _tx);
  // 9600, 19200,38400,57600,115200,230400,460800
  ssp->begin(_baud); //Set BluetoothBee BaudRate to default baud rate 38400
}


void BlueMega::process(){
  char a;
  if(_state != BT_CONNECTED && (a = ssp->read()) != -1){
      watchBtstate(a);
  } else if(_state == BT_CONNECTED && (a = ssp->peek()) != -1){
      watchBtstate(a);
  }
  
}

