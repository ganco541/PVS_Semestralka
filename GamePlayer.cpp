#include "GamePlayer.h"

GamePlayer::GamePlayer(int x1, int y)
{
    this->x1_ = x1;
    this->x2_ = x1+1;
    this->y_ = y;
    this->movementBuffer_ = new char[M_BUFFER_LENGTH]{};
    for (int i = 0; i < M_BUFFER_LENGTH; ++i)
    {
        this->movementBuffer_[i] = 'x';
    }
}

bool GamePlayer::move(int i)
{
    if (this->x1_ + i < BOUNDRY_LEFT || this->x2_ + i > BOUNDRY_RIGHT)
    {
        return false;
    }
    else 
    {
        this->x1_ += i;
        this->x2_ += i;
        return true;
    }
}

void GamePlayer::setSidewaysMovement(int i)
{
    this->sidewaysMovement_ = i;
}

void GamePlayer::getPosition(int& x1, int& x2)
{
    x1 = this->x1_;
    x2 = this->x2_;
}

void GamePlayer::setPosition(int x1, int y)
{
    this->x1_ = x1;
    this->x2_ = x1+1;
    this->y_ = y;
}

void GamePlayer::setPlaying(bool playing)
{
    this->playing_ = playing;
}

void GamePlayer::clearMovementBuffer()
{
    for (int i = 0; i < M_BUFFER_LENGTH; ++i)
    {
        this->movementBuffer_[i] = 'x';
    }
    this->leftMovementIndex_ = 0;
    this->rightMovementIndex_ = 0;
}

void GamePlayer::resetSidewaysMovement()
{
    this->sidewaysMovement_ = 0;
}

const int& GamePlayer::getSidewaysMovement()
{
    return this->sidewaysMovement_;
}

void GamePlayer::addMovement(char mov)
{
    this->bufferMutex_.lock();
    this->rightMovementIndex_ + 1 < M_BUFFER_LENGTH ? ++this->rightMovementIndex_ : this->rightMovementIndex_ = 0;
    this->movementBuffer_[this->rightMovementIndex_] = mov;
    this->bufferMutex_.unlock();
}

void GamePlayer::playerThreadMethod()
{
    while (this->playing_)
    {
        if (this->leftMovementIndex_ != this->rightMovementIndex_)
        {
            this->leftMovementIndex_ + 1 < M_BUFFER_LENGTH ? ++this->leftMovementIndex_ : this->leftMovementIndex_ = 0;
            this->bufferMutex_.lock();
            char mov = this->movementBuffer_[this->leftMovementIndex_];
            this->bufferMutex_.unlock();
            int i;
            if (mov == 'a')
            {
                i = -1;
            }
            else if (mov == 'd')
            {
                i = 1;
            }
            if (this->move(i)) this->setSidewaysMovement(i);
            this->movementBuffer_[this->leftMovementIndex_] = 'x';
        }
    }
}

GamePlayer::~GamePlayer()
{
    if (this->movementBuffer_ != nullptr) delete[] this->movementBuffer_;
}
