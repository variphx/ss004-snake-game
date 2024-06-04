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

void Game::poll() {
  if (khbit()) {
    switch (getch()) {
    case 'a':
    case 'A':
      if (this->direction != Direction::Right) {
        this->direction = Direction::Left;
      }
      break;
    case 'd':
    case 'D':
      if (this->direction != Direction::Left) {
        this->direction = Direction::Right;
      }
      break;
    case 'w':
    case 'W':
      if (this->direction != Direction::Down) {
        this->direction = Direction::Up;
      }
      break;
    case 's':
    case 'S':
      if (this->direction != Direction::Up) {
        this->direction = Direction::Down;
      }
      break;
    case 'X':
      this->is_over = true;
      break;
    default:
      break;
    }
  }
}

void Game::execute() {
  const Coordination tail = this->snake.get_tail();
  this->snake.move(this->direction);

  if (this->snake.is_bite_self()) {
    this->is_over = true;
    return;
  }

  if (this->snake.get_head().get_x() == 0 ||
      this->snake.get_head().get_x() == this->width + 1 ||
      this->snake.get_head().get_y() == 0 ||
      this->snake.get_head().get_y() == this->height + 1) {
    this->is_over = true;
    return;
  }

  if (this->snake.get_head() == this->fruit) {
    if (this->fruit_cycle_count == 4) {
      this->score += 3;
      fruit_cycle_count = 0;
    } else {
      this->score += 1;
      this->fruit_cycle_count += 1;
    }

    this->snake.grow(tail);

    bool is_fruit_overlapped_snake = true;
    while (is_fruit_overlapped_snake) {
      is_fruit_overlapped_snake = false;
      this->fruit = rand_coordination();

      for (const auto &body_cell : this->snake.get_body()) {
        if (this->fruit == body_cell) {
          is_fruit_overlapped_snake = true;
          break;
        }
      }
    }
  }
}

unsigned int Game::get_score() const { return this->score; }