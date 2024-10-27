#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <iostream>
#include <objidl.h>

using namespace std;
using namespace Gdiplus;

// <text x="0" y="0" fill="rgb(0,0,255)" font-size="40">SVG Demo</text>
// <text x="400" y="400" fill="rgb(255,0,255)" font-size="30">Nguyen Van
// A</text>

class myText {
private:
  int x;
  int y;
  Color fill;
  int fontSize;
  string text;

public:
  myText(int, int, Color, int, string);
  ~myText();
  void Draw(Graphics graphics);
};

myText::myText(int x, int y, Color fill, int fontSize, string text) {
  this->x = x;
  this->y = y;
  this->fill = fill;
  this->fontSize = fontSize;
  this->text = text;
}

void myText::Draw(Graphics graphics) {
  FontFamily fontFamily(L"Arial");
  Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
  SolidBrush fillDraw(this->fill);
  PointF pointF(this->x, this->y);
  std::wstring wtext(this->text.begin(), this->text.end());
  graphics.DrawString(wtext.c_str(), -1, &font, pointF, &fillDraw);
}

myText::~myText() {}
