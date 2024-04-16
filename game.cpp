#include "game.h"
#include "console/console.h"
#include <iostream>
#include "tetromino.h"

// 게임의 한 프레임을 처리한다.
void Game::update()
{
    checkTetrominoCollision();
    handleInput();
    cur = next;
    timer++;
    if(timer >= DROP_DELAY)
    {
        y_++;
        timer = 0;
    }
}

void Game::checkTetrominoCollision()
{
    int tempX = x_;
    int tempY = y_;
    for(int i = tempX; i < tempX + cur->size(); i++)
    {
        for(int j = tempY; j < tempY + cur->size(); j++)
        {
            if(cur->check(i, j))
            {
                if(i < 1)
                {
                    x_++;
                }
                else if(i >= BOARD_WIDTH)
                {
                    x_--;
                }
                else if(j >= BOARD_HEIGHT || board_[i][j])
                {
                    //whenCollision();
                }
            }
        }
    }
}

void Game::whenCollision()
{
    for(int i = x_; i < x_ + cur->size(); i++)
    {
        for(int j = y_; j < y_ + cur->size(); j++)
        {
            board_[i][j] = true;
        }
    }
    x_ = 5;
    y_ = 1;
    cur = next;
    next = randomTetromino();
}

void Game::handleInput()
{
    if (key(console::K_LEFT)) x_--;
    if (key(console::K_RIGHT)) x_++;
    if (key(console::K_DOWN)) y_++;
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

    return shape[rand() % 7];
}

std::string Game::stringTime(clock_t start)
{
    std::string time_string;
    clock_t end = clock();
    double secs = double(end - start) / CLOCKS_PER_SEC;

    int hour = int(secs / 3600);
    secs -= hour * 3600;
    int min = int(secs / 60);
    secs -= min * 60;
    int sec = int(secs);

    std::string str_hour = std::to_string(hour);
    std::string str_min = std::to_string(min);
    std::string str_sec = std::to_string(sec);

    if(sec < 10) str_sec = "0" + str_sec;
    if(min < 10) str_min = "0" + str_min;
    if(hour < 10) str_hour = "0" + str_hour;

    time_string = str_hour + ":" + str_min + ":" + str_sec;
    return time_string;
}

// 게임 화면을 그린다.
void Game::draw()
{
    console::drawBox(0, 0, 11, 21);
    console::drawBox(12, 0, 17, 5);
    console::drawBox(18, 0, 23, 5);
    console::draw(13, 0, "Next");
    console::draw(19, 0, "Hold");
    console::draw(0, 22, std::to_string(inputLines_) + " lines left");
    console::draw(2, 23, stringTime(start_));
    cur->drawAt(cur->name(), x_, y_);
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit()
{

}

Game::Game()
{
    std::cout << "Input Lines: ";
    std::cin >> inputLines_;
    start_ = clock();
    next = randomTetromino();
    x_ = 5;
    y_ = 1;
    timer = 0;
    for(int i = 0; i < BOARD_WIDTH; i++)
        for(int j = 0; j < BOARD_HEIGHT; j++) board_[i][j] = false;
}