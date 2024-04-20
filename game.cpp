#include "game.h"
#include "console/console.h"
#include <iostream>

// 게임의 한 프레임을 처리한다.
void Game::update()
{
    handleInput();
    timer++;
    if(timer >= DROP_DELAY)
    {
        bool canDown = true;
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j))
                {
                    if(y_ + j >= BOARD_HEIGHT)
                    {
                        canDown = false;
                        break;
                    }
                    else if(board_[x_ + i - 1][y_ + j]) isCollision();
                }
            }
        }
        if(canDown) y_++;
        else whenLand();
        timer = 0;
    }
    isLineFull();
}

void Game::setupNew()
{
    cur = next;
    next = randomTetromino();
    original_ = cur->original();
    x_ = 5;
    y_ = 1;
    for(int i = 0; i < cur->size(); i++)
    {
        for(int j = 0; j < cur->size(); j++)
        {
            if(cur->check(i, j) && board_[x_ + i - 1][y_ + j - 1])
            {
                isGameover = true;
            }
        }
    }
}

void Game::isLineFull()
{
    int cnt = 0;
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        cnt = 0;
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            if(board_[j][i]) cnt++;
        }
        if(cnt == 10)
        {
            inputLines_--;
            for(int x = 0; x < BOARD_WIDTH; x++)
            {
                board_[x][i] = false;
            }
            for(int k = i; k > 0; k--)
            {
                for(int x = 0; x < BOARD_WIDTH; x++)
                {
                    board_[x][k] = board_[x][k - 1];
                }
            }
        }
    }
}

void Game::isCollision()
{
    for(int i = 0; i < cur->size(); i++)
    {
        for(int j = 0; j < cur->size(); j++)
        {
            if(cur->check(i, j)) board_[x_ + i - 1][y_ + j - 1] = true;
        }
    }
    hold_cnt = 1;
    setupNew();
}

void Game::whenLand()
{
    for(int i = 0; i < cur->size(); i++)
    {
        for(int j = 0; j < cur->size(); j++)
        {
            if(cur->check(i, j)) board_[x_ + i - 1][y_ + j - 1] = true;
        }
    }
    hold_cnt = 1;
    setupNew();
}

void Game::handleInput()
{
    bool canMove = true;
    if(key(console::K_UP))
    {
        int drop_y = 0;
        while(canMove)
        {
            for(int i = 0; i < cur->size(); i++)
            {
                for(int j = 0; j < cur->size(); j++)
                {
                    if(cur->check(i, j) && (y_ + j + drop_y >= BOARD_HEIGHT || board_[x_ + i - 1][y_ + j + drop_y]))
                    {
                        canMove = false;
                        break;
                    }
                }
            }
            if(canMove) drop_y++;
        }
        y_ += drop_y;
        whenLand();
    }

    if (key(console::K_LEFT))
    {
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j))
                {
                    if(x_ + i <= 1 || board_[x_ + i - 2][y_ + j - 1])
                    {
                        canMove = false;
                        break;
                    }
                    else if(board_[x_ + i - 1][y_ + j])
                    {
                        isCollision();
                    }
                }
            }
        }
        if(canMove) x_--;
    }

    if (key(console::K_RIGHT))
    {
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j))
                {
                    if(x_ + i >= BOARD_WIDTH || board_[x_ + i][y_ + j - 1])
                    {
                        canMove = false;
                        break;
                    }
                    else if(board_[x_ + i - 1][y_ + j])
                    {
                        isCollision();
                    }
                }
            }
        }
        if(canMove) x_++;
    }
    
    if (key(console::K_DOWN))
    {
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j))
                {
                    if(y_ + j >= BOARD_HEIGHT || board_[x_ + i - 1][y_ + j])
                    {
                        canMove = false;
                        whenLand();
                        break;
                    }
                    else if(board_[x_ + i - 1][y_ + j])
                    {
                        isCollision();
                    }
                }
            }
        }
        if(canMove) y_++;
    }
    if (key(console::K_X))
    {
        Tetromino newCur = cur->rotatedCW();
        bool canRotate = true;
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j) && x_ + i >= BOARD_WIDTH)
                {
                    x_ = BOARD_WIDTH - cur->size() + 1;
                }
                else if(cur->check(i, j) && x_ + i <= 1) x_ = 1;
                else if(y_ + j >= BOARD_HEIGHT) y_ = BOARD_HEIGHT - cur->size();
                if(newCur.check(i, j) && board_[x_ + i - 1][y_ + j])
                {
                    canRotate = false;
                }
            }
        }
        if(canRotate) *cur = newCur;
    }
    if (key(console::K_Z))
    {
        Tetromino newCur = cur->rotatedCCW();
        bool canRotate = true;
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j) && x_ + i >= BOARD_WIDTH)
                {
                    x_ = BOARD_WIDTH - cur->size() + 1;
                }
                else if(cur->check(i, j) && x_ + i <= 1) x_ = 1;
                else if(y_ + j >= BOARD_HEIGHT) y_ = BOARD_HEIGHT - cur->size();
                if(newCur.check(i, j) && board_[x_ + i - 1][y_ + j])
                {
                    canRotate = false;
                }
            }
        }
        if(canRotate) *cur = newCur;
    }

    if(key(console::K_SPACE))
    {
        hold_cnt--;
        if(hold_cnt == 0)
        {
            if(hold == nullptr)
            {
                hold = cur->original();
                setupNew();
            }
            else
            {
                Tetromino* temp = cur->original();
                cur = new Tetromino(*hold);
                delete hold;
                hold = temp;
                x_ = 5;
                y_ = 1;
            }
        }
    }
}

