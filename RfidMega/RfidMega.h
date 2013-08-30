#ifndef RfidMega_h
#define RfidMega_h

/*

*/

#include "Arduino.h"
//#include "SoftwareSerial.h"

class RfidMega
{
    public:
        RfidMega(HardwareSerial& serialport, int baud);
        //RfidMega(SoftSerial& serialport, int baud);
        void configWarnings(bool buzzer, bool led1, bool led2);
        void configRepeatCtrl(int reads_size, int value_size);
        void configReadLoop(void (*fn)(byte [], int),bool allowRepeat);
        void startReadLoop();
        void stopReadLoop();
        void process();
        void begin();
    private:
        bool config[3] = {0,0,0}; // on/off: buzzer, led1 (qry), led2 (hit)
        byte query[8] =  { 0xAA, 0x00, 0x03, 0x25, 0x26, 0x00, 0x00, 0xBB};
        byte led1[8] =   { 0xAA, 0x00, 0x03, 0x87, 0x05, 0x01, 0x80, 0xBB};
        byte led2[8] =   { 0xAA, 0x00, 0x03, 0x88, 0x0A, 0x01, 0x80, 0xBB};
        byte led22[8] =  { 0xAA, 0x00, 0x03, 0x88, 0x05, 0x02, 0x8c, 0xBB};
        byte buzzer[8] = { 0xAA, 0x00, 0x03, 0x89, 0x0A, 0x01, 0x81, 0xBB};
        HardwareSerial* hsp;
        byte** matrix; 
        int arrayRows=20;
        int arrayCols=4;
        int msize=-1;
        byte _state;
        void clearInputSerial();
        int insert(byte vector[]);
        void initMatrix();
        void genMatrix();
        void runMsg(byte array[], bool clearInput);
        void freeMatrix();
        unsigned int _baud;
        bool _allowRepeat;
        void (*_fn)(byte [], int);
        //SoftSerial*     s_serialport;
};



#endif

