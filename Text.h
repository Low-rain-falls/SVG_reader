#ifndef MY_TEXT_H
#define MY_TEXT_H

#include "SVGElement.h"
#include <GdiPlus.h>
using namespace Gdiplus;

class my_text : public SVGElement {
private:
	double x, y;
	Color stroke;
	string fill;
	double font_size;
	string content;
	double stroke_width;
	string text_anchor;
	string font_style;
	string font_family;
	string transform;

public:
	my_text(string name, string transform, double x, double y, string fill, Color stroke,
		double stroke_width, int font_size, string font_style,
		string text_anchor, string font_family, string content)
		: x(x), y(y), stroke(stroke), font_size(font_size),
		font_style(font_style), text_anchor(text_anchor),
		font_family(font_family), content(content), fill(fill),
		stroke_width(stroke_width), SVGElement(name), transform(transform) {}
	void render(Graphics& graphics, vector<LinearGradient> gradients) override {
		//font - family = "Times New Roman,Times New Roman_MSFontService,sans-serif"
		// handle multiple font families
		string font_family_str = font_family;

		vector<wstring> font_families;
		int pos = font_family_str.find(',');
		if (pos != -1)
		{
			int pos = 0;
			while ((pos = font_family_str.find(",")) != string::npos) {
				font_families.push_back(wstring(font_family_str.begin(), font_family_str.begin() + pos));
				font_family_str.erase(0, pos + 1);
			}
		}
		else
		{
			font_families.push_back(wstring(font_family_str.begin(), font_family_str.end()));
		}
		FontFamily fontFamily(font_families[0].c_str());
		FontStyle style =
			(font_style == "italic") ? FontStyleItalic : FontStyleRegular;
		Font font(&fontFamily, static_cast<REAL>(font_size), style, UnitPixel);\
			Color filll = stoc(this->fill);
		SolidBrush brush(filll);
		RectF boundingBox;
		graphics.MeasureString(std::wstring(content.begin(), content.end()).c_str(),
			-1, &font, PointF(0, 0), &boundingBox);

		REAL text_width = boundingBox.Width;

		REAL adjusted_x = static_cast<REAL>(x);
		if (text_anchor == "middle") {
			adjusted_x -= text_width / 2;
		}
		else if (text_anchor == "end") {
			adjusted_x -= text_width;
		}
		else if (text_anchor == "start") {
			adjusted_x = static_cast<REAL>(x);
		}

		PointF pointF(static_cast<REAL>(adjusted_x), static_cast<REAL>(y));
		graphics.DrawString(std::wstring(content.begin(), content.end()).c_str(),
			-1, &font, pointF, &brush);
		if (stroke_width != 0) {
			Pen pen(stroke, stroke_width);
			GraphicsPath path;
			StringFormat format;
			path.AddString(std::wstring(content.begin(), content.end()).c_str(), -1,
				&fontFamily, style, static_cast<REAL>(font_size), pointF,
				&format);
			graphics.DrawPath(&pen, &path);
		}
	}

	~my_text() override {}

	string getTransform() override { return transform; }
};

#endif
