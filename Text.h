#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <iostream>
<<<<<<< HEAD
#include <objidl.h>
#include "SVGElement.h"
||||||| 2de2cbb
=======
#include <objidl.h>
>>>>>>> master

using namespace std;
using namespace Gdiplus;

// <text x="0" y="0" fill="rgb(0,0,255)" font-size="40">SVG Demo</text>
// <text x="400" y="400" fill="rgb(255,0,255)" font-size="30">Nguyen Van
// A</text>

<<<<<<< HEAD
class my_text : public SVGElement
{
||||||| 2de2cbb
class myText
{
private:
	int x;
	int y;
	Color fill;
	int fontSize;
	string text;
=======
class myText {
private:
  int x;
  int y;
  Color fill;
  int fontSize;
  string text;

>>>>>>> master
public:
<<<<<<< HEAD
	my_text(int x, int y, Color stroke, int font_size, string value) : SVGElement(x, y, stroke, font_size, value) {}
||||||| 2de2cbb
	myText(int, int, Color, int, string);
	~myText();
	void Draw(Graphics graphics);
};
	
myText::myText(int x, int y, Color fill, int fontSize, string text)
{
	this->x = x;
	this->y = y;
	this->fill = fill;
	this->fontSize = fontSize;
	this->text = text;
}
=======
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
>>>>>>> master

<<<<<<< HEAD
	void render(Graphics graphics) override
	{
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
		SolidBrush fillDraw(this->stroke);
		PointF pointF(this->x1, this->y1);
		std::wstring wtext(this->text.begin(), this->text.end());
		graphics.DrawString(wtext.c_str(), -1, &font, pointF, &fillDraw);
	}
};
||||||| 2de2cbb
void myText::Draw(Graphics graphics)
{
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
    SolidBrush fillDraw(this->fill);
    PointF pointF(this->x, this->y);
    std::wstring wtext(this->text.begin(), this->text.end());
    graphics.DrawString(wtext.c_str(), -1, &font, pointF, &fillDraw);
}

myText::~myText()
{
}
=======
void myText::Draw(Graphics graphics) {
  FontFamily fontFamily(L"Arial");
  Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
  SolidBrush fillDraw(this->fill);
  PointF pointF(this->x, this->y);
  std::wstring wtext(this->text.begin(), this->text.end());
  graphics.DrawString(wtext.c_str(), -1, &font, pointF, &fillDraw);
}

myText::~myText() {}
>>>>>>> master
