#include <BlueMega.h>
#include "SoftwareSerial.h"

#define RxDBt         51
#define TxDBt         50
#define BluetoothBaud 19200

uint8_t flag=1;

BlueMega blue = BlueMega(RxDBt,TxDBt, BluetoothBaud);

void stateChanged(int state){
     Serial.print("New state:");
     Serial.print(state);
     Serial.print('\n');
     if(state == blue.BT_CONNECTED){
       flag = 1;
     }
}

void setup(){
  Serial.begin(38400);
  Serial.print("Begin()");
  blue.begin();
  blue.configStateChange(stateChanged); 
  blue.startInquiry();
}

void loop(){
  blue.process();
  if(blue.state() == blue.BT_CONNECTED){
    char a;
    if((a = blue.ssp->read()) != -1){
      if(a == 'a'){
          blue.ssp->println("You are connected to Bluetooth Bee");
      }
      if(a == '+'){
         flag = 0;
      }
      if(flag){
        Serial.print(a);
      }
    }
  }
}
