#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

// <rect fill-opacity="0.01" stroke="rgb(200,200,200)" stroke-width="2" x="25"
// y="25" width="800" height="400" />

class myRect {
private:
  int x;
  int y;
  int width;
  int height;
  Color stroke;
  int strokeWidth;
  float fillOpacity;
  Color fill;

public:
  myRect(int x, int y, int width, int height, Color stroke, int strokeWidth, Color fill, float fillOpacity);
  ~myRect();
  void Draw(Graphics graphics);
};

myRect::myRect(int x, int y, int width, int height, Color stroke, int strokeWidth, Color fill, float fillOpacity) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->stroke = stroke;
  this->strokeWidth = strokeWidth;
  this->fillOpacity = fillOpacity;
  this->fill = fill;
}

void myRect::Draw(Graphics graphics) 
{
  Pen strokeDraw(this->stroke, this->strokeWidth);
  SolidBrush fillDraw(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
  graphics.FillRectangle(&fillDraw, this->x, this->y, this->width, this->height);
  graphics.DrawRectangle(&strokeDraw, this->x, this->y, this->width, this->height);
}

myRect::~myRect() {}
