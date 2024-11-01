#pragma once

#include "stdafx.h"
#include "gdiplus.h"
#include "objidl.h"
#include <iostream>

using namespace std;
using namespace Gdiplus;


class SVGElement
{
protected:
	// coordinates
	int x1, y1, x2, y2;

	// width and height or radius for circle and ellipse
	int width, height;

	// text
	int fontSize;
	string text;

	// polygon and polyline
	int* points;
	int num_points;

	// color fill
	Color fill;
	double fill_opacity;

	// color stroke
	Color stroke;
	double stroke_width;
	double stroke_opacity;
public:
	SVGElement(int x1, int y1, int width, int height, Color stroke, double stroke_width, double stroke_opacity, Color fill, double fill_opacity);
	SVGElement(int* points, int num_points, Color stroke, double stroke_width, double stroke_opacity, Color fill, double fill_opacity);
	SVGElement(int x, int y, Color stroke, int font_size, string value);

	virtual void render(Graphics graphic) = 0;
	~SVGElement();
};

SVGElement::SVGElement(int x1, int y1, int width, int height, Color stroke, double stroke_width, double stroke_opacity, Color fill, double fill_opacity)
{
	this->x1 = x1;
	this->y1 = y1;
	this->width = width;
	this->height = height;
	this->x2 = this->width;
	this->y2 = this->height;
	this->stroke = stroke;
	this->stroke_width = stroke_width;
	this->stroke_opacity = stroke_opacity;
	this->fill = fill;
	this->fill_opacity = fill_opacity;
	this->points = nullptr;
	this->num_points = 0;
	this->fontSize = 0;
	this->text = "";
}

SVGElement::SVGElement(int* points, int num_points, Color stroke, double stroke_width, double stroke_opacity, Color fill, double fill_opacity)
{
	this->points = new int[num_points];
	for (int i = 0; i < num_points; i++)
	{
		this->points[i] = points[i];
	}
	this->num_points = num_points;
	this->stroke = stroke;
	this->stroke_width = stroke_width;
	this->stroke_opacity = stroke_opacity;
	this->fill = fill;
	this->fill_opacity = fill_opacity;
	this->x1 = 0;
	this->y1 = 0;
	this->x2 = 0;
	this->y2 = 0;
	this->width = 0;
	this->height = 0;
	this->fontSize = 0;
	this->text = "";
}

SVGElement::SVGElement(int x, int y, Color stroke, int font_size, string value)
{
	this->x1 = x;
	this->y1 = y;
	this->stroke = stroke;
	this->stroke_width = stroke_width;
	this->fontSize = font_size;
	this->text = value;

	this->points = nullptr;
	this->num_points = 0;
	this->width = 0;
	this->height = 0;
	this->x2 = 0;
	this->y2 = 0;
	this->fill = Color::White;
	this->fill_opacity = 0;
	this->stroke_opacity = 1;

}

SVGElement::~SVGElement()
{
	if (points != nullptr)
	{
		delete[] points;
	}
}