Tetromino* Game::randomTetromino()
{
    Tetromino* shape[7];
    shape[0] = &Tetromino::I;
    shape[1] = &Tetromino::O;
    shape[2] = &Tetromino::T;
    shape[3] = &Tetromino::S;
    shape[4] = &Tetromino::Z;
    shape[5] = &Tetromino::J;
    shape[6] = &Tetromino::L;
    srand(time(NULL));

    return new Tetromino(*shape[rand() % 7]);
}

std::string Game::stringTime(clock_t start)
{
    std::string time_string;
    clock_t end = clock();
    double total_millisecs = double(end - start) * 1000.0 / CLOCKS_PER_SEC;

    int min = int(total_millisecs / 60000);
    total_millisecs -= min * 60000;
    int sec = int(total_millisecs / 1000);
    total_millisecs -= sec * 1000;
    int millisec = int(total_millisecs) / 10; // 밀리세컨드를 두 자릿수로 제한

    std::string str_min = (min < 10) ? "0" + std::to_string(min) : std::to_string(min);
    std::string str_sec = (sec < 10) ? "0" + std::to_string(sec) : std::to_string(sec);
    std::string str_millisec = (millisec < 10) ? "0" + std::to_string(millisec) : std::to_string(millisec);

    time_string = str_min + ":" + str_sec + ":" + str_millisec;
    return time_string;
}



// 게임 화면을 그린다.
void Game::draw()
{
    int shadow_y = 0;
    bool canMove = true;
    while(canMove)
    {
        for(int i = 0; i < cur->size(); i++)
        {
            for(int j = 0; j < cur->size(); j++)
            {
                if(cur->check(i, j) && (y_ + j + shadow_y >= BOARD_HEIGHT || board_[x_ + i - 1][y_ + j + shadow_y]))
                {
                    canMove = false; 
                    break;
                }
            }
        }
        if(canMove) shadow_y++; 
    }

    for(int i = 0; i < cur->size(); i++)
    {
        for(int j = 0; j < cur->size(); j++)
        {
            if(cur->check(i, j)) console::draw(i + x_, j + y_ + shadow_y, SHADOW_STRING);
        }
    }

    console::drawBox(0, 0, 11, 21);
    console::drawBox(12, 0, 17, 5);
    console::drawBox(18, 0, 23, 5);
    console::draw(13, 0, "Next");
    int s_x, s_y;
    if(next->size() == 2)
    {
        s_x = 14;
        s_y = 2;
    }
    else if(next->size() == 3)
    {
        s_x = 13;
        s_y = 2;
    }
    else
    {
        s_x = 13;
        s_y = 1;
    }
    for(int i = 0; i < next->size(); i++)
    {
        for(int j = 0; j < next->size(); j++)
        {
            if(next->check(i, j)) console::draw(i + s_x, j + s_y, BLOCK_STRING);
        }
    }
    if(hold != nullptr)
    {
        if(hold->size() == 2)
        {
            s_x = 20;
            s_y = 2;
        }
        else if(hold->size() == 3)
        {
            s_x = 19;
            s_y = 2;
        }
        else
        {
            s_x = 19;
            s_y = 1;
        }
        for(int i = 0; i < hold->size(); i++)
        {
            for(int j = 0; j < hold->size(); j++)
            {
                if(hold->check(i, j)) console::draw(s_x + i, s_y + j, BLOCK_STRING);
            }
        }
    }
    console::draw(19, 0, "Hold");
    console::draw(0, 22, std::to_string(inputLines_) + " lines left");
    console::draw(2, 23, stringTime(start_));
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j++)
        {
            if(board_[i][j]) console::draw(i + 1,  j + 1, BLOCK_STRING);
        }
    }
    cur->drawAt(cur->name(), x_, y_);
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{
    if(inputLines_ == 0)
    {
        console::draw(BOARD_WIDTH/2, BOARD_HEIGHT/2, "YOU WIN!");
        return true;
    }
    if(isGameover)
    {
        console::draw(BOARD_WIDTH/2, BOARD_HEIGHT/2, "GAME OVER!");
        return true;
    }
}

Game::Game()
{
    std::cout << "Input Lines: ";
    std::cin >> inputLines_;
    start_ = clock();
    next = randomTetromino();
    cur = randomTetromino();
    original_ = cur->original();
    hold = nullptr;
    x_ = 5;
    y_ = 1;
    timer = 0;
    hold_cnt = 1;
    isGameover = false;
    for(int i = 0; i < BOARD_WIDTH; i++)
        for(int j = 0; j < BOARD_HEIGHT; j++) board_[i][j] = false;
}