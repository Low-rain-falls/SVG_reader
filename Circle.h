#pragma once

#include "SVGElement.h"
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;

class my_circle : public SVGElement
{
public:
	my_circle(int cx = 0, int cy = 0, int r = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : SVGElement(cx, cy, r, r, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
  
	void render(Graphics graphic) override
	{
		Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Color fillColor(255 * this->fill_opacity, this->fill.GetR(), this->fill.GetG(), this->fill.GetB());
		Pen pen(strokeColor, this->stroke_width);
		SolidBrush brush(fillColor);
		graphic.FillEllipse(&brush, this->x1 - this->width, this->y1 - this->width, this->width * 2, this->width * 2);
		graphic.DrawEllipse(&pen, this->x1 - this->width, this->y1 - this->width, this->width * 2, this->width * 2);
	}
};
