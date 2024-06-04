#include <vector>

#include "coordination.hh"
#include "snake.hh"

Snake::Snake() : body(std::vector<Coordination>()), len(0) {}
Snake::~Snake() {}

unsigned int Snake::get_len() const { return this->len; }
const std::vector<Coordination> &Snake::get_body() const { return this->body; }

void Snake::move(const Direction direction) {
  Coordination &head = this->body.front();

  for (unsigned int i = this->len - 1; i > 0; i -= 1) {
    this->body[i] = this->body[i - 1];
  }

  switch (direction) {
  case Direction::Up:
    head.set_y(head.get_y() - 1);
    break;

  case Direction::Down:
    head.set_y(head.get_y() + 1);
    break;

  case Direction::Left:
    head.set_x(head.get_x() - 1);
    break;

  case Direction::Right:
    head.set_x(head.get_x() + 1);
    break;

  case Direction::Stop:
    break;

  default:
    break;
  }
}

void Snake::grow(const Coordination &old_tail) {
  this->body.push_back(old_tail);
  this->len += 1;
}

bool Snake::is_bite_self() const {
  for (unsigned int i = 1; i < this->len; i += 1) {
    if (this->body.front() == this->body[i]) {
      return true;
    }
  }

  return false;
}

const Coordination &Snake::get_head() const { return this->body.front(); }
const Coordination &Snake::get_tail() const { return this->body.back(); }