#ifndef ANT_H
#define ANT_H

#include "Grid.h"

#include <vector>

class Ant;

class Ant
{
public:
  enum move_type { turn_left, turn_right, move_forward };

public:
  int x, y;
  unsigned char color[3];
  int heading;

  std::vector<move_type> move;

public:
  Ant(int x, int y, int h=0);
  ~Ant();

  int num_states() { return move.size(); }
};

void simulate(Grid* g, Ant* a);

#endif
