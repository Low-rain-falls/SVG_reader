#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include "SVGElement.h"

using namespace Gdiplus;

//<line x1 = "10" y1 = "30" x2 = "30" y2 = "10" stroke = "rgb(0, 0, 255)" stroke
//- width = "5" stroke - opacity = "0.7" / > <line x1 = "30" y1 = "30" x2 = "50"
// y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "10" stroke - opacity =
//"0.8" / > <line x1 = "50" y1 = "30" x2 = "70" y2 = "10" stroke = "rgb(0, 0,
// 255)" stroke - width = "15" stroke - opacity = "0.9" / > <line x1 = "70" y1 =
//"30" x2 = "90" y2 = "10" stroke = "rgb(0, 0, 255)" stroke - width = "20"
// stroke - opacity = "0.9" / > <line x1 = "90" y1 = "30" x2 = "110" y2 = "10"
// stroke = "rgb(0, 0, 255)" stroke - width = "25" stroke - opacity = "1.0" / >

class my_line : public SVGElement
{
public:
	my_line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, Color stroke = Color(0,0,0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0,0,0), double fill_opacity = 1) : SVGElement(x1, y1, x2, y2, stroke, stroke_width, stroke_opacity, fill, fill_opacity) {}
	void render(Graphics graphics) override
	{
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Pen pen(stroke_color, this->stroke_width);
		graphics.DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
	}
};
