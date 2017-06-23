#ifndef ANT_H
#define ANT_H

#include "Grid.h"

class Ant;

class Ant
{
public:
  int x, y;
  unsigned char color[3];
  int heading;

public:
  Ant(int x, int y, int h=0);
  ~Ant();
};

void simulate(Grid& g, Ant& a);

#endif
