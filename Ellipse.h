#ifndef MY_ELLIPSE_H
#define MY_ELLIPSE_H

#include "SVGElement.h"

class my_ellipse : public SVGElement {
private:
	double cx, cy, rx, ry;
	double stroke_width, stroke_opacity, fill_opacity;
	Color stroke;
	string fill;
	string transform;

public:
	my_ellipse(string name, string transform, double cx = 0, double cy = 0, double rx = 1,
		double ry = 1, Color stroke = Color(0, 0, 0), double stroke_width = 1,
		double stroke_opacity = 1, string fill = "rgb(0, 0, 0)",
		double fill_opacity = 0)
		: cx(cx), cy(cy), rx(rx), ry(ry), stroke(stroke), fill(fill),
		stroke_width(stroke_width), stroke_opacity(stroke_opacity),
		fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

	void render(Graphics& graphic, vector<LinearGradient> gradients) override {
		if (fill[0] == 'u' && fill[1] == 'r' && fill[2] == 'l') {
			Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
			Pen pen(strokeColor, this->stroke_width);
			string id = extractID(fill);
			LinearGradientBrush* brush = nullptr;
			for (auto gradient : gradients) {
				if (gradient.getId() == id) {
					brush = gradient.createBrush();
					break;
				}
			}
			graphic.FillEllipse(brush, static_cast<REAL>(this->cx - this->rx), static_cast<REAL>(this->cy - this->ry),
				static_cast<REAL>(this->rx * 2), static_cast<REAL>(this->ry * 2));
			graphic.DrawEllipse(&pen, static_cast<REAL>(this->cx - this->rx), static_cast<REAL>(this->cy - this->ry),
				static_cast<REAL>(this->rx * 2), static_cast<REAL>(this->ry * 2)); 
			return;
		}
		Color filll = stoc(this->fill);
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(),
			this->stroke.GetG(), this->stroke.GetB());
		Color fill_color(255 * this->fill_opacity, filll.GetR(),
			filll.GetG(), filll.GetB());
		Pen pen(stroke_color, this->stroke_width);
		SolidBrush brush(fill_color);

		graphic.FillEllipse(&brush, static_cast<REAL>(this->cx - this->rx), static_cast<REAL>(this->cy - this->ry),
			static_cast<REAL>(this->rx * 2), static_cast<REAL>(this->ry * 2));
		graphic.DrawEllipse(&pen, static_cast<REAL>(this->cx - this->rx), static_cast<REAL>(this->cy - this->ry),
			static_cast<REAL>(this->rx * 2), static_cast<REAL>(this->ry * 2));

	}

	~my_ellipse() override {}

	string getTransform() override { return this->transform; }
};

#endif
