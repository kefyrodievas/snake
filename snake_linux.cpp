// #include <chrono>
// #include <conio.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <cstring>
#include <sys/ioctl.h>
// #include <stropts.h>`

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67



struct coord {
  int x, y;
};

struct _fruit {
  coord pos;
  bool consume = false;
};

struct _snake {
  std::list<coord> snake;
  int direction;
  int length;
};

int _kbhit() { // detects whether there is a keyboard input
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}



bool check_snake(_snake snake, coord coordinates);
void print_board(_snake snake, _fruit fruit);

int main() {
  _snake snake;
  int lb = 0, ub = 58;
  coord tmp_coord;
  tmp_coord.x = 28 / 2;
  tmp_coord.y = 58 / 2;
  snake.direction = 0;
  _fruit fruit;
  fruit.pos.x = rand() % 28;
  fruit.pos.y = rand() % 58;
  fruit.consume = false;
  snake.snake.push_front(tmp_coord);
  int c = 0;
  snake.length = 1;

  while (1) {

    if (fruit.consume == true) {
      fruit.pos.x = rand() % 28;
      fruit.pos.y = rand() % 58;
      fruit.consume = false;
    }
    if (check_snake(snake, fruit.pos)) {
      fruit.consume = true;
      snake.length++;
    }
    if (_kbhit())
      c = getchar();
    //   std::cout << c << "\n";
    switch (c) {
    case KEY_RIGHT:
      if (snake.direction != KEY_LEFT)
        snake.direction = c;
      break;
    case KEY_UP:
      if (snake.direction != KEY_DOWN)
        snake.direction = c;
      break;
    case KEY_LEFT:
      if (snake.direction != KEY_RIGHT)
        snake.direction = c;
      break;
    case KEY_DOWN:
      if (snake.direction != KEY_UP)
        snake.direction = c;
      break;
    default:
      break;
    }
    switch (snake.direction) {
    case KEY_RIGHT:
      tmp_coord = snake.snake.front();
      tmp_coord.y++;
      snake.snake.push_front(tmp_coord);
      if (snake.snake.size() > snake.length) {
        snake.snake.pop_back();
      }
      break;
    case KEY_UP:
      tmp_coord = snake.snake.front();
      tmp_coord.x--;
      snake.snake.push_front(tmp_coord);
      if (snake.snake.size() > snake.length) {
        snake.snake.pop_back();
      }
      break;
    case KEY_LEFT:
      tmp_coord = snake.snake.front();
      tmp_coord.y--;
      snake.snake.push_front(tmp_coord);
      if (snake.snake.size() > snake.length) {
        snake.snake.pop_back();
      }
      break;
    case KEY_DOWN:
      tmp_coord = snake.snake.front();
      tmp_coord.x++;
      snake.snake.push_front(tmp_coord);
      if (snake.snake.size() > snake.length) {
        snake.snake.pop_back();
      }
      break;
    }
    if (snake.snake.front().x == -1 || snake.snake.front().y == -1 ||
        snake.snake.front().x == 28 || snake.snake.front().y == 60) {
      system("clear");
      std::cout << "game over";
      getchar();
      return 0;
    }
    print_board(snake, fruit);

    usleep(100000);
  }
}

void print_board(_snake snake, _fruit fruit) {
  std::string str;
  system("clear");
  char board[28][58];
  coord tmp_coord;
  for (int i = 0; i < 28; i++) {
    memset(board[i], 0, 58 * sizeof(char));
  }
  char wall[60];
  memset(wall, '#', 60 * sizeof(char));
  str += wall;
  str += "\n";
  
  for (int i = 0; i < 28; i++) {
    str += '#';
    for (int j = 0; j < 60; j++) {
      tmp_coord.x = i;
      tmp_coord.y = j;
      if (check_snake(snake, tmp_coord))
        str += '@'; 
      else if (fruit.pos.x == tmp_coord.x && fruit.pos.y == tmp_coord.y)
        str += '*'; 
      else
        str += ' '; 
    }
    str += "#\n";

  }
  str += wall; 
  std::cout << str << "\n";
}

bool check_snake(_snake snake, coord coordinates) {
  for (coord i : snake.snake) {
    if (i.x == coordinates.x && i.y == coordinates.y)
      return true;
  }
  return false;
}
