#include "GameBall.h"

GameBall::GameBall(int x, int y) 
{
    this->x_ = x;
    this->y_ = y;
    this->vX_ = 0;
    this->vY_ = -1;
}

void GameBall::getPosition(int &x, int &y)
{
    x = this->x_;
    y = this->y_;
}

void GameBall::setPosition(int x, int y) 
{
    this->x_ = x;
    this->y_ = y;
}

void GameBall::getVelocity(int &vX, int &vY)
{
    vX = this->vX_;
    vY = this->vY_;
}

void GameBall::setVelocity(int vX, int vY)
{
    this->vX_ = vX;
    this->vY_ = vY;
}

GameBall::~GameBall() = default;