#include "grid.h"

Grid::Grid(int width, int height)
  :m_data(0)
  ,width(width)
  ,height(height)
{
  m_data = new int[width*height];

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      m_data[x*height+y] = 0;
    }
  }
}

Grid::~Grid()
{
  delete m_data;
}

int* Grid::operator[](int x)
{
  return &(m_data[x*height]);
}
