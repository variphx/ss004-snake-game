#include <chrono>
#include <ctime>
#include <string>
#include <thread>

#include "game.hh"
#include "macros.hh"

Game::Game()
    : is_over(false), direction(Direction::Stop), width(WIDTH), height(HEIGHT),
      score(0), fruit(rand_coordination()), fruit_cycle_count(0),
      snake(Snake()) {
  srand(time(0));
  {
    const unsigned int x = width / 2;
    const unsigned int y = height / 2;
    Coordination head = Coordination(x, y);
    snake.grow(head);
  }
}

Game::~Game() {
  erase();

  std::string message = "Your final score %u";

  unsigned int row = 0, column = 0;
  getmaxyx(stdscr, row, column);

  move(row / 2, (column - message.length()) / 2);
  printw(message.c_str(), this->score);
  refresh();
  std::this_thread::sleep_for(std::chrono::seconds(3));
}