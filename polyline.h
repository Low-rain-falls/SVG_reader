#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include "SVGElement.h"

using namespace Gdiplus;

//<polyline stroke = "rgb(255, 0, 0)" stroke - width = "2" stroke - opacity = "0.7" fill = "rgb(0, 255, 255)" fill - opacity = "0.5" points = "5,37 15,37 15,32 25,32 25,37 35,37 35,25 45,25 45,37 55,37 55,17 65,17 65,37 75,37 75,10 85,10 85,37 95,37 95,2 105,2 105,37 115,37" / >
//<polyline points = "0,40 40,40 40,80 80,80 80,120 120,120 120,140" fill - opacity = "0.5" / >

class my_polyline : public SVGElement
{
public:
	my_polyline(int* points = nullptr, int num_points = 0, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : SVGElement(points, num_points, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
	void render(Graphics graphics) override
	{
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Pen pen(stroke_color, this->stroke_width);
		SolidBrush brush(Color(this->fill_opacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
		Point* point_array = new Point[this->num_points / 2];
		for (int i = 0; i < this->num_points; i += 2) {
			point_array[i / 2].X = this->points[i];
			point_array[i / 2].Y = this->points[i + 1];
		}
		graphics.DrawLines(&pen, point_array, this->num_points / 2);
		graphics.FillPolygon(&brush, point_array, this->num_points / 2);
		delete[] point_array;
	}
};
