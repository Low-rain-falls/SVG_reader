#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

//<polygon fill = "rgb(153, 204, 255)" fill - opacity = "0.5" stroke - width = "10" stroke = "rgb(255, 0, 102)" stroke - opacity = "0.7" points = "850,75 958,137 958,262 850,325 742,262 742,137" / >
//<polygon fill = "rgb(255, 255, 0)" fill - opacity = "0.6" stroke - width = "10" stroke = "rgb(255, 0, 0)" stroke - opacity = "0.7" points = "350,75 379,161 469,161 397,215 423,301 350,250 277,301 303,215 231,161 321,161" / >

class myPolygon
{
private:
	Color fill;
	float fillOpacity;
	int strokeWidth;
	Color stroke;
	float strokeOpacity;
	int* points;
	int numPoints;
public:
	myPolygon(int* points, int numPoints, Color stroke, int strokeWidth, float strokeOpacity, Color fill, float fillOpacity);
	~myPolygon();
	void Draw(Graphics graphics);


};

myPolygon::myPolygon(int* points, int numPoints, Color stroke, int strokeWidth, float strokeOpacity, Color fill, float fillOpacity)
{
	this->fill = fill;
	this->fillOpacity = fillOpacity;
	this->strokeWidth = strokeWidth;
	this->stroke = stroke;
	this->strokeOpacity = strokeOpacity;
	this->numPoints = numPoints;
	this->points = new int[numPoints];
	for (int i = 0; i < numPoints; i++)
	{
		this->points[i] = points[i];
	}
}

void myPolygon::Draw(Graphics graphics)
{
	Pen strokeDraw(Color(this->strokeOpacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()), this->strokeWidth);
	SolidBrush fillDraw(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	Point* pointArray = new Point[this->numPoints / 2];
	for (int i = 0; i < this->numPoints; i += 2)
	{
		pointArray[i / 2].X = this->points[i];
		pointArray[i / 2].Y = this->points[i + 1];
	}
	graphics.FillPolygon(&fillDraw, pointArray, this->numPoints / 2);
	graphics.DrawPolygon(&strokeDraw, pointArray, this->numPoints / 2);
}

myPolygon::~myPolygon()
{
	delete[] points;
}