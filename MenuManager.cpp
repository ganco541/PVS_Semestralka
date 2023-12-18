#include "MenuManager.h"
#include <cstdio>

MenuManager::MenuManager()
{
    this->userButton_ = new InterruptIn(PC_13);
    
    this->comboLed_ = new PwmOut(PC_7);
    this->comboLed_->period_ms(1);
    *(this->comboLed_) = 0.0f;

    this->gameManager_ = new BlockGameManager();
    
    printf("-----------------------------\n\r");
    printf("Welcome to FRItari Breakpoint\n\r");
    
    this->userButton_->rise(callback(this, &MenuManager::resetCombo));
    this->menuThreadMethod();
}

void MenuManager::destroyThread()
{
    if (this->monitorThread_ != nullptr)
    {
        this->monitorThread_->join();
        delete this->monitorThread_;
        this->monitorThread_ = nullptr;
    }
}

void MenuManager::printMenu()
{
    printf("-----------------------------\n\r");
    printf("(1) Start Game\n\r");
    printf("(2) Quit\n\r");
    printf("-----------------------------\n\r");
}

void MenuManager::addToCombo()
{
    ++comboCounter_;
    float dimLevel = comboCounter_ * 0.083f;
    *(this->comboLed_) = dimLevel;
}

void MenuManager::resetCombo()
{
    comboCounter_ = 0;
    *(this->comboLed_) = 0.0f;
}

void MenuManager::startGame()
{
    this->gameManager_->startGame();

    this->destroyThread();
    this->monitorThread_ = new Thread();
    this->monitorThread_->start(callback(this, &MenuManager::monitorThreadMethod));

    char input;
    printf("Move platform with A/D or exit game(1):\n\r");
    while (this->gameManager_->getState())
    {
        input = getchar();
        if (!this->gameManager_->getState()) break;
        if (input == 'a' || input == 'd')
        {
            this->gameManager_->userInput(input);
        }
        else if (input == '1')
        {
            this->gameManager_->stopGame();
        }
        else {
            printf("Move platform with (A|D) or exit game(1):\n\r");
        }
    }
}

void MenuManager::monitorThreadMethod()
{
    if (this->comboCounter_ != 0) this->resetCombo();
    int oldLives = this->gameManager_->getAmountOfLives();
    int oldGameBlocks = this->gameManager_->getAmountOfBlocks();
    int newLives{0};
    int newGameBlocks{0};
    while (this->gameManager_->getState())
    {
        newLives = this->gameManager_->getAmountOfLives();
        newGameBlocks = this->gameManager_->getAmountOfBlocks();
        if (oldLives != newLives)
        {
            oldLives = newLives;
            printf("You lost a life! Lives left: %i\n\r", newLives);
            this->resetCombo();
        }
        if (oldGameBlocks != newGameBlocks)
        {
            oldGameBlocks = newGameBlocks;
            this->addToCombo();
        }
    }
    if (this->gameManager_->getAmountOfLives() == 0) {
        printf("YOU LOST!\n\r");
    } 
    else if (this->gameManager_->getAmountOfBlocks() == 0)
    {
        printf("YOU WON!\n\r");
    } 
    else 
    {
        printf("The game was interrupted!\n\r");    
    }
    this->resetCombo();
}

void MenuManager::menuThreadMethod()
{
    char input;
    while(!this->exit_)
    {
        this->printMenu();
        input = getchar();
        switch (input) {
            case '1':
                this->startGame();
                break;
            case '2':
                this->exit_ = true;
                printf("Goodbye!\n\r");
        }
    }
}

void MenuManager::interruptMethod()
{
    if (this->gameManager_->getState()) this->gameManager_->stopGame();
}

MenuManager::~MenuManager()
{
    if (this->gameManager_->getState()) this->gameManager_->stopGame();
    this->destroyThread();
    delete this->userButton_;
    delete this->comboLed_;
    delete this->gameManager_;
}