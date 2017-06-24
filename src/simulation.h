#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>

class Simulation;
class Ant;
class Grid;

class Simulation
{
private:
  Grid *m_grid;
  std::vector<Ant*> m_ants;

public:
  Simulation();
  virtual ~Simulation();

  void set_grid(Grid* grid) { m_grid = grid; }
  Grid* get_grid() const { return m_grid; }

  void add_ant(Ant* ant) { m_ants.push_back(ant); }
  const std::vector<Ant*>& get_ants() const { return m_ants; }

  void tick();
};

#endif /* end of include guard: SIMULATION_H */
