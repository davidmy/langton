#include "button.h"

Button::Button(int x, int y, int w, int h, int texx, int texy, SDL_Texture* tex)
  :m_rect_up{texx, texy, w, h}
  ,m_rect_down{texx, texy+h, w, h}
  ,m_texture(tex)
  ,m_rect{x, y, w, h}
  ,m_pressed(false)
  ,m_data(0)
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
    (**it)(this);
  }
}
