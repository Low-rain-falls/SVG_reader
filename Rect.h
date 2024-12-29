#ifndef MY_RECT_H
#define MY_RECT_H

#include "SVGElement.h"

class my_rect : public SVGElement {
private:
  double x, y, width, height;
  Color stroke;
  string fill;
  double stroke_width, stroke_opacity, fill_opacity;
  string transform;

public:
  my_rect(string name, string transform, double x = 0, double y = 0,
          double width = 1, double height = 1, Color stroke = Color(0, 0, 0),
          double stroke_width = 1, double stroke_opacity = 1,
          string fill = "rgb(0, 0, 0)", double fill_opacity = 1)
      : x(x), y(y), width(width), height(height), stroke(stroke), fill(fill),
        stroke_width(stroke_width), stroke_opacity(stroke_opacity),
        fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

  void render(Graphics &graphics, vector<LinearGradient> gradients) override {
    if (fill[0] == 'u' && fill[1] == 'r' && fill[2] == 'l') {
      Color strokeColor(255 * this->stroke_opacity, this->stroke.GetR(),
                        this->stroke.GetG(), this->stroke.GetB());
      Pen pen(strokeColor, this->stroke_width);
      string id = extractID(fill);
      LinearGradientBrush *brush = nullptr;
      for (auto gradient : gradients) {
        if (gradient.getId() == id) {
          brush = gradient.createBrush();
          break;
        }
      }
      graphics.FillRectangle(
          brush, static_cast<REAL>(this->x), static_cast<REAL>(this->y),
          static_cast<REAL>(this->width), static_cast<REAL>(this->height));
      graphics.DrawRectangle(
          &pen, static_cast<REAL>(this->x), static_cast<REAL>(this->y),
          static_cast<REAL>(this->width), static_cast<REAL>(this->height));
      return;
    }
    Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(),
                       this->stroke.GetG(), this->stroke.GetB());
    Pen pen(stroke_color, this->stroke_width);
    Color filll = stoc(fill);
    SolidBrush brush(Color(this->fill_opacity * 255, filll.GetR(), filll.GetG(),
                           filll.GetB()));
    graphics.FillRectangle(
        &brush, static_cast<REAL>(this->x), static_cast<REAL>(this->y),
        static_cast<REAL>(this->width), static_cast<REAL>(this->height));
    graphics.DrawRectangle(
        &pen, static_cast<REAL>(this->x), static_cast<REAL>(this->y),
        static_cast<REAL>(this->width), static_cast<REAL>(this->height));
  }

  ~my_rect() override {}

  string getTransform() override { return this->transform; }
};

#endif
