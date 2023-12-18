#pragma once
#include "DisplayController.h"
#include "GameBlock.h"
#include "GameBall.h"
#include "GamePlayer.h"

class BlockGameManager 
{
private:
    DisplayController* displayController_{nullptr};
    Thread* gameThread_{nullptr};
    Thread* playerThread_{nullptr};
    Thread* playerLedsThread_{nullptr};
    GameBlock** gameBlocks_{nullptr};
    GameBall* gameBall_{nullptr};
    GamePlayer* gamePlayer_{nullptr};
    const int PLAYER_Y = 8;
    const int BLOCK_ARRAY_SIZE = 12;
    int amountOfGameBlocks_{12};
    int amountOfLives_{3};
    bool state_{false};

    /** Turn all leds in display off
    */
    void turnOffAllLeds();
    /** Reset states and positions of objects
    */
    void resetGame();
    /** Reset game ball velocity and position
    */
    void resetBall();
    /** Display all leds of objects
    */
    void displayAll();
    /** Merge and destroy threads
    */
    void destroyThreads();
    /** Thread method of showing realtime player position
    */
    void playerUpdateLeds();
    /** Calculate physics of ball and 
        it's interaction with other objects
    */
    void moveBall();
    
public:
    /** BlockGame constructor, creates all game objects
    */
    BlockGameManager();
    /** Get state of game
    *   @return playing state of game
    */
    bool getState();
    /** Get amount of active blocks
    *   @return amount of not yet destroyed blocks
    */
    int getAmountOfBlocks();
    /** Get current amount of player lives
    *   @return remaining player lives
    */
    int getAmountOfLives();
    /** Pass movement characters to player
    *   @param character to be passed
    */
    void userInput(char c);
    /** Create all game logic threads and start the game
    */
    void startGame();
    /** Change the state of game and thus stop all threads
    */
    void stopGame();
    /** Thread method for delaying movement of ball
    */
    void tic();
    /** BlockGameManager destructor, deletes all objects
    */
    ~BlockGameManager();
};