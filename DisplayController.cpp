#include "DisplayController.h"

DisplayController::DisplayController()
{
    this->ledArray_ = new Led*[LED_ARRAY_SIZE];
    
    for (int i = 0; i < LED_ARRAY_SIZE; ++i)
    {
        this->ledArray_[i] = nullptr;
    }

    this->y_ = new DigitalOut*[8] {
        new DigitalOut(PF_12), 
        new DigitalOut(PD_15), 
        new DigitalOut(PD_14), 
        new DigitalOut(PA_7),
        new DigitalOut(PA_6),
        new DigitalOut(PA_5),
        new DigitalOut(PB_9),
        new DigitalOut(PB_8),
    };

    this->x_ = new DigitalOut*[8] {
        new DigitalOut(PG_8), 
        new DigitalOut(PG_7), 
        new DigitalOut(PF_15), 
        new DigitalOut(PE_13),
        new DigitalOut(PF_14),
        new DigitalOut(PE_11),
        new DigitalOut(PE_9),
        new DigitalOut(PF_13),
    };

    this->ledArray_ = new Led*[LED_ARRAY_SIZE];
    int i = 0;
    for (int y = 0; y < Y_SIZE; ++y)
    {
        for (int x = 0; x < X_SIZE; ++x)
        {
           this->ledArray_[i] = new Led(this->y_[y], this->x_[x]);
           ++i;
        }
    }

}

void DisplayController::resetDisplay()
{
    for (int x = 0; x < X_SIZE; ++x) 
    {
        if (this->x_[x]->read() == 1) this->x_[x]->write(0);
    }
    for (int y = 0; y < Y_SIZE; ++y) 
    {
        if (this->y_[y]->read() == 0) this->y_[y]->write(1);
    }
}

void DisplayController::refreshDisplay()
{
    resetDisplay();
    while (this->startRefresh_)
    {
        for (int i = 0; i < LED_ARRAY_SIZE; ++i)
        {
            Led* led = this->ledArray_[i];
            if (led->getState())
            {
                led->turnOn();
                led->turnOff();
            }
        }
    }
}

int DisplayController::findIndexOfLed(int x, int y)
{
    --x;
    --y;
    return y*8 + x;
}

void DisplayController::startDisplay()
{
    if (this->startRefresh_) this->stopDisplay();
    this->startRefresh_ = true;
    this->refreshThread_ = new Thread();
    this->refreshThread_->start(callback(this, &DisplayController::refreshDisplay));
}

void DisplayController::stopDisplay()
{
    this->startRefresh_ = false;
    if (this->refreshThread_ != nullptr)
    {
        this->refreshThread_->join();
        delete this->refreshThread_;
        this->refreshThread_ = nullptr;
    }
}

void DisplayController::turnOnLed(int x, int y)
{
    if ((x <= 8 && x > 0) && (y <= 8 && y > 0))
    {
        this->ledArray_[this->findIndexOfLed(x, y)]->setState(true);
    }
}

void DisplayController::turnOffLed(int x, int y)
{
    if ((x <= 8 && x > 0) && (y <= 8 && y > 0))
    {
        this->ledArray_[this->findIndexOfLed(x, y)]->setState(false);
    }
}

DisplayController::~DisplayController() {
    this->stopDisplay();
    if (this->ledArray_ != nullptr)
    {
        for (int i = 0; i < LED_ARRAY_SIZE; ++i) {
            if (this->ledArray_[i] != nullptr) delete this->ledArray_[i];
        }
        delete[] this->ledArray_;
        this->ledArray_ = nullptr;
    }

    if (this->y_ != nullptr)
    {
        for (int i = 0; i < Y_SIZE; ++i) {
            if (this->y_[i] != nullptr) delete this->y_[i];
        }
        delete[] this->y_;
        this->y_ = nullptr;
    }
    
    if (this->x_ != nullptr)
    {
        for (int i = 0; i < X_SIZE; ++i) {
            if (this->x_[i] != nullptr) delete this->x_[i];
        }
        delete[] this->x_;
        this->x_ = nullptr;
    }


}