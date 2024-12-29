#ifndef PATH_H
#define PATH_H

#include "SVGElement.h"
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include <tuple>
#include <vector>

#define M_PI 3.14159265358979323846

using namespace Gdiplus;
using namespace std;

class my_path : public SVGElement {
private:
  vector<tuple<char, vector<PointF>>> d;
  string fill_color;
  double fill_opacity;
  Color stroke_color;
  double stroke_width;
  string transform;

public:
  my_path(string name, string transform, vector<tuple<char, vector<PointF>>> d,
          string fill_color, double, Color stroke_color, double stroke_width);
  void render(Graphics &graphics, vector<LinearGradient> gradients) override;
  string getTransform() override { return transform; }
};

my_path::my_path(string name, string transform,
                 vector<tuple<char, vector<PointF>>> d, string fill_color,
                 double fill_opacity, Color stroke_color,
                 double stroke_width = 0)
    : d(d), fill_color(fill_color), stroke_color(stroke_color),
      stroke_width(stroke_width), fill_opacity(fill_opacity), SVGElement(name),
      transform(transform) {}

void my_path::render(Graphics &graphics, vector<LinearGradient> gradients) {
  if (fill_color[0] == 'u' && fill_color[1] == 'r' && fill_color[2] == 'l') {
    string id = extractID(fill_color);
    LinearGradientBrush *brush = nullptr;
    for (auto gradient : gradients) {
      if (gradient.getId() == id) {
        brush = gradient.createBrush();
        break;
      }
    }

    GraphicsPath path;

    PointF endPoint = PointF(0, 0);
    PointF startPoint;

    int length = d.size();
    for (int i = 0; i < length; i++) {
      switch (get<0>(d[i])) {
      case 'M': {
        int numPoints = get<1>(d[i]).size();
        startPoint = get<1>(d[i])[0];
        path.StartFigure();
        endPoint = get<1>(d[i])[0];
        for (int j = 1; j < numPoints; j++) {
          path.AddLine(endPoint, get<1>(d[i])[j]);
          endPoint = get<1>(d[i])[j];
        }
        break;
      }
      case 'm': {
        int numPoints = get<1>(d[i]).size();
        startPoint = endPoint + get<1>(d[i])[0];
        path.StartFigure();
        endPoint = startPoint;
        for (int j = 1; j < numPoints; j++) {
          PointF newPoint = endPoint + get<1>(d[i])[j];
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'A': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 5) {
          // Lấy các thông số
          double rx = get<1>(d[i])[j].X;                // rx
          double ry = get<1>(d[i])[j].Y;                // ry
          double xAxisRotation = get<1>(d[i])[j + 1].X; // x-axis-rotation
          bool largeArc =
              static_cast<bool>(get<1>(d[i])[j + 2].X); // large-arc-flag
          bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X); // sweep-flag
          PointF startPoint = endPoint;                          // Điểm bắt đầu
          endPoint = get<1>(d[i])[j + 4]; // Điểm kết thúc (absolute)

          // Tính toán bounding box của elip
          REAL x = startPoint.X - rx; // X tọa độ góc trên-trái của elip
          REAL y = startPoint.Y - ry; // Y tọa độ góc trên-trái của elip
          REAL width = 2 * rx;        // Chiều rộng của elip
          REAL height = 2 * ry;       // Chiều cao của elip

          // Tính góc bắt đầu (startAngle) và góc quét (sweepAngle)
          REAL startAngle =
              atan2(-ry, rx) * 180 / M_PI; // Góc từ trục x đến điểm bắt đầu
          REAL sweepAngle =
              sweep ? 180 : -180; // Sweep clockwise hoặc counter-clockwise
          if (largeArc) {
            sweepAngle =
                sweep ? 360 - abs(sweepAngle) : -(360 - abs(sweepAngle));
          }

          // Tạo ma trận xoay để áp dụng xAxisRotation
          Matrix matrix;
          matrix.RotateAt(xAxisRotation, PointF(startPoint.X, startPoint.Y));

          // Thêm cung vào GraphicsPath sử dụng 6 tham số
          path.AddArc(x, y, width, height, startAngle, sweepAngle);
          path.Transform(&matrix);
        }
        break;
      }
      case 'a': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 5) {
          // Lấy các thông số
          double rx = get<1>(d[i])[j].X;                // rx
          double ry = get<1>(d[i])[j].Y;                // ry
          double xAxisRotation = get<1>(d[i])[j + 1].X; // x-axis-rotation
          bool largeArc =
              static_cast<bool>(get<1>(d[i])[j + 2].X); // large-arc-flag
          bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X); // sweep-flag
          PointF startPoint = endPoint;                          // Điểm bắt đầu
          PointF relativeEndPoint =
              get<1>(d[i])[j + 4]; // Điểm kết thúc (relative)

          // Tính toán điểm kết thúc tuyệt đối từ điểm bắt đầu
          PointF absoluteEndPoint = PointF(startPoint.X + relativeEndPoint.X,
                                           startPoint.Y + relativeEndPoint.Y);

          // Tính toán bounding box của elip
          REAL x = startPoint.X - rx;
          REAL y = startPoint.Y - ry;
          REAL width = 2 * rx;
          REAL height = 2 * ry;

          // Tính góc bắt đầu (startAngle) và góc quét (sweepAngle)
          REAL startAngle =
              atan2(-ry, rx) * 180 / M_PI; // Góc từ trục x đến điểm bắt đầu
          REAL sweepAngle = sweep ? 180 : -180;

          if (largeArc) {
            sweepAngle =
                sweep ? 360 - abs(sweepAngle) : -(360 - abs(sweepAngle));
          }

          // Tạo ma trận xoay để áp dụng xAxisRotation
          Matrix matrix;
          matrix.RotateAt(xAxisRotation, PointF(startPoint.X, startPoint.Y));

          // Thêm cung vào GraphicsPath sử dụng 6 tham số
          path.AddArc(x, y, width, height, startAngle, sweepAngle);
          path.Transform(&matrix);

          // Cập nhật điểm kết thúc
          endPoint = absoluteEndPoint;
        }
        break;
      }
      case 'L': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          path.AddLine(endPoint, get<1>(d[i])[j]);
          endPoint = get<1>(d[i])[j];
        }
        break;
      }
      case 'l': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = endPoint + get<1>(d[i])[j];
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'C': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 3) {
          PointF p1 = get<1>(d[i])[j];
          PointF p2 = get<1>(d[i])[j + 1];
          PointF p3 = get<1>(d[i])[j + 2];
          path.AddBezier(endPoint, p1, p2, p3);
          endPoint = p3;
        }
        break;
      }
      case 'c': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 3) {
          PointF p1 = get<1>(d[i])[j] + endPoint;
          PointF p2 = get<1>(d[i])[j + 1] + endPoint;
          PointF p3 = get<1>(d[i])[j + 2] + endPoint;
          path.AddBezier(endPoint, p1, p2, p3);
          endPoint = p3;
        }
        break;
      }
      case 'H': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(get<1>(d[i])[j].X, endPoint.Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'h': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X + get<1>(d[i])[j].X, endPoint.Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'V': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X, get<1>(d[i])[j].Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'v': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X, endPoint.Y + get<1>(d[i])[j].Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'Z':
      case 'z': {
        path.AddLine(endPoint, startPoint);
        endPoint = startPoint;
        path.CloseFigure();
        break;
      }
      case 'Q': {
        path.AddBezier(endPoint, get<1>(d[i])[0], get<1>(d[i])[1],
                       get<1>(d[i])[1]);
        endPoint = get<1>(d[i])[1];
        break;
      }
      case 'q': {
        for (int count = 0; count < get<1>(d[i]).size(); count += 2) {
          PointF p1 = get<1>(d[i])[count] + endPoint;
          PointF p2 = get<1>(d[i])[count + 1] + endPoint;
          path.AddBezier(endPoint, p1, p2, p2);
          endPoint = p2;
        }
        break;
      }
      case 'S': {
        path.AddBezier(endPoint, endPoint, get<1>(d[i])[0], get<1>(d[i])[1]);
        endPoint = get<1>(d[i])[1];
        for (int count = 2; count < get<1>(d[i]).size(); count += 2) {
          PointF p1 = get<1>(d[i])[count];
          PointF p2 = get<1>(d[i])[count + 1];
          path.AddBezier(endPoint, endPoint, p1, p2);
          endPoint = p2;
        }

        break;
      }
      case 's': {
        for (int count = 0; count < get<1>(d[i]).size(); count += 2) {
          PointF p1 = get<1>(d[i])[count] + endPoint;
          PointF p2 = get<1>(d[i])[count + 1] + endPoint;
          path.AddBezier(endPoint, endPoint, p1, p2);
          endPoint = p2;
        }
        break;
      }
      default:
        break;
      }
    }

    if (stroke_width > 0) {
      Pen pen(stroke_color, stroke_width);
      graphics.DrawPath(&pen, &path);
    }

    graphics.FillPath(brush, &path);
    delete brush;
    return;
  } else {
    Color fill = stoc(this->fill_color);
    Color fill_colorr =
        Color(fill_opacity * 255, fill.GetR(), fill.GetG(), fill.GetB());
    SolidBrush brushh(fill_colorr);
    GraphicsPath path;

    PointF endPoint = PointF(0, 0);
    PointF startPoint;

    int length = d.size();
    for (int i = 0; i < length; i++) {
      switch (get<0>(d[i])) {
      case 'M': {
        int numPoints = get<1>(d[i]).size();
        startPoint = get<1>(d[i])[0];
        path.StartFigure();
        endPoint = get<1>(d[i])[0];
        for (int j = 1; j < numPoints; j++) {
          path.AddLine(endPoint, get<1>(d[i])[j]);
          endPoint = get<1>(d[i])[j];
        }
        break;
      }
      case 'm': {
        int numPoints = get<1>(d[i]).size();
        startPoint = endPoint + get<1>(d[i])[0];
        path.StartFigure();
        endPoint = startPoint;
        for (int j = 1; j < numPoints; j++) {
          PointF newPoint = endPoint + get<1>(d[i])[j];
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'A': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 5) {
          double rx = get<1>(d[i])[j].X;
          double ry = get<1>(d[i])[j].Y;
          double xAxisRotation = get<1>(d[i])[j + 1].X;
          bool largeArc = static_cast<bool>(get<1>(d[i])[j + 2].X);
          bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X);
          PointF startPoint = endPoint;
          endPoint = get<1>(d[i])[j + 4];
        }
        break;
      }
      case 'a': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 5) {
          double rx = get<1>(d[i])[j].X;
          double ry = get<1>(d[i])[j].Y;
          double xAxisRotation = get<1>(d[i])[j + 1].X;
          bool largeArc = static_cast<bool>(get<1>(d[i])[j + 2].X);
          bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X);
          PointF startPoint = endPoint;
          endPoint = get<1>(d[i])[j + 4];
        }
        break;
      }
      case 'L': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          path.AddLine(endPoint, get<1>(d[i])[j]);
          endPoint = get<1>(d[i])[j];
        }
        break;
      }
      case 'l': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = endPoint + get<1>(d[i])[j];
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'C': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 3) {
          PointF p1 = get<1>(d[i])[j];
          PointF p2 = get<1>(d[i])[j + 1];
          PointF p3 = get<1>(d[i])[j + 2];
          path.AddBezier(endPoint, p1, p2, p3);
          endPoint = p3;
        }
        break;
      }
      case 'c': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j += 3) {
          PointF p1 = get<1>(d[i])[j] + endPoint;
          PointF p2 = get<1>(d[i])[j + 1] + endPoint;
          PointF p3 = get<1>(d[i])[j + 2] + endPoint;
          path.AddBezier(endPoint, p1, p2, p3);
          endPoint = p3;
        }
        break;
      }
      case 'H': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(get<1>(d[i])[j].X, endPoint.Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'h': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X + get<1>(d[i])[j].X, endPoint.Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'V': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X, get<1>(d[i])[j].Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'v': {
        int numPoints = get<1>(d[i]).size();
        for (int j = 0; j < numPoints; j++) {
          PointF newPoint = PointF(endPoint.X, endPoint.Y + get<1>(d[i])[j].Y);
          path.AddLine(endPoint, newPoint);
          endPoint = newPoint;
        }
        break;
      }
      case 'Z':
      case 'z': {
        path.AddLine(endPoint, startPoint);
        endPoint = startPoint;
        path.CloseFigure();
        break;
      }
      case 'Q': {
        path.AddBezier(endPoint, get<1>(d[i])[0], get<1>(d[i])[1],
                       get<1>(d[i])[1]);
        endPoint = get<1>(d[i])[1];
        break;
      }
      case 'q': {
        for (int count = 0; count < get<1>(d[i]).size(); count += 2) {
          PointF p1 = get<1>(d[i])[count] + endPoint;
          PointF p2 = get<1>(d[i])[count + 1] + endPoint;
          path.AddBezier(endPoint, p1, p2, p2);
          endPoint = p2;
        }
        break;
      }
      case 'S': {
        path.AddBezier(endPoint, endPoint, get<1>(d[i])[0], get<1>(d[i])[1]);
        endPoint = get<1>(d[i])[1];
        break;
      }
      case 's': {
        for (int count = 0; count < get<1>(d[i]).size(); count += 2) {
          PointF p1 = get<1>(d[i])[count] + endPoint;
          PointF p2 = get<1>(d[i])[count + 1] + endPoint;
          path.AddBezier(endPoint, endPoint, p1, p2);
          endPoint = p2;
        }
        break;
      }
      default:
        break;
      }
    }

    if (stroke_width > 0) {
      Pen pen(stroke_color, stroke_width);
      graphics.DrawPath(&pen, &path);
    }

    graphics.FillPath(&brushh, &path);
  }
}

#endif
