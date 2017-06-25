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

#define TILE_WIDTH 3
#define TILE_HEIGHT 3

#define TILE_X_OFFSET 4
#define TILE_Y_OFFSET 4

#define TILE_ODD_OFFSET (TILE_X_OFFSET/2)

#define ANT_OFFSET -1
#define ANT_SIZE 5

#define EVENT_QUIT 1

class MainWindow
{
public:
  MainWindow(int width, int height);
  ~MainWindow();

  void draw_grid(Grid *g);
  void draw_ant(Grid *g, Ant *a);

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

  int m_odd_offset;
  int m_tile_width;
  int m_tile_height;
  int m_tile_x_offset;
  int m_tile_y_offset;
  int m_ant_width;
  int m_ant_height;
  int m_ant_x_offset;
  int m_ant_y_offset;

public:
  void set_sim(Simulation *sim);
  Simulation* get_sim() const { return m_sim; }

  int get_ticks_per_frame() const { return m_ticks_per_frame; }
  void set_speed(int delay, int tpf) { m_frame_delay=delay; m_ticks_per_frame=tpf; }
};


#endif /* end of include guard: GUI_H */
