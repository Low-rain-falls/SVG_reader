#ifndef MY_TEXT_H
#define MY_TEXT_H

#include "SVGElement.h"
#include <GdiPlus.h>
using namespace Gdiplus;

class my_text : public SVGElement {
private:
  int x, y;
  Color stroke;
  Color fill;
  int font_size;
  string content;
  double stroke_width;
  string text_anchor;
  string font_style;
  string font_family;
  string transform;

public:
  my_text(string name, string transform, int x, int y, Color fill, Color stroke,
          double stroke_width = 0, int font_size, string font_style,
          string text_anchor, string font_family, string content)
      : x(x), y(y), stroke(stroke), font_size(font_size),
        font_style(font_style), text_anchor(text_anchor),
        font_family(font_family), content(content), fill(fill),
        stroke_width(stroke_width), SVGElement(name), transform(transform) {}

  void render(Graphics &graphics) override {
    FontFamily fontFamily(
        std::wstring(font_family.begin(), font_family.end()).c_str());
    FontStyle style =
        (font_style == "italic") ? FontStyleItalic : FontStyleRegular;
    Font font(&fontFamily, static_cast<REAL>(font_size), style, UnitPixel);
    SolidBrush brush(fill);

    RectF boundingBox;
    graphics.MeasureString(std::wstring(content.begin(), content.end()).c_str(),
                           -1, &font, PointF(0, 0), &boundingBox);

    REAL text_width = boundingBox.Width;

    REAL adjusted_x = static_cast<REAL>(x);
    if (text_anchor == "middle") {
      adjusted_x -= text_width / 2; // C?n gi?a
    } else if (text_anchor == "end") {
      adjusted_x -= text_width; // C?n ph?i
    }

    PointF pointF(static_cast<REAL>(adjusted_x), static_cast<REAL>(y));
    graphics.DrawString(std::wstring(content.begin(), content.end()).c_str(),
                        -1, &font, pointF, &brush);
    Pen pen(stroke, stroke_width);
    GraphicsPath path;
    StringFormat format;
    path.AddString(std::wstring(content.begin(), content.end()).c_str(), -1,
                   &fontFamily, style, static_cast<REAL>(font_size), pointF,
                   &format);
    graphics.DrawPath(&pen, &path);
  }

  ~my_text() override {}

  string getTransform() override { return transform; }
};

#endif
