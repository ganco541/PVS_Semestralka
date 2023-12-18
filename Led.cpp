#include "Led.h"

Led::Led(DigitalOut* row, DigitalOut* coloumn) 
{
    this->row_ = row;
    this->coloumn_ = coloumn;
    this->row_->write(0);
    this->coloumn_->write(0);
    this->state_ = false;
}

void Led::turnOn()
{
    this->coloumn_->write(1);
    this->row_->write(0);
}

void Led::turnOff()
{
    this->coloumn_->write(0);
    this->row_->write(1);
}

void Led::setState(bool state)
{
    this->state_ = state;
}

bool Led::getState()
{
    return this->state_;    
}

Led::~Led()
{
    
}