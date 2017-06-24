#ifndef H_BUTTON
#define H_BUTTON

#include <vector>
#include <sdl2/sdl.h>

class Button;

class Button
{
public:
  typedef void (*button_event)(Button* b);
private:
  SDL_Rect m_rect_up, m_rect_down;
  SDL_Texture *m_texture;

  SDL_Rect m_rect;
  bool m_pressed;

  std::vector<button_event> m_onclick_events;

  void* m_data;

public:
  Button(int x, int y, int w, int h, int texx, int texy, SDL_Texture* tex);
  ~Button();

  void draw(SDL_Renderer *ren);
  void handle_event(SDL_Event &e);

  void add_click_listener(button_event event) { m_onclick_events.push_back(event); }

  void set_data(void* data) { m_data = data; }
  void* get_data() const { return m_data; }

private:
  void do_click();
};

#endif
