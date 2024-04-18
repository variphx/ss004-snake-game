#include "coordination.hh"

Coordination::Coordination() : x(0), y(0) {}
Coordination::Coordination(const unsigned int x, const unsigned int y)
    : x(x), y(y) {}
Coordination::~Coordination() {}
unsigned int Coordination::get_x() const { return this->x; }
unsigned int Coordination::get_y() const { return this->y; }
void Coordination::set_x(const unsigned int x) { this->x = x; }
void Coordination::set_y(const unsigned int y) { this->y = y; }
bool Coordination::operator==(const Coordination &other) const {
  return this->x == other.x && this->y == other.y;
}