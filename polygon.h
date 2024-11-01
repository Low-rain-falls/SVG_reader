#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include "SVGElement.h"
using namespace Gdiplus;

//<polygon fill = "rgb(153, 204, 255)" fill - opacity = "0.5" stroke - width = "10" stroke = "rgb(255, 0, 102)" stroke - opacity = "0.7" points = "850,75 958,137 958,262 850,325 742,262 742,137" / >
//<polygon fill = "rgb(255, 255, 0)" fill - opacity = "0.6" stroke - width = "10" stroke = "rgb(255, 0, 0)" stroke - opacity = "0.7" points = "350,75 379,161 469,161 397,215 423,301 350,250 277,301 303,215 231,161 321,161" / >

class my_polygon : public SVGElement
{
public:
	my_polygon(int* points = nullptr, int num_points = 0, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : SVGElement(points, num_points, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
	void render(Graphics graphics) override
	{
		Pen pen(Color(this->stroke_opacity * 255, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB()), this->stroke_width);
		SolidBrush brush(Color(this->fill_opacity * 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
		Point* point_array = new Point[this->num_points / 2];
		for (int i = 0; i < this->num_points; i += 2)
		{
			point_array[i / 2].X = this->points[i];
			point_array[i / 2].Y = this->points[i + 1];
		}
		graphics.FillPolygon(&brush, point_array, this->num_points / 2);
		graphics.DrawPolygon(&pen, point_array, this->num_points / 2);
		delete[] point_array;
	}
};