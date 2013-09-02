#ifndef BlueMega_h
#define BlueMega_h

#include "Arduino.h"

class ButtonMega
{
    public:
        ButtonMega(uint8_t* buttons_pins,  uint8_t size);        
        void configStateChanged(void (*stateChange)(uint8_t, uint8_t));
        void configButtonPushed(void (*buttonPushed)(uint8_t));
        void process();
        void begin();
        uint8_t* buttonStates();
        uint8_t lastButtonPushed;
        uint8_t bsize;
    private:
        uint8_t* _button_pins;
        uint8_t* _buttonState;
        void (*_stateChanged)(uint8_t, uint8_t);
        void (*_buttonPushed)(uint8_t);
};

#endif


