#ifndef STOP_H
#define STOP_H

#include <gdiplus.h>

using namespace Gdiplus;

class Stop {
public:
  double offset;
  double opacity;
  Color color;

  Stop(double offset, double opacity, Color color)
      : offset(offset), opacity(opacity), color(color) {}
};

#endif
