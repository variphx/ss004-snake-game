#if !defined(COORDINATION_HH)
#define COORDINATION_HH

class Coordination {
private:
  unsigned int x, y;

public:
  Coordination();
  Coordination(const unsigned int x, const unsigned int y);
  ~Coordination();

public:
  unsigned int get_x() const;
  unsigned int get_y() const;

  void set_x(const unsigned int x);
  void set_y(const unsigned int y);
  bool operator==(const Coordination &other) const;
};

#endif