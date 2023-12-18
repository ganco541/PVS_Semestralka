#pragma once

class GameBall 
{
private:
    int x_;
    int y_;
    int vX_;
    int vY_;
public:
    /** GameBall constructor
    *   @param x - coloumn coordinate of ball
    *   @param y - row coordinate of ball
    */
    GameBall(int x, int y);
    /** Get position by changing references
    *   @param x - reference to be changed to x_ coordinate
    *   @param y - reference to be changed to y_ coordinate
    */
    void getPosition(int& x, int& y);
    /** Set position to coordinates
    *   @param x - coloumn coordinate
    *   @param y - row coordinate
    */
    void setPosition(int x, int y);
    /** Get velocity vector by changing references
    *   @param vX - reference to be changed to vX_ velocity
    *   @param vY - reference to be changed to vY_ velocity
    */
    void getVelocity(int& vX, int& vY);
    /** Set velocity vector
    *   @param vX - coloumn velocity
    *   @param vY - row velocity
    */
    void setVelocity(int vX, int vY);
    /** GameBall destructor
    */
    ~GameBall();
};