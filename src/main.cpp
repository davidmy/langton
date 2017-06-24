/*  Langton's Ant v1.1
 *  https://github.com/davidmy/langton
*/

#define LANGTON_VERSION_STR "v1.1"

#include <iostream>
#include <vector>

#include <sdl2/sdl.h>

#include "grid.h"
#include "ant.h"

using namespace std;

int g_frame_delay = 1;
int g_ticks_per_frame = 1;

void set_speed(int delay, int tpf)
{
  g_frame_delay = delay;
  g_ticks_per_frame = tpf;
}

#define FRAME_DELAY g_frame_delay

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

#define MAX_COLORS (sizeof(colors)/3)

#define TILE_SIZE 8/2
#define GRID_WIDTH 120*2
#define GRID_HEIGHT 90*2

#define ANT_OFFSET 1
#define ANT_SIZE (TILE_SIZE-2*ANT_OFFSET)

void draw_grid(SDL_Renderer *ren, Grid &g);
void draw_ant(SDL_Renderer *ren, Ant& a);

Grid grid(GRID_WIDTH, GRID_HEIGHT);
Ant ant(GRID_WIDTH/2, GRID_HEIGHT/2);
Ant ant2(GRID_WIDTH/2, GRID_HEIGHT/2);

Ant *ants[] = { &ant, &ant2 };

#define N_ANTS 2

void tick()
{
  for (int i=0; i<N_ANTS; i++) simulate(grid, *ants[i]);
}

void draw(SDL_Renderer *ren)
{
  draw_grid(ren, grid);

  for (int i=0; i<N_ANTS; i++) draw_ant(ren, *ants[i]);
}

void draw_grid(SDL_Renderer *ren, Grid &g)
{
  for (int y = 0; y < g.height; y++)
  {
    for (int x = 0; x < g.width; x++)
    {
      int i = g[x][y];
      SDL_Rect r = {x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE};
      SDL_SetRenderDrawColor(ren, colors[i][0], colors[i][1], colors[i][2], 255);
      SDL_RenderFillRect(ren, &r);
    }
  }
}

void draw_ant(SDL_Renderer *ren, Ant& a)
{
  SDL_Rect r = {a.x*TILE_SIZE+ANT_OFFSET, a.y*TILE_SIZE+ANT_OFFSET, ANT_SIZE, ANT_SIZE};
  SDL_SetRenderDrawColor(ren, a.color[0], a.color[1], a.color[2], 255);
  SDL_RenderFillRect(ren, &r);
}

class Button
{
public:
  typedef void (*button_event)();
private:
  SDL_Rect m_rect_up, m_rect_down;
  SDL_Texture *m_texture;

  SDL_Rect m_rect;
  bool m_pressed;

  std::vector<button_event> m_onclick_events;

public:
  Button(int x, int y, int w, int h, int texx, int texy, SDL_Texture* tex);
  ~Button();

  void draw(SDL_Renderer *ren);
  void handle_event(SDL_Event &e);

  void add_click_listener(button_event event) { m_onclick_events.push_back(event); }

private:
  void do_click();
};

Button::Button(int x, int y, int w, int h, int texx, int texy, SDL_Texture* tex)
  :m_rect_up{texx, texy, w, h}
  ,m_rect_down{texx, texy+h, w, h}
  ,m_texture(tex)
  ,m_rect{x, y, w, h}
  ,m_pressed(false)
{

}

Button::~Button()
{

}

void Button::draw(SDL_Renderer *ren)
{
  SDL_RenderCopy(ren, m_texture, m_pressed?&m_rect_down:&m_rect_up, &m_rect);
}

void Button::handle_event(SDL_Event &e)
{
  switch (e.type)
  {
  case SDL_MOUSEBUTTONDOWN:
    if (e.button.x >= m_rect.x && e.button.x < m_rect.x+m_rect.w
      && e.button.y >= m_rect.y && e.button.y < m_rect.y+m_rect.h
      && e.button.button == SDL_BUTTON_LEFT)
    {
      m_pressed = true;
    }
    break;
  case SDL_MOUSEBUTTONUP:
    if (m_pressed && e.button.button == SDL_BUTTON_LEFT)
    {
      m_pressed = false;
      if (e.button.x >= m_rect.x && e.button.x < m_rect.x+m_rect.w
        && e.button.y >= m_rect.y && e.button.y < m_rect.y+m_rect.h)
      {
        do_click();
      }
    }
  }
}

void Button::do_click()
{
  for (std::vector<button_event>::iterator it = m_onclick_events.begin(); it != m_onclick_events.end(); it++)
  {
    (**it)();
  }
}

void go_slow()
{
  set_speed(500, 1);
}

void go_stop()
{
  set_speed(10, 0);
}

void go_medium()
{
  set_speed(1, 1);
}

void go_fast()
{
  if (g_ticks_per_frame == 1)
    set_speed(1, 50);
  else
    set_speed(1, (3*g_ticks_per_frame)/2);
}

int main(int argc, char** argv)
{
  cout << "Langton's Ant with SDL2" LANGTON_VERSION_STR << endl;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    cerr << "Err: SDL_Init - " << SDL_GetError() << endl;
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow(
    "Langton's ant " LANGTON_VERSION_STR,
    100, 100, GRID_WIDTH*TILE_SIZE, GRID_HEIGHT*TILE_SIZE,
    SDL_WINDOW_SHOWN
  );

  if (win == NULL)
  {
    cerr << "Err: SDL_CreateWindow - " << SDL_GetError() << endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == NULL)
  {
    cerr << "Err: SDL_CreateRenderer - " << SDL_GetError() << endl;
    SDL_Quit();
    return 1;
  }

  SDL_Surface *button_surf = SDL_LoadBMP("data/buttons.bmp");
  if (button_surf == NULL)
  {
    cerr << "Err: SDL_LoadBMP - " << SDL_GetError() << endl;
    SDL_Quit();
    return 1;
  }
  SDL_Texture *button_tex = SDL_CreateTextureFromSurface(ren, button_surf);
  if (button_tex == NULL)
  {
    cerr << "Err: SDL_CreateTextureFromSurface - " << SDL_GetError() << endl;
    SDL_Quit();
    return 1;
  }
  SDL_FreeSurface(button_surf);

  Button btn_stop(5+40*0, 5, 32, 32, 32*3, 0, button_tex);
  Button btn_slow(5+40*1, 5, 32, 32, 32*0, 0, button_tex);
  Button btn_medium(5+40*2, 5, 32, 32, 32*1, 0, button_tex);
  Button btn_fast(5+40*3, 5, 32, 32, 32*2, 0, button_tex);

  btn_stop.add_click_listener((Button::button_event)(&go_stop));
  btn_slow.add_click_listener((Button::button_event)(&go_slow));
  btn_medium.add_click_listener((Button::button_event)(&go_medium));
  btn_fast.add_click_listener((Button::button_event)(&go_fast));

  Button *buttons[] = {
    &btn_stop, &btn_slow, &btn_medium, &btn_fast
  };

  bool running = true;

  while (running)
  {
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
      switch(e.type)
      {
        case SDL_QUIT: running = false; continue;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          for (int i = 0; i < 4; i++) buttons[i]->handle_event(e);
      }
    }

    SDL_RenderClear(ren);

    draw(ren);

    for (int i = 0; i < 4; i++) buttons[i]->draw(ren);

    SDL_RenderPresent(ren);
    SDL_Delay(FRAME_DELAY);

    for (int i = 0; i < g_ticks_per_frame; i++) tick();
  }

  SDL_DestroyTexture(button_tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cout << "Goodbye!" << endl;
}
