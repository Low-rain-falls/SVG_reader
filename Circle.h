#ifndef MY_CIRCLE_H
#define MY_CIRCLE_H

#include "SVGElement.h"

class my_circle : public SVGElement
{
	private:
		int cx, cy, r;
		double stroke_width, stroke_opacity, fill_opacity;
		Color fill, stroke;
		string transform;
	public:
		my_circle(string name, string transform, int cx = 0, int cy = 0, int r = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 0) : cx(cx), cy(cy), r(r), stroke(stroke), fill(fill), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}
		
		void render(Graphics& graphic) override
		{
			if ((this->fill.GetR() != 255 && this->fill.GetG() != 255 && this->fill.GetB() != 255)) {
				Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
				Color fillColor(255 * this->fill_opacity, this->fill.GetR(), this->fill.GetG(), this->fill.GetB());
				Pen pen(strokeColor, this->stroke_width);
				SolidBrush brush(fillColor);
				graphic.FillEllipse(&brush, this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
				graphic.DrawEllipse(&pen, this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
			}
			else {
				Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
				Pen pen(strokeColor, this->stroke_width);
				graphic.DrawEllipse(&pen, this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
			}
		}

		~my_circle() override {

		}

		string getTransform() override {
			return transform;
		}
};

#endif