#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>

#include "gui.h"
#include "button.h"
#include "version.h"

#include "grid.h"
#include "ant.h"
#include "simulation.h"

unsigned char colors[14][3] = {
  {255, 255, 255},  // white
  {0, 0, 0},        // black
  {255, 0, 0},      // red
  {255, 128, 0},    // orange
  {255, 255, 0},    // yellow
  {128, 255, 0},    // lime
  {0, 255, 0},      // green
  {0, 255, 128},    // mint
  {0, 255, 255},    // cyan
  {0, 128, 255},    // navy
  {0, 0, 255},      // blue
  {128, 0, 255},    // violet
  {255, 0, 255},    // purple
  {255, 0, 128},    // magenta
};

void go_slow(Button* b)
{
  MainWindow *win = reinterpret_cast<MainWindow*>(b->get_data());
  win->set_speed(500, 1);
}

void go_stop(Button* b)
{
  MainWindow *win = reinterpret_cast<MainWindow*>(b->get_data());
  win->set_speed(10, 0);
}

void go_medium(Button* b)
{
  MainWindow *win = reinterpret_cast<MainWindow*>(b->get_data());
  win->set_speed(1, 1);
}

void go_fast(Button* b)
{
  MainWindow *win = reinterpret_cast<MainWindow*>(b->get_data());

  int tpf = win->get_ticks_per_frame();
  if (tpf == 1)
  {
    win->set_speed(1, 50);
  }
  else
  {
    win->set_speed(1, (tpf*3)/2);
  }
}

