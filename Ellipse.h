#ifndef MY_ELLIPSE_H
#define MY_ELLIPSE_H

#include "SVGElement.h"

class my_ellipse : public SVGElement
{
	private:
		int cx, cy, rx, ry;
		double stroke_width, stroke_opacity, fill_opacity;
		Color stroke, fill;
		string transform;
	public:
		my_ellipse(string name, string transform, int cx = 0, int cy = 0, int rx = 1, int ry = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 0) : cx(cx), cy(cy), rx(rx), ry(ry), stroke(stroke), fill(fill), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

		void render(Graphics& graphic) override
		{
			Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
			Color fill_color(255 * this->fill_opacity, this->fill.GetR(), this->fill.GetG(), this->fill.GetB());
			Pen pen(stroke_color, this->stroke_width                                                                                                                   );
			SolidBrush brush(fill_color);

			graphic.FillEllipse(&brush, this->cx - this->rx, this->cy - this->ry, this->rx * 2, this->ry * 2);
			graphic.DrawEllipse(&pen, this->cx - this->rx, this->cy - this->ry, this->rx * 2, this->ry * 2);
		}

		~my_ellipse() override {

		}

		string getTransform() override
		{
			return this->transform;
		}
};

#endif