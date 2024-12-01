#ifndef MY_LINE_H
#define My_LINE_H

#include "SVGElement.h"

class my_line : public SVGElement
{
	private:
		int x1, x2, y1, y2;
		Color stroke, fill;
		double stroke_width, stroke_opacity, fill_opacity;
	public:
		my_line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, Color stroke = Color(0,0,0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0,0,0), double fill_opacity = 1) : x1(x1), x2(x2), y1(y1), y2(y2), fill(fill), stroke(stroke), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity) {}

		void setTransform(const string& content) override {
			this->t.parseTransform(content);
		}

		void render(Graphics& graphics) override
		{
			this->t.applyTransform(graphics);
			Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
			Pen pen(stroke_color, this->stroke_width);
			graphics.DrawLine(&pen, this->x1, this->y1, this->x2, this->y2);
			this->t.resetTransform(graphics);
		}
		~my_line() override {
			
		}
};

#endif