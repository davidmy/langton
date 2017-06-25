#include "grid.h"

Grid::Grid(int width, int height, GridType type)
  :m_data(0)
  ,width(width)
  ,height(height)
  ,type(type)
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

void Grid::normalize(vec2i& pos) const
{
  while (pos.x<0) pos.x+=width;
  while (pos.x>=width) pos.x-=width;
  while (pos.y<0) pos.y+=height;
  while (pos.y>=height) pos.y-=height;
}

int& Grid::at(vec2i& pos)
{
  return (*this)[pos.x][pos.y];
}

void Grid::move(vec2i& pos, int heading) const
{
  if (type==square)
  {
    //             3
    // headings: 2 x 0
    //             1
    pos.x += heading==0? 1: heading==2? -1: 0;
    pos.y += heading==1? 1: heading==3? -1: 0;
  }
  else if (type==hex)
  {
    //            4 5
    // headings: 3 x 0
    //            2 1

    // mapping:
    //  y=   x=
    //   0   0 1 2 3
    //   1    0 1 2 3
    //   2   0 1 2 3

    switch (heading)
    {
      case 0: pos.x++; break;
      case 3: pos.x--; break;

      case 1:
      case 2:
        pos.x+=-heading+1+pos.y%2;
        pos.y++;
        break;
      case 4:
      case 5:
        pos.x+=heading-5+pos.y%2;
        pos.y--;
        break;
    }
  }
}

GridCol Grid::operator[](int ndx)
{
  while (ndx<0) ndx+=width;
  while (ndx>=width) ndx-=width;
  return GridCol(&(m_data[ndx*height]), width);
}

GridCol::GridCol(int* row, int height)
  :row(row), height(height)
{

}

int& GridCol::operator[](int ndx)
{
  while (ndx<0) ndx+=height;
  while(ndx>=height) ndx-=height;
  return row[ndx];
}
