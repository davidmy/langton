#ifndef LOADEDSIMULATION_H
#define LOADEDSIMULATION_H

#include "simulation.h"

#include <vector>
#include <json/json.h>

class Ant;
class Grid;

class LoadedSimulation;

class LoadedSimulation: public Simulation
{
private:
  LoadedSimulation();

public:
  ~LoadedSimulation();

  static LoadedSimulation* load_json(Json::Value& val);

private:
  Grid *m_loaded_grid;
  std::vector<Ant*> m_loaded_ants;
};

#endif /* end of include guard: LOADEDSIMULATION_H */
