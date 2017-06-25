#ifndef TYPES_H
#define TYPES_H

class vec2i
{
public:
  union {
    struct {
      int x;
      int y;
    };
    int v[2];
  };

public:
  vec2i();
  vec2i(int x, int y);
  vec2i(int* v);
};

#endif /* end of include guard: TYPES_H */
