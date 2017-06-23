#ifndef GRID_H
#define GRID_H

class Grid;

class Grid
{
private:
  int *m_data;

public:
  const int width;
  const int height;

public:
  Grid(int width, int height);
  ~Grid();

  int* operator[](int ndx);
};

#endif
