#pragma once

#include <objidl.h>
#include <gdiplus.h>
#include "stdafx.h"

using namespace Gdiplus;

//<line x1 = "10" y1 = "30" x2 = "30" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "5" stroke - opacity = "0.7" / >
//<line x1 = "30" y1 = "30" x2 = "50" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "10" stroke - opacity = "0.8" / >
//<line x1 = "50" y1 = "30" x2 = "70" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "15" stroke - opacity = "0.9" / >
//<line x1 = "70" y1 = "30" x2 = "90" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "20" stroke - opacity = "0.9" / >
//<line x1 = "90" y1 = "30" x2 = "110" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "25" stroke - opacity = "1.0" / >

class myLine
{
private:
	int x1;
	int y1;
	int x2;
	int y2;
	Color stroke;
	int strokeWidth;
public:
	myLine(int, int, int, int, Color, int, float);
	~myLine();
	void Draw(Graphics graphics);

};

myLine::myLine(int x1, int y1, int x2, int y2, Color stroke, int strokeWidth, float strokeOpacity)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	//this->stroke.SetValue(Color(strokeOpacity * 255, stroke.GetR(), stroke.GetG(), stroke.GetB()));
	this->stroke = Color(strokeOpacity * 255, stroke.GetR(), stroke.GetG(), stroke.GetB());	
	this->strokeWidth = strokeWidth;
}

void myLine::Draw(Graphics graphics)
{
	Pen strokeDraw(this->stroke, this->strokeWidth);
	graphics.DrawLine(&strokeDraw, this->x1, this->y1, this->x2, this->y2);
}

myLine::~myLine()
{
}