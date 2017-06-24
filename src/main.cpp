/*  Langton's Ant v1.1
 *  https://github.com/davidmy/langton
*/

#include <iostream>
#include <fstream>
#include <vector>

#include <sdl2/sdl.h>

#include "grid.h"
#include "ant.h"
#include "simulation.h"
#include "loadedsimulation.h"

#include "gui.h"
#include "version.h"

#define GRID_WIDTH 120*2
#define GRID_HEIGHT 90*2

int main(int argc, char** argv)
{
  std::cout << "Langton's Ant with SDL2" LANGTON_VERSION_STR << std::endl;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    std::cerr << "Err: SDL_Init - " << SDL_GetError() << std::endl;
    return 1;
  }

  try
  {
    MainWindow *win = new MainWindow(GRID_WIDTH*TILE_SIZE, GRID_HEIGHT*TILE_SIZE);

    Json::Value val;

    std::ifstream config_file("langton.json");
    if (config_file.good())
    {
      config_file >> val;
    }
    config_file.close();

    Simulation *sim = LoadedSimulation::load_json(val);

    win->set_sim(sim);

    bool running = true;

    while (running)
    {
      switch (win->handle_events())
      {
      case EVENT_QUIT: running = false; continue;
      }

      win->draw();
      win->tick();
    }

    delete sim;
  }
  catch(...)
  {
    SDL_Quit();
    return 1;
  }

  std::cout << "Goodbye!" << std::endl;
  return 0;
}
