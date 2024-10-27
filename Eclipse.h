#pragma once

#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

class Eclipse {
private:
  int cx, cy, rx, ry;
  Color stroke;
  Color fill;
  double stroke_opacity, fill_opacity, stroke_width;

public:
  Eclipse(int cx, int cy, int rx, int ry, Color stroke, Color fill,
          double stroke_width, double stroke_opacity, double fill_opacity);
  void draw(Graphics graphic);
};

Eclipse::Eclipse(int cx, int cy, int rx, int ry, Color stroke, Color fill,
                 double stroke_width, double stroke_opacity,
                 double fill_opacity) {
  this->cx = cx;
  this->cy = cy;
  this->rx = rx;
  this->ry = ry;
  this->fill = fill;
  this->stroke = stroke;
  this->stroke_width = stroke_width;
  this->stroke_opacity = stroke_opacity;
  this->fill_opacity = fill_opacity;
}

void Eclipse::draw(Graphics graphic) {
  BYTE strokeAlpha = static_cast<BYTE>(255 * this->stroke_opacity);
  BYTE fillAlpha = static_cast<BYTE>(255 * this->fill_opacity);

  Color strokeColor(strokeAlpha, this->stroke.GetR(), this->stroke.GetG(),
                    this->stroke.GetB());
  Color fillColor(fillAlpha, this->fill.GetR(), this->fill.GetG(),
                  this->fill.GetB());

  Pen pen(strokeColor, static_cast<REAL>(this->stroke_width));
  SolidBrush brush(fillColor);

  graphic.FillEllipse(&brush, this->cx - this->rx, this->cy - this->ry,
                      this->rx * 2, this->ry * 2);
  graphic.DrawEllipse(&pen, this->cx - this->rx, this->cy - this->ry,
                      this->rx * 2, this->ry * 2);
}
