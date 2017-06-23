#include <iostream>

#include <sdl2/sdl.h>

#include "grid.h"
#include "ant.h"

using namespace std;

unsigned char colors[2][3] = {
  {255, 255, 255},
  {0, 0, 0},
};

void draw_grid(SDL_Renderer *ren, Grid &g)
{
  for (int y = 0; y < g.height; y++)
  {
    for (int x = 0; x < g.width; x++)
    {
      int i = g[x][y];
      SDL_Rect r = {x*10, y*10, 10, 10};
      SDL_SetRenderDrawColor(ren, colors[i][0], colors[i][1], colors[i][2], 255);
      SDL_RenderFillRect(ren, &r);
    }
  }
}

void draw_ant(SDL_Renderer *ren, Ant& a)
{
  SDL_Rect r = {a.x*10+2, a.y*10+2, 6, 6};
  SDL_SetRenderDrawColor(ren, a.color[0], a.color[1], a.color[2], 255);
  SDL_RenderFillRect(ren, &r);
}

int main(int argc, char** argv)
{
  cout << "Langton's ant SDL2" << endl;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    cerr << "Err: SDL_Init - " << SDL_GetError() << endl;
    return 1;
  }

  SDL_Window *win = SDL_CreateWindow(
    "Langton's ant",
    100, 100, 800, 600,
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

  bool running = true;

  Grid grid(80, 60);
  Ant ant(40, 40);

  while (running)
  {
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
      switch(e.type)
      {
        case SDL_QUIT: running = false; continue;
      }
    }

    SDL_RenderClear(ren);

    draw_grid(ren, grid);
    draw_ant(ren, ant);
    simulate(grid, ant);

    SDL_RenderPresent(ren);
    SDL_Delay(1);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cout << "Goodbye!" << endl;
}
