#pragma once
#include "BlockGameManager.h"

class MenuManager 
{
private:
    Thread* monitorThread_{nullptr};
    InterruptIn* userButton_{nullptr};
    PwmOut* comboLed_{nullptr};
    BlockGameManager* gameManager_{nullptr};
    int comboCounter_{0};
    bool exit_{false};

    /** Merge and destroy monitor thread
    */
    void destroyThread();
    /** Print menu to user
    */
    void printMenu();
    /** Increment comboCounter_ integer 
        and change brightness of Led
    */
    void addToCombo();
    /** Reset comboCounter_ to 0
    */
    void resetCombo();
    /** Start game main thread method
    */
    void startGame();
    /** Monitor thread method for game statistics and outcome
    */
    void monitorThreadMethod();
    /** Menu main thread method
    */
    void menuThreadMethod();
    /** Interrupt active game
    */
    void interruptMethod();
public:
    /** MenuManager constructor, creates game manager 
        and user interface logic 
    */
    MenuManager();
    /** MenuManager destructor
    */
    ~MenuManager();
};