#ifndef MY_RECT_H
#define MY_RECT_H

#include "SVGElement.h"

class my_rect : public SVGElement
{
	private:
		int x, y, width, height;
		Color stroke, fill;
		double stroke_width, stroke_opacity, fill_opacity;
public:
	my_rect(int x = 0, int y = 0, int width = 1, int height = 1, Color stroke = Color(0,0,0), double stroke_width = 1, double stroke_opacity = 1, Color fill = Color(0, 0, 0), double fill_opacity = 1) : x(x), y(y), width(width), height(height), stroke(stroke), fill(fill), stroke_width(stroke_width), stroke_opacity(stroke_opacity), fill_opacity(fill_opacity) {}

	void setTransform(const string& content) override {
		this->t.parseTransform(content);
	}

	void render(Graphics& graphics) override
	{
		this->t.applyTransform(graphics);
		Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(), this->stroke.GetG(), this->stroke.GetB());
		Pen pen(stroke_color, this->stroke_width);
		SolidBrush brush(Color(this->fill_opacity* 255, this->fill.GetR(), this->fill.GetG(), this->fill.GetB()));
		graphics.FillRectangle(&brush, this->x, this->y, this->width, this->height);
		graphics.DrawRectangle(&pen, this->x, this->y, this->width, this->height);
		this->t.resetTransform(graphics);
	}

	~my_rect() override {
		
	}
};

#endif