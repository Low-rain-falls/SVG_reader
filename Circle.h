#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

//  <circle cx="200" cy="300" r="100" fill="rgb(255, 255, 0)" stroke="rgb(0,
//  255, 255)" stroke-width="10" stroke-opacity="0.7" fill-opacity="0.5" />
class Circle {
private:
  int cx, cy, r;
  Color fill;
  Color stroke;
  double stroke_width, stroke_opacity, fill_opacity;

public:
  Circle(int cx, int cy, int r, Color fill, Color stroke, double stroke_width,
         double stroke_opacity, double fill_opacity);
  void draw(Graphics graphic);
};

Circle::Circle(int cx, int cy, int r, Color fill, Color stroke,
               double stroke_width, double stroke_opacity,
               double fill_opacity) {
  this->cx = cx;
  this->cy = cy;
  this->r = r;
  this->fill = fill;
  this->stroke = stroke;
  this->stroke_width = stroke_width;
  this->stroke_opacity = stroke_opacity;
  this->fill_opacity = fill_opacity;
}

void Circle::draw(Graphics graphic) {
  BYTE strokeAlpha = static_cast<BYTE>(255 * this->stroke_opacity);
  BYTE fillAlpha = static_cast<BYTE>(255 * this->fill_opacity);

  Color strokeColor(strokeAlpha, this->stroke.GetR(), this->stroke.GetG(),
                    this->stroke.GetB());
  Color fillColor(fillAlpha, this->fill.GetR(), this->fill.GetG(),
                  this->fill.GetB());

  Pen pen(strokeColor, static_cast<REAL>(this->stroke_width));
  SolidBrush brush(fillColor);

  graphic.FillEllipse(&brush, this->cx - this->r, this->cy - this->r,
                      this->r * 2, this->r * 2);
  graphic.DrawEllipse(&pen, this->cx - this->r, this->cy - this->r, this->r * 2,
                      this->r * 2);
}
