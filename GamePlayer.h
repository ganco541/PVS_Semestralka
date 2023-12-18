#pragma once
#include <Mutex.h>

class GamePlayer 
{
private:
    int x1_;
    int x2_;
    int y_;
    
    rtos::Mutex bufferMutex_;
    char* movementBuffer_{nullptr};
    const int M_BUFFER_LENGTH = 32;
    const int BOUNDRY_LEFT = 1;
    const int BOUNDRY_RIGHT = 8;
    int sidewaysMovement_{0};
    int leftMovementIndex_{0};
    int rightMovementIndex_{0};
    bool playing_{false};

    /** Attempt to move player
    *   @param i move x1 coordinate by i
    *   @return movement is possible 
    */
    bool move(int i);
    /** Set velocity
    *   @param i set sidewaysMovement_ to i 
    */
    void setSidewaysMovement(int i);
public:
    /** GamePlayer constructor, creates movement buffer 
        and fills it with 'x's
    *   @param x1 left coloumn coordinate of player
    *   @param y row coordinate of player
    */
    GamePlayer(int x1, int y);
    /** Get position of player by changing references
    *   @param x1 reference to be changed to x1_ coordinate
    *   @param x2 reference to be changed to x2_ coordinate
    */
    void getPosition(int& x1, int& x2);
    /** Set position of player
    *   @param x1 left coloumn coordinate of player
    *   @param y row coordinate of player
    */
    void setPosition(int x1, int y);
    /** Set playing_ state
    *   @param playing state which playing_ is changed to
    */
    void setPlaying(bool playing);
    /** Change all characters in movementBuffer_ to 'x'
    */
    void clearMovementBuffer();
    /** Reset sidewaysMovement_ to 0
    */
    void resetSidewaysMovement();
    /** Get sideways movement
    *   @return sidewaysMovement_
    */
    const int& getSidewaysMovement();
    /** Add movement to movementBuffer_
    *   @param mov character to be added
    */
    void addMovement(char mov);
    /** Method for player thread to calculate movement 
        buffer and make changes to position of player
    */
    void playerThreadMethod();
    /** GamePlayer destructor, destroys movement buffer
    */
    ~GamePlayer();
};