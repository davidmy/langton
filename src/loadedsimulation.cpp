#include "loadedsimulation.h"

#include "grid.h"
#include "ant.h"
#include "simulation.h"

LoadedSimulation::LoadedSimulation()
  :m_loaded_grid(NULL)
{

}

LoadedSimulation::~LoadedSimulation()
{
  delete m_loaded_grid;
  for (int i = 0; i < m_loaded_ants.size(); i++) delete m_loaded_ants[i];
}

LoadedSimulation* LoadedSimulation::load_json(Json::Value& val)
{
  LoadedSimulation *sim = new LoadedSimulation();

  int w = 100, h = 100;

  Json::Value& j_sim = val["langton_colony"];

  Json::Value& j_grid = j_sim["grid"];

  Grid::GridType grid_type = Grid::square;
  Json::Value j_grid_type = j_grid.get("type", "square");
  if (j_grid_type == "square")
  {
    grid_type = Grid::square;
  }
  else if (j_grid_type == "hex")
  {
    grid_type = Grid::hex;
  }
  int width = j_grid.get("width", 200).asInt();
  int height = j_grid.get("height", 150).asInt();

  Grid *grid = new Grid(width, height, grid_type);
  sim->m_loaded_grid = grid;
  sim->set_grid(grid);

  Json::Value& j_ants = j_sim["ants"];
  for (int i = 0; i < j_ants.size(); i++)
  {
    Json::Value& j_ant = j_ants[i];

    Ant *ant;
    ant = new Ant(j_ant.get("x", 0).asInt(), j_ant.get("y", 0).asInt());
    grid->normalize(ant->pos);

    Json::Value& j_turn = j_ant["turn"];
    ant->move.clear();
    for (int i = 0; i < j_turn.size(); i++)
    {
      if (j_turn[i] == "left") ant->move.push_back(Ant::turn_left);
      else if (j_turn[i] == "right") ant->move.push_back(Ant::turn_right);
      else if (j_turn[i] == "hard left") ant->move.push_back(Ant::hard_left);
      else if (j_turn[i] == "hard right") ant->move.push_back(Ant::hard_right);
      else if (j_turn[i] == "ahead") ant->move.push_back(Ant::move_forward);
      else if (j_turn[i] == "back") ant->move.push_back(Ant::turn_back);
      else if (j_turn[i] == "wait") ant->move.push_back(Ant::wait);
      else if (j_turn[i] == "random") ant->move.push_back(Ant::random);
    }

    sim->m_loaded_ants.push_back(ant);
    sim->add_ant(ant);
  }

  return sim;
}
