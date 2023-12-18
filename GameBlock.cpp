#include "GameBlock.h"

GameBlock::GameBlock(int x, int y) 
{
    this->x1_ = x;
    this->x2_ = x+1;
    this->y_ = y;
    this->state_ = true;
}

void GameBlock::setState(bool state)
{
    this->state_ = state;
}

bool GameBlock::getState()
{
    return this->state_;
}

void GameBlock::getPositions(int &x1, int &x2, int &y) 
{
    x1 = this->x1_;
    x2 = this->x2_;
    y = this->y_;
}

GameBlock::~GameBlock()
{

}