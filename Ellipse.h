#pragma once

#include "SVGElement.h"
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

class my_ellipse : public SVGElement
{
public:
	my_ellipse(int cx = 0, int cy = 0, int rx = 1, int ry = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : SVGElement(cx, cy, rx, ry, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
	void render(Graphics graphic) override
	{
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Color fill_color(255 * this->fill_opacity, this->fill.GetR(), this->fill.GetG(), this->fill.GetB());

		Pen pen(stroke_color, this->stroke_width);
		SolidBrush brush(fill_color);

		graphic.FillEllipse(&brush, this->x1 - this->width, this->y1 - this->height, this->width * 2, this->height * 2);
		graphic.DrawEllipse(&pen, this->x1 - this->width, this->y1 - this->height, this->width * 2, this->height * 2);
	}
};