#include "BlockGameManager.h"

BlockGameManager::BlockGameManager()
{
    this->displayController_ = new DisplayController();
    this->gameBlocks_ = new GameBlock*[BLOCK_ARRAY_SIZE]{};
    
    int x = 1;
    int y = 1;
    for (int i = 0; i < BLOCK_ARRAY_SIZE; ++i)
    {
        this->gameBlocks_[i] = new GameBlock(x, y);
        if (x != 7) {
            x += 2;
        } else {
            x = 1;
            ++y;
        }
    }

    this->gameBall_ = new GameBall(4, 6);
    this->gameBall_->setVelocity(0, 1);

    this->gamePlayer_ = new GamePlayer(4, PLAYER_Y);
}

void BlockGameManager::turnOffAllLeds()
{
    for (int y = 1; y < 9; ++y)
    {
        for (int x = 1; x < 9; ++x)
        {
            this->displayController_->turnOffLed(x, y);
        }
    }
}

void BlockGameManager::resetGame()
{
    this->amountOfGameBlocks_ = 12;
    this->amountOfLives_ = 3;
    for (int i = 0; i < BLOCK_ARRAY_SIZE; ++i)
    {
        this->gameBlocks_[i]->setState(true);
    }

    this->resetBall();

    this->gamePlayer_->clearMovementBuffer();
    this->gamePlayer_->setPosition(4, PLAYER_Y);
}

void BlockGameManager::resetBall()
{
    this->gameBall_->setPosition(4, 6);
    this->gameBall_->setVelocity(0, 1);
}

void BlockGameManager::displayAll()
{
    this->turnOffAllLeds();
    this->displayController_->startDisplay();
    int x1 = 0;
    int x2 = 0;
    int y = 0;
    for (int i = 0; i < BLOCK_ARRAY_SIZE; ++i)
    {
        this->gameBlocks_[i]->getPositions(x1, x2, y);
        if (x1 != 0 && x2 != 0 && y != 0)
        {
            this->displayController_->turnOnLed(x1, y);
            this->displayController_->turnOnLed(x2, y);
        }
    }
    x1 = 0;
    x2 = 0;

    int x = 0;
    this->gameBall_->getPosition(x, y);
    if (x != 0 && y != 0) this->displayController_->turnOnLed(x, y);

    this->gamePlayer_->getPosition(x1, x2);
    if (x1 != 0 && x2 != 0) 
    {
        this->displayController_->turnOnLed(x1, PLAYER_Y);
        this->displayController_->turnOnLed(x2, PLAYER_Y);
    }
}

void BlockGameManager::destroyThreads()
{
    if (this->state_) this->state_ = false;
    if (this->gameThread_ != nullptr)
    {
        this->gameThread_->join();
        delete this->gameThread_;
        this->gameThread_ = nullptr;
    }
    if (this->playerThread_ != nullptr)
    {
        this->gamePlayer_->setPlaying(false);
        this->playerThread_->join();
        delete this->playerThread_;
        this->playerThread_ = nullptr;
    }
    if (this->playerLedsThread_ != nullptr)
    {
        this->playerLedsThread_->join();
        delete this->playerLedsThread_;
        this->playerLedsThread_ = nullptr;
    }
}

void BlockGameManager::playerUpdateLeds()
{
    int oldX1{0};
    int oldX2{0};
    int newX1{0};
    int newX2{0};
    this->gamePlayer_->getPosition(oldX1, oldX2);
    while (this->state_)
    {
        this->gamePlayer_->getPosition(newX1, newX2);
        if (oldX1 != newX1)
        {
            this->displayController_->turnOffLed(oldX1, PLAYER_Y);
            this->displayController_->turnOffLed(oldX2, PLAYER_Y);
            
            this->displayController_->turnOnLed(newX1, PLAYER_Y);
            this->displayController_->turnOnLed(newX2, PLAYER_Y);
            oldX1 = newX1;
            oldX2 = newX2;
        }
    }
}

bool BlockGameManager::getState()
{
    return this->state_;
}

int BlockGameManager::getAmountOfBlocks()
{
    return this->amountOfGameBlocks_;
}

int BlockGameManager::getAmountOfLives()
{
    return this->amountOfLives_;
}

void BlockGameManager::userInput(char c)
{
    if (c == 'a' || c == 'd') this->gamePlayer_->addMovement(c);
}

void BlockGameManager::startGame()
{
    if (this->state_) this->stopGame();
    this->resetGame();
    this->displayAll();
    this->state_ = true;
    this->gamePlayer_->setPlaying(true);
    this->gameThread_ = new Thread();
    this->playerThread_ = new Thread();
    this->playerLedsThread_ = new Thread(); 
    this->gameThread_->start(callback(this, &BlockGameManager::tic));
    this->playerThread_->start(callback(this->gamePlayer_, &GamePlayer::playerThreadMethod));
    this->playerLedsThread_->start(callback(this, &BlockGameManager::playerUpdateLeds));
}

void BlockGameManager::stopGame()
{
    if (this->state_) 
    {
        this->state_ = false;
        this->destroyThreads();
    }
}

