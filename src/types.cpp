#include "types.h"

vec2i::vec2i()
  :x(0), y(0)
{
}

vec2i::vec2i(int x, int y)
  :x(x), y(y)
{
}

vec2i::vec2i(int* v)
  :x(v[0]), y(v[1])
{
}
