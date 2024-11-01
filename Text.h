#pragma once

#include "stdafx.h"
#include <gdiplus.h>
#include <iostream>
#include <objidl.h>
#include "SVGElement.h"

using namespace std;
using namespace Gdiplus;

// <text x="0" y="0" fill="rgb(0,0,255)" font-size="40">SVG Demo</text>
// <text x="400" y="400" fill="rgb(255,0,255)" font-size="30">Nguyen Van A</text>

class my_text : public SVGElement
{
public:
	my_text(int x, int y, Color stroke, int font_size, string value) : SVGElement(x, y, stroke, font_size, value) {}

	void render(Graphics graphics) override
	{
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, this->fontSize, FontStyleRegular, UnitPixel);
		SolidBrush fillDraw(this->stroke);
		PointF pointF(this->x1, this->y1);
		std::wstring wtext(this->text.begin(), this->text.end());
		graphics.DrawString(wtext.c_str(), -1, &font, pointF, &fillDraw);
	}
};