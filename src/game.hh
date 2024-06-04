#if !defined(SNAKE_HH)
#define SNAKE_HH

#include <vector>

#include "coordination.hh"
#include "direction.hh"

class Snake {
private:
  std::vector<Coordination> body;
  unsigned int len;

public:
  Snake();
  ~Snake();

public:
  unsigned int get_len() const;
  const std::vector<Coordination> &get_body() const;
  void move(const Direction direction);
  void grow(const Coordination &old_tail);
  bool is_bite_self() const;
  const Coordination &get_head() const;
  const Coordination &get_tail() const;
};

#endif
