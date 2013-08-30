
#include <RfidMega.h>

RfidMega rfid = RfidMega(Serial1, 9800);

void setup(){
  rfid.begin();
  rfid.configWarnings(1,1,1);
  rfid.configReadLoop(newRfidCode, false);
  rfid.startReadLoop();
  Serial.begin(38400);
}

void newRfidCode(byte code[], int lenght){
  for(int i=0;i<lenght;i++){
     Serial.print(code[i], HEX);     
  }
  Serial.println("|");
} 

void loop(){
  rfid.process();
}


  
