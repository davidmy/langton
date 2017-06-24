#ifndef ANT_H
#define ANT_H

#include "Grid.h"

#include <vector>

class Ant;

class Ant
{
public:
  int x, y;
  unsigned char color[3];
  int heading;

  std::vector<bool> turn_left;

public:
  Ant(int x, int y, int h=0);
  ~Ant();

  int num_states() { return turn_left.size(); }
};

void simulate(Grid* g, Ant* a);

#endif
