#if !defined(GAME_HH)
#define GAME_HH

#include <cstdlib>
#include <curses.h>

#include "coordination.hh"
#include "direction.hh"
#include "snake.hh"

class Game {
private:
  bool is_over;
  Direction direction;
  unsigned int width, height;
  unsigned int score;
  Coordination fruit;
  unsigned int fruit_cycle_count;
  Snake snake;

private:
  Coordination rand_coordination() const {
    unsigned int x = rand() % this->width + 1;
    unsigned int y = rand() % this->height + 1;
    return Coordination(x, y);
  }

  int khbit() {
    int ch = getch();
    if (ch != ERR) {
      ungetch(ch);
      return 1;
    } else {
      return 0;
    }
  }

public:
  Game();
  ~Game();

public:
  bool get_is_over() const;
  void draw() const;
  void poll();
  void execute();
  unsigned int get_score() const;
};

#endif