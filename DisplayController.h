#pragma once
#include "Led.h"

class DisplayController 
{
private:
    Led** ledArray_{nullptr};
    DigitalOut** y_{nullptr};
    DigitalOut** x_{nullptr};
    Thread* refreshThread_{nullptr};

    bool startRefresh_{false};
    const int LED_ARRAY_SIZE = 64;
    const int X_SIZE = 8;
    const int Y_SIZE = 8;

    /** Resets row and coloumn pins, so all leds are turned off
    */
    void resetDisplay();
    /** Calculate index of Led in ledArray_ by coordinates
        @param x coloumn coordinate
        @param y row coordinate
        @return index of perticular Led in ledArray_
    */
    int findIndexOfLed(int x, int y);
    /** Refresh method for refreshThread_, 
    finds Leds that are active and actively shows them
    */
    void refreshDisplay();
public:
    /** DisplayControler constructor, 
    automatically creates rows, coloumns and each Led, etc...
    */
    DisplayController();
    /** Turn on led with coordinates
        @param x coloumn coordinate
        @param y row coordinate
    */
    void turnOnLed(int x, int y);
    /** Turn off led with coordinates
        @param x coloumn coordinate
        @param y row coordinate
    */
    void turnOffLed(int x, int y);
    /** Start display and start refreshing
    */
    void startDisplay();
    /** Stop display and stop refreshing
    */
    void stopDisplay();
    /** DisplayControler destructor, 
    automatically deletes rows, coloumns and each Led, etc...
    */
    ~DisplayController(); 
};