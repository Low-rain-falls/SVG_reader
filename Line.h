#ifndef MY_LINE_H
#define My_LINE_H

#include "SVGElement.h"

class my_line : public SVGElement {
private:
  double x1, x2, y1, y2;
  Color stroke;
  string fill;
  double stroke_width, stroke_opacity, fill_opacity;
  string transform;

public:
  my_line(string name, string transform, double x1 = 0, double y1 = 0,
          double x2 = 0, double y2 = 0, Color stroke = Color(0, 0, 0),
          double stroke_width = 1, double stroke_opacity = 1,
          string fill = "rgb(0, 0, 0)", double fill_opacity = 1)
      : x1(x1), x2(x2), y1(y1), y2(y2), fill(fill), stroke(stroke),
        stroke_width(stroke_width), stroke_opacity(stroke_opacity),
        fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

  void render(Graphics &graphics, vector<LinearGradient> gradients) override {
    if (fill[0] == 'u' && fill[1] == 'r' && fill[2] == 'l') {
      string id = extractID(fill);
      LinearGradientBrush *brush = nullptr;
      for (auto gradient : gradients) {
        if (gradient.getId() == id) {
          brush = gradient.createBrush();
          break;
        }
      }
      Pen pen(brush, this->stroke_width);
      graphics.DrawLine(
          &pen, static_cast<REAL>(this->x1), static_cast<REAL>(this->y1),
          static_cast<REAL>(this->x2), static_cast<REAL>(this->y2));
      return;
    }
    Color stroke_color(255 * this->stroke_opacity, this->stroke.GetR(),
                       this->stroke.GetG(), this->stroke.GetB());
    Pen pen(stroke_color, this->stroke_width);
    graphics.DrawLine(&pen, static_cast<REAL>(this->x1),
                      static_cast<REAL>(this->y1), static_cast<REAL>(this->x2),
                      static_cast<REAL>(this->y2));
  }
  ~my_line() override {}
  string getTransform() override { return this->transform; }
};

#endif
