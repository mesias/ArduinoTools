#include "Arduino.h"
#include "RfidMega.h"
//#include "SoftwareSerial.h"

#define IDLE    0
#define READING 2

RfidMega::RfidMega(HardwareSerial& serialport, int baud){
    hsp = &serialport;
    _state = IDLE;
    _baud = baud;
}

void RfidMega::begin(){
    hsp->begin(_baud);
}

void RfidMega::configReadLoop(void (*fn)(byte [], int),bool allowRepeat = true){
    if(_state == IDLE){
        _allowRepeat = allowRepeat;
        _fn = fn;
        if(!_allowRepeat && matrix == NULL){
            genMatrix();
        }
    }
}
void RfidMega::stopReadLoop(){
    _state = IDLE;
}

void RfidMega::configWarnings(bool buzzer, bool led1, bool led2){
    config[0] = buzzer;
    config[1] = led1;
    config[2] = led2;
}

void RfidMega::configRepeatCtrl(int reads_size, int value_size){
    arrayRows = reads_size;
    arrayCols = value_size;
    if(matrix != NULL){
        freeMatrix();
        genMatrix();
    } else {
        genMatrix();
    }
}

void RfidMega::startReadLoop(){
    // Limpar matriz de valores repetidos (se houver valor)
    if(!_allowRepeat && matrix != NULL){
        initMatrix();
    }
    _state = READING;
}

void RfidMega::clearInputSerial(){
  char a;
  while( (a = hsp->read()) != -1){
  }
}

void RfidMega::genMatrix(){
    matrix = (byte**) malloc(arrayRows*sizeof(byte*));
    for (int i = 0; i < arrayRows; i++)
        matrix[i] = (byte*) malloc(arrayCols*sizeof(byte));
}

void RfidMega::freeMatrix(){
    for (int i = 0; i < arrayRows; i++){
        free(matrix[i]);
    }
    free(matrix);
    msize=-1;
}

int RfidMega::insert(byte vector[]){
  if(msize == arrayRows){
    // No free position to hold new value
    return 2;
  }
  
  for(int k=0; k < msize; k++){
    int count=0;
    for(int l=0; l<arrayCols; l++){
      if(matrix[k][l]==vector[l]){
        count++;
        if(count == arrayCols){
           // Value hit
           return 1;
        }
      }
    }
  }
  // Insere no contador de tags encontradas
  for(int i=0;i<arrayCols;i++){
    matrix[msize][i] = vector[i];
  }
  msize++;
  return 0;
}

void RfidMega::initMatrix(){
  int i,j;
  for ( i = 0; i < arrayRows ; i++){
    for( j = 0 ; j < arrayCols ; j++){
      matrix[i][j]=0;
    }
  }
  msize=0;
}

void RfidMega::runMsg(byte array[], bool clearInput = true){
    hsp->write(array,8);
    delay(30);
    if(clearInput){
        clearInputSerial();
    }
}

void RfidMega::process(){
    if(_state == READING){
        if(config[1]){
            runMsg(led1);
        }
        runMsg(query, false);
        
        if(hsp->available()){
            byte c, sz, station_id, cstatus, collision;

            if(hsp->read() == 0xAA){      
                if((station_id = hsp->read()) == 0x00){
                    sz = hsp->read();
                    cstatus = hsp->read();   // STATUS
                    collision = hsp->read();
                    byte bfrase[sz-2];
                    byte sum =  station_id ^ sz ^ cstatus ^ collision;
                    for(int i = 0; i < (sz - 2); i++){
                        bfrase[i] = hsp->read();
                        sum = sum ^ bfrase[i];
                    }
                    byte bcc = hsp->read();
                    c = hsp->read();
                    if(c== 0xBB){
                        if(bcc == sum && cstatus == 0){
                            if(collision && config[2]){
                                runMsg(led2);
                            }
                            if(_allowRepeat){
                                if(config[0]){
                                    runMsg(buzzer);
                                }
                                _fn(bfrase, sz - 2);  // Callback
                            } else {
                                int result = insert(bfrase);
                                if(result==0){
                                    _fn(bfrase, sz - 2);
                                    if(config[0]){
                                        runMsg(buzzer);
                                    }
                                } else if (result == 1){
                                    runMsg(led22);
                                } else if (result == 2){
                                    // Array size problems
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

