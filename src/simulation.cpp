#include "simulation.h"

#include "grid.h"
#include "ant.h"

Simulation::Simulation()
  :m_grid(0)
{

}

Simulation::~Simulation()
{

}

void Simulation::tick()
{
  if (m_grid != 0)
  {
    for (int i = 0; i < m_ants.size(); i++) {
      simulate(m_grid, m_ants[i]);
    }
  }
}
