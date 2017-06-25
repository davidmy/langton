#ifndef GRID_H
#define GRID_H

#include "types.h"

class Grid;
class GridCol;

class Grid
{
public:
  enum GridType { square, hex };

private:
  int *m_data;

public:
  const int width;
  const int height;
  const GridType type;

public:
  Grid(int width, int height, GridType type=square);
  ~Grid();

  void normalize(vec2i& pos) const;
  int& at(vec2i& pos);
  void move(vec2i& pos, int heading) const;

  virtual GridCol operator[](int ndx);
};

class GridCol
{
public:
  int * const row;
  int const height;

public:
  GridCol(int* row, int height);
  int& operator[](int ndx);
};

#endif
