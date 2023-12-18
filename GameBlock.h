#pragma once

class GameBlock
{
private:
    int x1_;
    int x2_;
    int y_;
    bool state_;
public:
    /** GameBlock constructor
    *   @param x - left coloumn coordinate of GameBlock
    *   @param y - row coordinate of GameBlock
    */
    GameBlock(int x, int y);
    /** Set state of GameBlock
    *   @param state which state to turn to
    */
    void setState(bool state);
    /** Get state of GameBlock
    *   @return state of GameBlock
    */
    bool getState();
    /** Get position by changing references
    *   @param x1 reference to be changed to x1_ coordinate;
    *   @param x2 reference to be changed to x2_ coordinate;
    *   @param y reference to be changed to y_ coordinate;
    */
    void getPositions(int& x1, int& x2, int& y);
    /** GameBlock destructor
    */
    ~GameBlock();
};