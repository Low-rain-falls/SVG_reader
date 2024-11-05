#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include "SVGElement.h"
using namespace Gdiplus;

class my_rect : public SVGElement
{
public:
	my_rect(int x = 0, int y = 0, int width = 1, int height = 1, Color stroke = Color(0,0,0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : SVGElement(x, y, width, height, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
	void render(Graphics graphics) override
	{
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Pen pen(stroke_color, this->stroke_width);
		SolidBrush brush(Color(this->fill_opacity* 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
		graphics.FillRectangle(&brush, this->x1, this->y1, this->width, this->height);
		graphics.DrawRectangle(&pen, this->x1, this->y1, this->width, this->height);
	}
};
