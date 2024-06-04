#include <chrono>
#include <string>
#include <thread>

#if defined(_WIN32)
#include <conio.h>
#else
#include <curses.h>
#endif

#include "game.hh"
#include "macros.hh"

int main() {
#if !defined(_WIN32)
  initscr();

  start_color();
  init_pair(BACKGROUND_PAIR, COLOR_BLACK, COLOR_WHITE);
  init_pair(WALL_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(SNAKE_PAIR, COLOR_WHITE, COLOR_GREEN);
  init_pair(APPLE_PAIR, COLOR_WHITE, COLOR_RED);
  init_pair(GOLDEN_APPLE_PAIR, COLOR_WHITE, COLOR_YELLOW);

  nodelay(stdscr, true);
  keypad(stdscr, true);
  noecho();
  curs_set(false);
#endif
  std::string message = "Welcome to our Snake Game";

  const unsigned int max_column = getmaxx(stdscr);
  const unsigned int max_row = getmaxy(stdscr);

  move(max_row / 2, (max_column - message.length()) / 2);
  printw("%s", message.c_str());
  refresh();
  std::this_thread::sleep_for(std::chrono::seconds(3));

  {
    Game game = Game();

    while (!game.get_is_over()) {
      game.draw();
      game.poll();
      game.execute();

      std::this_thread::sleep_for(
          std::chrono::milliseconds(150 - game.get_score()));
    }
  }

#if !defined(_WIN32)
  endwin();
#endif

  return 0;
}