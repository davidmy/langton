#ifndef ANT_H
#define ANT_H

#include "grid.h"
#include "types.h"

#include <vector>

class Ant;

class Ant
{
public:
  enum move_type { turn_left, turn_right, hard_left, hard_right, move_forward, turn_back, wait, random };

public:
  vec2i pos;
  unsigned char color[3];
  int heading;
  int waiting;

  std::vector<move_type> move;

public:
  Ant(int x, int y, int h=0);
  ~Ant();

  int num_states() { return move.size(); }
};

void simulate(Grid* g, Ant* a);

#endif