MainWindow::MainWindow(int width, int height)
  :m_frame_delay(1)
  ,m_ticks_per_frame(1)
  ,m_win(NULL)
  ,m_ren(NULL)
  ,m_sim(NULL)
  ,m_show_buttons(false)
  ,m_odd_offset(0)
  ,m_tile_width(TILE_HEIGHT) // TILE_WIDTH only when grid type is hex
  ,m_tile_height(TILE_HEIGHT)
  ,m_tile_x_offset(TILE_Y_OFFSET) // TILE_X_OFFSET only when grid type is hex
  ,m_tile_y_offset(TILE_Y_OFFSET)
  ,m_ant_width(ANT_SIZE)
  ,m_ant_height(ANT_SIZE)
  ,m_ant_x_offset(ANT_OFFSET)
  ,m_ant_y_offset(ANT_OFFSET)
{
  m_win = SDL_CreateWindow(
    "Langton's ant " LANGTON_VERSION_STR,
    100, 100, width, height,
    SDL_WINDOW_SHOWN
  );
  if (m_win == NULL)
  {
    std::cerr << "Err: SDL_CreateWindow - " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to load window.");
  }

  m_ren = SDL_CreateRenderer(m_win, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (m_ren == NULL)
  {
    std::cerr << "Err: SDL_CreateRenderer - " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to create m_renderer.");
  }

  SDL_Surface *button_surf = SDL_LoadBMP("data/buttons.bmp");
  if (button_surf == NULL)
  {
    std::cerr << "Err: SDL_LoadBMP - " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to load button image.");
  }
  m_button_tex = SDL_CreateTextureFromSurface(m_ren, button_surf);
  if (m_button_tex == NULL)
  {
    std::cerr << "Err: SDL_CreateTextureFromSurface - " << SDL_GetError() << std::endl;
    throw std::runtime_error("Failed to convert button image to texture.");
  }
  SDL_FreeSurface(button_surf);

  Button *btn_stop, *btn_slow, *btn_medium, *btn_fast;

  btn_stop = new Button(5+40*0, 5, 32, 32, 32*3, 0, m_button_tex);
  btn_slow = new Button(5+40*1, 5, 32, 32, 32*0, 0, m_button_tex);
  btn_medium = new Button(5+40*2, 5, 32, 32, 32*1, 0, m_button_tex);
  btn_fast = new Button(5+40*3, 5, 32, 32, 32*2, 0, m_button_tex);

  btn_stop->add_click_listener(&go_stop);
  btn_slow->add_click_listener(&go_slow);
  btn_medium->add_click_listener(&go_medium);
  btn_fast->add_click_listener(&go_fast);

  btn_stop->set_data(reinterpret_cast<void*>(this));
  btn_slow->set_data(reinterpret_cast<void*>(this));
  btn_medium->set_data(reinterpret_cast<void*>(this));
  btn_fast->set_data(reinterpret_cast<void*>(this));

  m_buttons = { btn_stop, btn_slow, btn_medium, btn_fast };

}

MainWindow::~MainWindow()
{
  for (int i = 0; i < m_buttons.size(); i++) delete m_buttons[i];

  SDL_DestroyTexture(m_button_tex);
  SDL_DestroyRenderer(m_ren);
  SDL_DestroyWindow(m_win);
  SDL_Quit();
}

int MainWindow::handle_events()
{
  SDL_Event e;

  while(SDL_PollEvent(&e))
  {
    switch(e.type)
    {
      case SDL_QUIT: return EVENT_QUIT;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        for (int i = 0; i < 4; i++) m_buttons[i]->handle_event(e);
        break;

      case SDL_MOUSEMOTION:
        if (e.motion.x < (32*4+64) && e.motion.y < (32*1+64)) m_show_buttons = true;
        else m_show_buttons = false;
        break;
    }
  }
  return 0;
}

void MainWindow::tick()
{
  if (m_sim != NULL)
  {
    for (int i = 0; i < m_ticks_per_frame; i++) m_sim->tick();
  }
}

void MainWindow::draw()
{
  SDL_SetRenderDrawColor(m_ren, 128, 128, 128, 255);
  SDL_RenderClear(m_ren);

  if (m_sim != NULL)
  {
    Grid *grid = m_sim->get_grid();
    if (grid != NULL) draw_grid(grid);

    const std::vector<Ant*> &ants = m_sim->get_ants();
    for (int i=0; i<ants.size(); i++) if (ants[i] != NULL) draw_ant(grid, ants[i]);
  }

  if (m_show_buttons)
  {
    for (int i = 0; i < 4; i++) m_buttons[i]->draw(m_ren);
  }

  SDL_RenderPresent(m_ren);

  SDL_Delay(m_frame_delay);
}

void MainWindow::set_sim(Simulation *sim)
{
  m_sim = sim;
  if (m_sim != NULL)
  {
    Grid* grid = m_sim->get_grid();
    if (grid != NULL)
    {
      switch (grid->type)
      {
        case Grid::hex:
          m_tile_width = TILE_WIDTH;
          m_tile_height = TILE_HEIGHT;
          m_tile_x_offset = TILE_X_OFFSET;
          m_tile_y_offset = TILE_Y_OFFSET;
          m_odd_offset = TILE_X_OFFSET/2;
          m_ant_width = ANT_SIZE;
          m_ant_height = ANT_SIZE;
          m_ant_x_offset = ANT_OFFSET;
          m_ant_y_offset = ANT_OFFSET;
          break;
        case Grid::square:
          m_tile_width = TILE_HEIGHT;
          m_tile_height = TILE_HEIGHT;
          m_tile_x_offset = TILE_Y_OFFSET;
          m_tile_y_offset = TILE_Y_OFFSET;
          m_odd_offset = 0;
          m_ant_width = ANT_SIZE;
          m_ant_height = ANT_SIZE;
          m_ant_x_offset = ANT_OFFSET;
          m_ant_y_offset = ANT_OFFSET;
          break;
      }

      SDL_SetWindowSize(m_win, grid->width*m_tile_x_offset+m_odd_offset, grid->height*m_tile_y_offset);
    }
  }
}

void MainWindow::draw_grid(Grid *g)
{
  for (int y = 0; y < g->height; y++)
  {
    for (int x = 0; x < g->width; x++)
    {
      SDL_Rect r = { x*m_tile_x_offset+m_odd_offset*(y%2), y*m_tile_y_offset, m_tile_width, m_tile_height };
      int i = (*g)[x][y];
      SDL_SetRenderDrawColor(m_ren, colors[i][0], colors[i][1], colors[i][2], 255);
      SDL_RenderFillRect(m_ren, &r);
    }
  }
}

void MainWindow::draw_ant(Grid *g, Ant *a)
{
  SDL_Rect r = {a->pos.x*m_tile_x_offset+m_ant_x_offset+m_odd_offset*(a->pos.y%2), a->pos.y*m_tile_x_offset+m_ant_y_offset, m_ant_width, m_ant_height};
  SDL_SetRenderDrawColor(m_ren, a->color[0], a->color[1], a->color[2], 255);
  SDL_RenderFillRect(m_ren, &r);
}
