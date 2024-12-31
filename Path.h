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
	void render(Graphics& graphics, vector<LinearGradient> gradients) override;
	string getTransform() override { return transform; }
};

vector<PointF> centerEllipse(PointF p1, PointF p2, double rx, double ry) {
	double cx = 0;
	double cy = 0;
	double r1 = (p2.X - p1.X) / (2 * rx);
	double r2 = (p1.Y - p2.Y) / (2 * ry);
	double a1 = atan(r1 / r2);
	double a2 = asin(r1 / sin(a1));
	double t1 = a1 + a2;
	double t2 = a1 - a2;
	cx = p1.X - rx * cos(t1);
	cy = p1.Y - ry * sin(t1);
	double cx2 = 0;
	double cy2 = 0;
	double ix = (p1.X + p2.X) / 2;
	double iy = (p1.Y + p2.Y) / 2;
	cx2 = 2 * ix - cx;
	cy2 = 2 * iy - cy;
	vector<PointF> centerPoints;
	centerPoints.push_back(PointF(cx, cy));
	centerPoints.push_back(PointF(cx2, cy2));
	return centerPoints;
}

double realLength(PointF p1, PointF p2)
{
	return sqrt(pow(p2.X - p1.X, 2) + pow(p2.Y - p1.Y, 2));
}

my_path::my_path(string name, string transform,
	vector<tuple<char, vector<PointF>>> d, string fill_color,
	double fill_opacity, Color stroke_color,
	double stroke_width = 0)
	: d(d), fill_color(fill_color), stroke_color(stroke_color),
	stroke_width(stroke_width), fill_opacity(fill_opacity), SVGElement(name),
	transform(transform) {}

