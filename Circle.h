#ifndef MY_CIRCLE_H
#define MY_CIRCLE_H

#include "SVGElement.h"

class my_circle : public SVGElement
{
private:
	double cx, cy, r;
	double stroke_width, stroke_opacity, fill_opacity;
	string fill;
	Color stroke;
	string transform;
public:
	my_circle(string name, string transform, double cx = 0, double cy = 0, double r = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, string fill = "rgb(0, 0, 0)", double fill_opacity = 0) : cx(cx), cy(cy), r(r), stroke(stroke), fill(fill), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

	void render(Graphics& graphic, vector<LinearGradient> gradients) override
	{
		Color filll = stoc(this->fill);
		Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Color fillColor(255 * this->fill_opacity, filll.GetR(), filll.GetG(), filll.GetB());
		Pen pen(strokeColor, this->stroke_width);
		SolidBrush brush(fillColor);
		graphic.FillEllipse(&brush, static_cast<REAL>(this->cx - this->r), static_cast<REAL>(this->cy - this->r), static_cast<REAL>(this->r * 2), static_cast<REAL>(this->r * 2));
		graphic.DrawEllipse(&pen, static_cast<REAL>(this->cx - this->r), static_cast<REAL>(this->cy - this->r), static_cast<REAL>(this->r * 2), static_cast<REAL>(this->r * 2));

	}

	~my_circle() override {

	}

	string getTransform() override {
		return transform;
	}
};

#endif
