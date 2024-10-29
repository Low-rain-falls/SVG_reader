#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

//<polyline stroke = "rgb(255, 0, 0)" stroke - width = "2" stroke - opacity = "0.7" fill = "rgb(0, 255, 255)" fill - opacity = "0.5" points = "5,37 15,37 15,32 25,32 25,37 35,37 35,25 45,25 45,37 55,37 55,17 65,17 65,37 75,37 75,10 85,10 85,37 95,37 95,2 105,2 105,37 115,37" / >
//<polyline points = "0,40 40,40 40,80 80,80 80,120 120,120 120,140" fill - opacity = "0.5" / >

class myPolyline {
private:
	int* points;
	int numPoints;
	Color stroke;
	int strokeWidth;
	Color fill;
	float strokeOpacity;
	float fillOpacity;
public:
	myPolyline(int*, int, Color, int, float, Color, float);
	~myPolyline();
	void Draw(Graphics graphics);
};

myPolyline::myPolyline(int* points, int numPoints, Color stroke, int strokeWidth, float strokeOpacity, Color fill, float fillOpacity) {
	this->points = new int[numPoints];
	for (int i = 0; i < numPoints; i++) {
		this->points[i] = points[i];
	}
	this->numPoints = numPoints;
	this->stroke = stroke;
	this->strokeWidth = strokeWidth;
	this->strokeOpacity = strokeOpacity;
	this->fill = fill;
	this->fillOpacity = fillOpacity;
}

void myPolyline::Draw(Graphics graphics) {
	Color strokeColor(255 * this->strokeOpacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
	Pen strokeDraw(strokeColor, this->strokeWidth);
	SolidBrush fillDraw(Color(this->fillOpacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
	Point* pointArray = new Point[this->numPoints / 2];
	for (int i = 0; i < this->numPoints; i += 2) {
		pointArray[i / 2].X = this->points[i];
		pointArray[i / 2].Y = this->points[i + 1];
	}
	graphics.DrawLines(&strokeDraw, pointArray, this->numPoints / 2);
	graphics.FillPolygon(&fillDraw, pointArray, this->numPoints / 2);
	delete[] pointArray;
}

myPolyline::~myPolyline() {
	delete[] this->points;
}