#include "ant.h"

#define H2X(h) ((h)==0? 1: (h)==2? -1: 0)
#define H2Y(h) ((h)==1? 1: (h)==3? -1: 0)

Ant::Ant(int x, int y, int h)
  :x(x), y(y)
  ,color{255, 0, 0}
  ,heading(h)
  ,move{turn_left, turn_right}
{

}

Ant::~Ant()
{

}

void simulate(Grid* g, Ant* a)
{
  int value = (*g)[a->x][a->y];

  if (a->move.size() <= value)
  {
    (*g)[a->x][a->y] = 0;
  }
  else
  {
    switch (a->move[value])
    {
      case Ant::turn_left:
          a->heading = (a->heading+1)%4;
        break;
      case Ant::turn_right:
        a->heading = (a->heading+3)%4;
        break;
      case Ant::move_forward:
        break;
    }
    (*g)[a->x][a->y] = (value+1)%a->num_states();
  }

  a->x = a->x + H2X(a->heading);
  a->y = a->y + H2Y(a->heading);

  if (a->x < 0) a->x += g->width;
  else if (a->x >= g->width) a->x -= g->width;
  if (a->y < 0) a->y += g->height;
  else if (a->y >= g->height) a->y -= g->height;
}
