
#ifndef PATH_H
#define PATH_H

#include "SVGElement.h"
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include <tuple>
#include <vector>

using namespace Gdiplus;
using namespace std;

//<path fill = "none" stroke = "rgb(255,0,0)" stroke - width = "5" d = "M100,200
//C100,100 250,100 250,200 C250,300 400,300 400,200" / >

class my_path : public SVGElement {
private:
  vector<tuple<char, vector<PointF>>> d;
  Color fill_color;
  double fill_opacity;
  Color stroke_color;
  double stroke_width;
  string transform;

public:
  my_path(string name, string transform, vector<tuple<char, vector<PointF>>> d,
          Color fill_color, double, Color stroke_color, double stroke_width);
  void render(Graphics &graphics) override;
  string getTransform() override { return transform; }
};

my_path::my_path(string name, string transform,
                 vector<tuple<char, vector<PointF>>> d, Color fill_color,
                 double fill_opacity, Color stroke_color,
                 double stroke_width = 0)
    : d(d), fill_color(fill_color), stroke_color(stroke_color),
      stroke_width(stroke_width), fill_opacity(fill_opacity), SVGElement(name),
      transform(transform) {}

// M L H V C Z
void my_path::render(Graphics &graphics) {
  GraphicsPath path;
  Color fill_colorr = Color(fill_opacity * 255, fill_color.GetR(),
                            fill_color.GetG(), fill_color.GetB());
  SolidBrush brush(fill_colorr);
  PointF endPoint = PointF(0, 0);
  PointF startPoint;
  int length = d.size();
  for (int i = 0; i < length; i++) {
    switch (get<0>(d[i])) {
    case 'M':
      startPoint = get<1>(d[i])[0];
      path.StartFigure();
      endPoint = get<1>(d[i])[0];
      if (get<1>(d[i]).size() > 1) {
        path.AddLine(endPoint, get<1>(d[i])[1]);
        endPoint = get<1>(d[i])[1];
      }
      break;
    case 'm': {
      startPoint = get<1>(d[i])[0];
      path.StartFigure();
      endPoint = get<1>(d[i])[0];
      int count = 1;
      int lengthh = get<1>(d[i]).size();
      while (count < lengthh) {
        path.AddLine(endPoint, get<1>(d[i])[count] + endPoint);
        endPoint = get<1>(d[i])[count] + endPoint;
        count++;
      }
      break;
    }
    case 'C':
      path.AddBezier(endPoint, get<1>(d[i])[0], get<1>(d[i])[1],
                     get<1>(d[i])[2]);
      endPoint = get<1>(d[i])[2];
      break;
    case 'c': {
      int lengthh = get<1>(d[i]).size();
      for (int count = 0; count < lengthh; count += 3) {
        PointF p1 = get<1>(d[i])[count] + endPoint;
        PointF p2 = get<1>(d[i])[count + 1] + endPoint;
        PointF p3 = get<1>(d[i])[count + 2] + endPoint;
        path.AddBezier(endPoint, p1, p2, p3);
        endPoint = p3;
      }
      break;
    }
    case 'L':
      path.AddLine(endPoint, get<1>(d[i])[0]);
      endPoint = get<1>(d[i])[0];
      break;
    case 'l':
      path.AddLine(endPoint, get<1>(d[i])[0] + endPoint);
      endPoint = endPoint + get<1>(d[i])[0];
      break;
    case 'H':
      path.AddLine(endPoint, PointF(get<1>(d[i])[0].X, endPoint.Y));
      endPoint = PointF(get<1>(d[i])[0].X, endPoint.Y);
      break;
    case 'h':
      path.AddLine(endPoint,
                   PointF(endPoint.X + get<1>(d[i])[0].X, endPoint.Y));
      endPoint = PointF(endPoint.X + get<1>(d[i])[0].X, endPoint.Y);
      break;
    case 'V':
      path.AddLine(endPoint, PointF(endPoint.X, get<1>(d[i])[0].Y));
      endPoint = PointF(endPoint.X, get<1>(d[i])[0].Y);
      break;
    case 'v':
      path.AddLine(endPoint,
                   PointF(endPoint.X, endPoint.Y + get<1>(d[i])[0].Y));
      endPoint = PointF(endPoint.X, endPoint.Y + get<1>(d[i])[0].Y);
      break;
    case 'z':
    case 'Z':
      path.AddLine(endPoint, startPoint);
      endPoint = startPoint;
      path.CloseFigure();
      break;
    default:
      break;
    }
  }
  graphics.FillPath(&brush, &path);
  if (stroke_width != 0) {
    Pen pen(stroke_color, stroke_width);
    graphics.DrawPath(&pen, &path);
  }
}

#endif
