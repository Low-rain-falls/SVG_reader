#ifndef MY_TEXT_H
#define MY_TEXT_H

#include "SVGElement.h"
#include <GdiPlus.h>
using namespace Gdiplus;

class my_text : public SVGElement
{
private:
	int x, y;
	Color stroke;
	Color fill;
	int font_size;
	string content;
	double stroke_width;
	string transform;
	

public:
	my_text(string name, string transform, int x, int y, Color fill, Color stroke, double stroke_width, int font_size, string content) : x(x), y(y), stroke(stroke), font_size(font_size), content(content), fill(fill), stroke_width(stroke_width), SVGElement(name), transform(transform) {}

	void render(Graphics& graphics) override
	{
		FontFamily fontFamily(L"Consolas");
		Font font(&fontFamily, static_cast<REAL>(font_size), FontStyleRegular, UnitPixel);
		SolidBrush brush(fill);
		PointF pointF(static_cast<REAL>(x), static_cast<REAL>(y));
		graphics.DrawString(std::wstring(content.begin(), content.end()).c_str(), -1, &font, pointF, &brush);
		Pen pen(stroke, stroke_width);
		GraphicsPath path;
		StringFormat format;
		path.AddString(std::wstring(content.begin(), content.end()).c_str(), -1, &fontFamily, FontStyleRegular, static_cast<REAL>(font_size), pointF, &format);
		graphics.DrawPath(&pen, &path);
	}

	~my_text() override {

	}

	string getTransform() override
	{
		return transform;
	}
};

#endif