void BlockGameManager::moveBall()
{
    int x{0};
    int newX{0};
    
    int y{0};
    int newY{0};
    
    int vX{0};
    int vY{0};

    bool rebound{false};
    
    this->gameBall_->getPosition(x, y);
    this->gameBall_->getVelocity(vX, vY);
    
    newX = x + vX;
    newY = y + vY;
    
    if (newX < 1)
    {
        newX = 2;
        vX = 1; 
        rebound = true;
    }
    else if (newX > 8)
    {
        newX = 7;
        vX = -1;
        rebound = true;
    }

    if (newY < 1)
    {
        newY = 2;
        vY = 1;
        rebound = true;
    } else if (newY > 8)
    {
        --this->amountOfLives_;
        if (this->amountOfLives_ == 0)
        {
            this->stopGame();
            return;
        }
        this->displayController_->turnOffLed(x, y);
        this->resetBall();
        this->gameBall_->getPosition(x, y);
        this->displayController_->turnOnLed(x, y);
        return;
    }

    if (newY > 0 && newY < 4)
    {
        int blockX1{0};
        int blockX2{0};
        int blockY{0};
        int destroyBlockX1{0};
        int destroyBlockX2{0};
        int destroyBlockY{0};
        GameBlock* destroyBlock = nullptr;
        for (int i = 0; i < BLOCK_ARRAY_SIZE; ++i)
        {
            GameBlock* tmpBlock = this->gameBlocks_[i];
            
            if (tmpBlock->getState())
            {
                tmpBlock->getPositions(blockX1, blockX2, blockY);
                if (vY < 0)
                {
                    if ((blockX1 == newX || blockX2 == newX) && blockY == y)
                    {
                        destroyBlock = tmpBlock;
                        vY = -1;
                        vX == 1 ? vX = -1 : vX = 1;
                        //Tu asi pouzit stare x/y
                        newY = y+vY;
                        newX = x+vX;

                        destroyBlockX1 = blockX1;
                        destroyBlockX2 = blockX2;
                        destroyBlockY = blockY;
                        
                        rebound = true;
                        break;
                    }
                    else if ((blockX1 == newX || blockX2 == newX) && blockY == newY)
                    {
                        if (destroyBlock == nullptr)
                        {
                            destroyBlock = tmpBlock;
                            vY = 1;
                            //Tu asi pouzit stare x/y
                            newY = y+vY;
                            newX = x+vX;

                            destroyBlockX1 = blockX1;
                            destroyBlockX2 = blockX2;
                            destroyBlockY = blockY;

                            rebound = true;
                        }
                    }
                }
                else 
                {
                    if ((blockX1 == newX || blockX2 == newX) && blockY == y)
                    {
                        destroyBlock = tmpBlock;
                        vY = 1;
                        vX == 1 ? vX = -1 : vX = 1;
                        //Tu asi pouzit stare x/y
                        newY = y+vY;
                        newX = x+vX;
                        rebound = true;
                        
                        destroyBlockX1 = blockX1;
                        destroyBlockX2 = blockX2;
                        destroyBlockY = blockY;
                        
                        break;
                    }
                    else if ((blockX1 == newX || blockX2 == newX) && blockY == newY)
                    {
                        if (destroyBlock == nullptr)
                        {
                            destroyBlock = tmpBlock;
                            vY = -1;
                            //Tu asi pouzit stare x/y
                            newY = y+vY;
                            newX = x+vX;

                            destroyBlockX1 = blockX1;
                            destroyBlockX2 = blockX2;
                            destroyBlockY = blockY;

                            rebound = true;
                        }
                    }
                }
                
            }
        }
        if (destroyBlock != nullptr)
        {
            destroyBlock->setState(false);
            this->displayController_->turnOffLed(destroyBlockX1, destroyBlockY);
            this->displayController_->turnOffLed(destroyBlockX2, destroyBlockY);
            --amountOfGameBlocks_;
            if (this->amountOfGameBlocks_ == 0)
            {
                this->stopGame();
                return;
            }
        }
    }
    
    if (newY == PLAYER_Y)
    {
        int playerX1{0};
        int playerX2{0};
        int sidewaysMov = this->gamePlayer_->getSidewaysMovement();
        this->gamePlayer_->getPosition(playerX1, playerX2);
        if (playerX1 == newX || playerX2 == newX)
        {
            
            vY = -1;
            sidewaysMov != 0 ? vX = sidewaysMov : vX = 0;
            newY += vY*2;
            newX += vX;
            rebound = true;
        }
    }

    if (newX < 1)
    {
        newX = 2;
        vX = 1; 
        rebound = true;
    }
    else if (newX > 8)
    {
        newX = 7;
        vX = -1;
        rebound = true;
    }

    if (newY < 1)
    {
        newY = 2;
        vY = 1;
        rebound = true;
    } else if (newY > 8)
    {
        --this->amountOfLives_;
        if (this->amountOfLives_ == 0)
        {
            this->stopGame();
            return;
        }
        this->displayController_->turnOffLed(x, y);
        this->resetBall();
        this->gameBall_->getPosition(x, y);
        this->displayController_->turnOnLed(x, y);
        return;
    }


    this->displayController_->turnOffLed(x, y);
    this->gameBall_->setPosition(newX, newY);
    if (rebound) this->gameBall_->setVelocity(vX, vY);
    this->displayController_->turnOnLed(newX, newY);
}

void BlockGameManager::tic()
{
    while (this->state_)
    {
        this->gamePlayer_->resetSidewaysMovement();
        ThisThread::sleep_for(500ms);
        this->moveBall();
    }
}


BlockGameManager::~BlockGameManager()
{
    if (this->state_) this->stopGame();
    delete this->displayController_;
    for (int i = 0; i < BLOCK_ARRAY_SIZE; ++i)
    {
        if (this->gameBlocks_[i] != nullptr) delete this->gameBlocks_[i];
    }
    delete[] this->gameBlocks_;
    delete this->gameBall_;
    delete this->gamePlayer_;
}
