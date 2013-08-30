#ifndef BlueMega_h
#define BlueMega_h

/*

*/

#include "Arduino.h"
#include "SoftwareSerial.h"

class BlueMega
{
    public:
        BlueMega(byte rx, byte tx, int baud);
        void begin();
        int state();
        void configStateChange(void (*stateChange)(int));
        void startInquiry();
        void process();
        byte read();
        byte write();
        SoftwareSerial* ssp;
        static const uint8_t BT_INITIAL =  0;
        static const uint8_t BT_READY      = 1;
        static const uint8_t BT_INQUIRING  = 2;
        static const uint8_t BT_CONNECTING = 3;
        static const uint8_t BT_CONNECTED = 4;
    private:
        void sendBlueToothCommand(char command[]);
        void watchBtstate(char c);
        void clearInputSerial();
        uint8_t checkOK();
        int _state = -1;
        byte _rx;
        byte _tx;
        int _baud;
        void (*_stateChange)(int);
};



#endif

