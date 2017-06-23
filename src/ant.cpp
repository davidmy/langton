#include "ant.h"

#define H2X(h) ((h)==0? 1: (h)==2? -1: 0)
#define H2Y(h) ((h)==1? 1: (h)==3? -1: 0)

Ant::Ant(int x, int y, int h)
  :x(x), y(y)
  ,color{255, 0, 0}
  ,heading(h)
{

}

Ant::~Ant()
{

}

void simulate(Grid& g, Ant& a)
{
  int value = g[a.x][a.y];

  if (value == 0)
  {
    g[a.x][a.y] = 1;
    a.heading = (a.heading+1)%4;
  }
  else
  {
    g[a.x][a.y] = 0;
    a.heading = (a.heading+3)%4;
  }

  a.x = a.x + H2X(a.heading);
  a.y = a.y + H2Y(a.heading);

  if (a.x < 0) a.x += g.width;
  else if (a.x >= g.width) a.x -= g.width;
  if (a.y < 0) a.y += g.height;
  else if (a.y >= g.height) a.y -= g.height;
}
