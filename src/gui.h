#ifndef GUI_H
#define GUI_H

#include <vector>

//#include <sdl2/sdl.h>
class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

//#include "button.h"
class Button;

//#include "ant.h"
//#include "grid.h"
class Ant;
class Grid;
class Simulation;

#define TILE_SIZE (8/2)
#define ANT_OFFSET 1
#define ANT_SIZE (TILE_SIZE-2*ANT_OFFSET)

#define EVENT_QUIT 1

class MainWindow
{
public:
  MainWindow(int width, int height);
  ~MainWindow();

  void draw_grid(Grid *g);
  void draw_ant(Ant *a);

  int handle_events();
  void draw();
  void tick();

private:
  int m_frame_delay;
  int m_ticks_per_frame;

  SDL_Window *m_win;
  SDL_Renderer *m_ren;
  SDL_Texture *m_button_tex;
  std::vector<Button*> m_buttons;
  bool m_show_buttons;

  Simulation* m_sim;

public:
  void set_sim(Simulation *sim);
  Simulation* get_sim() const { return m_sim; }

  int get_ticks_per_frame() const { return m_ticks_per_frame; }
  void set_speed(int delay, int tpf) { m_frame_delay=delay; m_ticks_per_frame=tpf; }
};


#endif /* end of include guard: GUI_H */
