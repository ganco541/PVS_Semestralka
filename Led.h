#pragma once
#include "mbed.h"

class Led 
{
    private:
        DigitalOut* row_;
        DigitalOut* coloumn_;
        bool state_;
    
    public:
        /** LED constructor
        *
        *   @param row reference to DigitalOut of Led row
        *   @param coloumn reference to DigitalOut of Led coloumn
        */
        Led(DigitalOut* row, DigitalOut* coloumn);
        /** Turn on the diode (row - LOW, coloumn - HIGH)
        */
        void turnOn();
        /** Turn off the diode (row - HIGH, coloumn - Low)
        */
        void turnOff();
        /** Set state of LED
        *   @param state which state to turn to
        */
        void setState(bool state);
        /** Get state of LED
        *   @return state of LED
        */
        bool getState();
        /** LED destructor
        */
        ~Led();
};