void my_path::render(Graphics& graphics, vector<LinearGradient> gradients) {
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
				double rx = get<1>(d[i])[j].X; // rx
				double ry = get<1>(d[i])[j].Y; // ry
				double xAxisRotation = get<1>(d[i])[j + 1].X; // x-axis-rotation
				bool largeArc = static_cast<bool>(get<1>(d[i])[j + 2].X); // large-arc-flag
				bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X); // sweep-flag

				PointF startPoint = endPoint;
				endPoint = get<1>(d[i])[j + 4];
				vector<PointF> centerPoint = centerEllipse(startPoint, endPoint, rx, ry);

				/*if (centerPoint[0].Y < centerPoint[1].Y)
				{
					swap(centerPoint[0], centerPoint[1]);
				}*/
				RectF rect1 = RectF(centerPoint[0].X - rx, centerPoint[0].Y - ry, 2 * rx, 2 * ry);
				RectF rect2 = RectF(centerPoint[1].X - rx, centerPoint[1].Y - ry, 2 * rx, 2 * ry);

				double angleStart1 = cos(realLength(centerPoint[0], PointF(startPoint.X, centerPoint[0].Y)) / realLength(startPoint, centerPoint[0])) * 180 / M_PI;
				double angleEnd1 = cos(realLength(centerPoint[0], PointF(endPoint.X, centerPoint[0].Y)) / realLength(endPoint, centerPoint[0])) * 180 / M_PI;
				double angleStart2 = cos(realLength(centerPoint[1], PointF(startPoint.X, centerPoint[1].Y)) / realLength(startPoint, centerPoint[1])) * 180 / M_PI;
				double angleEnd2 = cos(realLength(centerPoint[1], PointF(endPoint.X, centerPoint[1].Y)) / realLength(endPoint, centerPoint[1])) * 180 / M_PI;

				if (largeArc == 1)
				{
					if (sweep == 1)
					{
						path.AddArc(rect2, -angleStart2, 180 + angleStart2 + angleEnd2);
					}
					else
					{
						path.AddArc(rect1, angleStart1, -(180 + angleStart1 + angleEnd1));
					}
				}
				else
				{
					if (sweep == 1)
					{
						path.AddArc(rect2, angleStart1, (180 - angleStart1 - angleEnd1));
					}
					else
					{
						path.AddArc(rect2, -angleStart2, -(180 - angleStart2 - angleEnd2));
					}
				}

			}
			break;
		}
		case 'a': {
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j += 5) {
				double rx = get<1>(d[i])[j].X; // rx
				double ry = get<1>(d[i])[j].Y; // ry
				double xAxisRotation = get<1>(d[i])[j + 1].X; // x-axis-rotation
				bool largeArc = static_cast<bool>(get<1>(d[i])[j + 2].X); // large-arc-flag
				bool sweep = static_cast<bool>(get<1>(d[i])[j + 3].X); // sweep-flag
				PointF startPoint = endPoint;
				endPoint = endPoint + get<1>(d[i])[j + 4];
				vector<PointF> centerPoint = centerEllipse(startPoint, endPoint, rx, ry);
				/*if (centerPoint[0].Y < centerPoint[1].Y)
				{
					swap(centerPoint[0], centerPoint[1]);
				}*/

				RectF rect1 = RectF(centerPoint[0].X - rx, centerPoint[0].Y - ry, 2 * rx, 2 * ry);
				RectF rect2 = RectF(centerPoint[1].X - rx, centerPoint[1].Y - ry, 2 * rx, 2 * ry);


				double angleStart1 = cos(realLength(centerPoint[0], PointF(startPoint.X, centerPoint[0].Y)) / realLength(startPoint, centerPoint[0])) * 180 / M_PI;
				double angleEnd1 = cos(realLength(centerPoint[0], PointF(endPoint.X, centerPoint[0].Y)) / realLength(endPoint, centerPoint[0])) * 180 / M_PI;
				double angleStart2 = cos(realLength(centerPoint[1], PointF(startPoint.X, centerPoint[1].Y)) / realLength(startPoint, centerPoint[1])) * 180 / M_PI;
				double angleEnd2 = cos(realLength(centerPoint[1], PointF(endPoint.X, centerPoint[1].Y)) / realLength(endPoint, centerPoint[1])) * 180 / M_PI;

				if (largeArc == 1)
				{
					if (sweep == 1)
					{
						path.AddArc(rect2, -angleStart2, 180 + angleStart2 + angleEnd2);
					}
					else
					{
						path.AddArc(rect1, angleStart1, -(180 + angleStart1 + angleEnd1));
					}
				}
				else
				{
					if (sweep == 1)
					{
						path.AddArc(rect1, angleStart1, (180 - angleStart1 - angleEnd1));
					}
					else
					{
						path.AddArc(rect2, -angleStart2, -(180 - angleStart2 - angleEnd2));
					}
				}
				//Pen pPen(Color(255, 0, 0, 0), 1);
				//graphics.DrawRectangle(&pPen, rect1);
				//graphics.DrawRectangle(&pPen, rect2);
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


	if (fill_color[0] == 'u' && fill_color[1] == 'r' && fill_color[2] == 'l') {
		string id = extractID(fill_color);
		LinearGradientBrush* brush = nullptr;
		for (auto gradient : gradients) {
			if (gradient.getId() == id) {
				brush = gradient.createBrush();
				break;
			}
		}
		if (stroke_width > 0) {
			Pen pen(stroke_color, stroke_width);
			graphics.DrawPath(&pen, &path);
		}
		graphics.FillPath(brush, &path);
		delete brush;
	}
	else
	{
		Color fill = stoc(this->fill_color);
		Color fill_colorr = Color(fill_opacity * 255, fill.GetR(),
			fill.GetG(), fill.GetB());
		SolidBrush brushh(fill_colorr);

		if (stroke_width > 0) {
			Pen pen(stroke_color, stroke_width);
			graphics.DrawPath(&pen, &path);
		}
		graphics.FillPath(&brushh, &path);
	}
}



#endif
