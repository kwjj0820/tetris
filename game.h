
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  // line 설정할 변수.
  int inputLines_;

  // 도형 좌측 상단 위치
  int x_, y_;

  // 타이머를 위한 변수.
  clock_t start_;

  // 프레임 조절을 위한 변수.
  int timer;

  Tetromino* cur;
  Tetromino* next;
  Tetromino* hold;

  Tetromino* randomTetromino();
  std::string stringTime(clock_t start);
  void handleInput();
  void whenCollision();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif