#include <BlueMega.h>
#include "SoftwareSerial.h"

#define RxDBt         51
#define TxDBt         50
#define BluetoothBaud 19200

BlueMega blue = BlueMega(RxDBt,TxDBt, BluetoothBaud);

void stateChanged(int state){
     Serial.print("New state:");
     Serial.print(state);
     Serial.print('\n');
}

void setup(){
  blue.begin();
  blue.startInquiry();
  blue.configStateChange(stateChanged); 
}

void loop(){
}
