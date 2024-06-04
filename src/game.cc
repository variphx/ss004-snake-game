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

bool Game::get_is_over() const { return this->is_over; }
void Game::draw() const {
  erase();

  // fill background
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  for (unsigned int i = 0; i < getmaxy(stdscr); i += 1) {
    mvhline(i, 0, ' ', getmaxx(stdscr));
  }
  attroff(COLOR_PAIR(BACKGROUND_PAIR));

  const unsigned int max_width = getmaxx(stdscr);
  const unsigned int max_height = getmaxy(stdscr);

  const unsigned int horizontal_border_len = this->width + 2;
  const unsigned int vertical_border_len = this->height + 2;

  const unsigned int x_start = (max_width - horizontal_border_len) / 2;
  const unsigned int y_start = (max_height - vertical_border_len) / 2;

  attron(COLOR_PAIR(WALL_PAIR));
  for (unsigned int i = 0; i < horizontal_border_len; i += 1) {
    mvaddch(y_start, x_start + i, ' ');
    mvaddch(y_start + vertical_border_len - 1, x_start + i, ' ');
  }
  attroff(COLOR_PAIR(WALL_PAIR));

  attron(COLOR_PAIR(WALL_PAIR));
  for (unsigned int i = 0; i < vertical_border_len; i += 1) {
    mvaddch(y_start + i, x_start, ' ');
    mvaddch(y_start + i, x_start + horizontal_border_len - 1, ' ');
  }
  attroff(COLOR_PAIR(WALL_PAIR));

  // Seed a special fruit for every 5 fruits
  if (this->fruit_cycle_count == 4) {
    attron(COLOR_PAIR(GOLDEN_APPLE_PAIR));
    mvaddch(y_start + this->fruit.get_y(), x_start + this->fruit.get_x(), ' ');
    attroff(COLOR_PAIR(GOLDEN_APPLE_PAIR));
  } else {
    attron(COLOR_PAIR(APPLE_PAIR));
    mvaddch(y_start + this->fruit.get_y(), x_start + this->fruit.get_x(), ' ');
    attroff(COLOR_PAIR(APPLE_PAIR));
  }

  attron(COLOR_PAIR(SNAKE_PAIR));
  mvaddch(y_start + this->snake.get_head().get_y(),
          x_start + this->snake.get_head().get_x(), ' ');

  const std::vector<Coordination> &snake_body = this->snake.get_body();

  for (unsigned int i = 1; i < snake.get_len(); i += 1) {
    mvaddch(y_start + snake_body[i].get_y(), x_start + snake_body[i].get_x(),
            ' ');
  }
  attroff(COLOR_PAIR(SNAKE_PAIR));

  move(y_start + vertical_border_len, x_start);
  attron(COLOR_PAIR(BACKGROUND_PAIR));
  printw("Score: %u", this->score);
  attroff(COLOR_PAIR(BACKGROUND_PAIR));

  refresh();
}