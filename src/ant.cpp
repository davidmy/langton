#include "ant.h"

#include <cstdlib>

#define H2X(h) ((h)==0? 1: (h)==2? -1: 0)
#define H2Y(h) ((h)==1? 1: (h)==3? -1: 0)

Ant::Ant(int x, int y, int h)
  :pos(x, y)
  ,color{128, 0, 0}
  ,heading(h)
  ,move{turn_left, turn_right}
  ,waiting(0)
{

}

Ant::~Ant()
{

}

void simulate(Grid* g, Ant* a)
{
  int value = g->at(a->pos);
  int num_headings = g->type==Grid::square? 4: g->type==Grid::hex? 6: 4;

  if (a->move.size() <= value)
  {
    g->at(a->pos) = 0;
  }
  else
  {
    switch (a->move[value])
    {
      case Ant::turn_left:
          a->heading = (a->heading+3)%num_headings;
        break;
      case Ant::turn_right:
        a->heading = (a->heading+1)%num_headings;
        break;
      case Ant::hard_left:
          a->heading = (a->heading+4)%num_headings;
        break;
      case Ant::hard_right:
        a->heading = (a->heading+2)%num_headings;
        break;
      case Ant::move_forward:
        break;
      case Ant::turn_back:
        a->heading = (a->heading+num_headings/2)%num_headings;
        break;
      case Ant::wait:
        a->waiting++;
        break;
      case Ant::random:
        a->heading = rand()%num_headings;
        break;
    }
    g->at(a->pos) = (value+1)%a->num_states();
  }

  if (a->waiting <= 0)
  {
    g->move(a->pos, a->heading);
  }
  else
  {
    a->waiting--;
  }

  g->normalize(a->pos);
}
