#ifndef MY_CIRCLE_H
#define MY_CIRCLE_H

#include "SVGElement.h"

class my_circle : public SVGElement
{
	private:
		int cx, cy, r;
		double stroke_width, stroke_opacity, fill_opacity;
		Color fill, stroke;
	public:
		my_circle(int cx = 0, int cy = 0, int r = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : cx(cx), cy(cy), r(r), stroke(stroke), fill(fill), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity) {}
		
		void setTransform(const string& content) override {
			this->t.parseTransform(content);
		}

		void render(Graphics& graphic) override
		{
			this->t.applyTransform(graphic);
			Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
			Color fillColor(255 * this->fill_opacity, this->fill.GetR(), this->fill.GetG(), this->fill.GetB());
			Pen pen(strokeColor, this->stroke_width);
			SolidBrush brush(fillColor);
			graphic.FillEllipse(&brush, this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
			graphic.DrawEllipse(&pen, this->cx - this->r, this->cy - this->r, this->r * 2, this->r * 2);
			this->t.resetTransform(graphic);
		}

		~my_circle() override {

		}
};

#endif