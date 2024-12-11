#ifndef PATH_H
#define PATH_H

#include "SVGElement.h"
#include <vector>
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>
#include <tuple>

using namespace Gdiplus;
using namespace std;

//<path fill = "none" stroke = "rgb(255,0,0)" stroke - width = "5" d = "M100,200 C100,100 250,100 250,200 C250,300 400,300 400,200" / >

class my_path : public SVGElement
{
private:
	vector<tuple<char, vector<PointF>>> d;
	Color fill_color;
	double fill_opacity;
	Color stroke_color;
	double stroke_width;
	string transform;
public:
	my_path(string name, string transform, vector<tuple<char, vector<PointF>>> d, Color fill_color, double, Color stroke_color, double stroke_width);
	void render(Graphics& graphics) override;
	string getTransform() override { return transform; }
};

my_path::my_path(string name, string transform, vector<tuple<char, vector<PointF>>> d, Color fill_color, double fill_opacity, Color stroke_color, double stroke_width = 0) : d(d), fill_color(fill_color), stroke_color(stroke_color), stroke_width(stroke_width), fill_opacity(fill_opacity), SVGElement(name), transform(transform) {}

// M L H V C Z
void my_path::render(Graphics& graphics)
{
	GraphicsPath path;
	Color fill_colorr = Color(fill_opacity * 255, fill_color.GetR(), fill_color.GetG(), fill_color.GetB());
	SolidBrush brush(fill_colorr);
	path.SetFillMode(FillModeAlternate);

	PointF endPoint = PointF(0, 0);
	PointF startPoint;

	int length = d.size();
	for (int i = 0; i < length; i++)
	{
		switch (get<0>(d[i]))
		{
		case 'M':
		{
			int numPoints = get<1>(d[i]).size();
			startPoint = get<1>(d[i])[0];
			path.StartFigure();
			endPoint = get<1>(d[i])[0];
			for (int j = 1; j < numPoints; j++) 
			{
				path.AddLine(endPoint, get<1>(d[i])[j]);
				endPoint = get<1>(d[i])[j];
			}
			break;
		}
		case 'm': 
		{
			int numPoints = get<1>(d[i]).size();
			startPoint = endPoint + get<1>(d[i])[0];
			path.StartFigure();
			endPoint = startPoint;
			for (int j = 1; j < numPoints; j++) 
			{
				PointF newPoint = endPoint + get<1>(d[i])[j];
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'L': 
		{ 
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				path.AddLine(endPoint, get<1>(d[i])[j]);
				endPoint = get<1>(d[i])[j];
			}
			break;
		}
		case 'l': 
		{
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				PointF newPoint = endPoint + get<1>(d[i])[j];
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'C': 
		{ 
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j += 3) 
			{
				PointF p1 = get<1>(d[i])[j];
				PointF p2 = get<1>(d[i])[j + 1];
				PointF p3 = get<1>(d[i])[j + 2];
				path.AddBezier(endPoint, p1, p2, p3);
				endPoint = p3;
			}
			break;
		}
		case 'c': 
		{ 
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j += 3) 
			{
				PointF p1 = get<1>(d[i])[j] + endPoint;
				PointF p2 = get<1>(d[i])[j + 1] + endPoint;
				PointF p3 = get<1>(d[i])[j + 2] + endPoint;
				path.AddBezier(endPoint, p1, p2, p3);
				endPoint = p3;
			}
			break;
		}
		case 'H': 
		{
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				PointF newPoint = PointF(get<1>(d[i])[j].X, endPoint.Y);
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'h': 
		{ 
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				PointF newPoint = PointF(endPoint.X + get<1>(d[i])[j].X, endPoint.Y);
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'V':
		{ 
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				PointF newPoint = PointF(endPoint.X, get<1>(d[i])[j].Y);
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'v': 
		{
			int numPoints = get<1>(d[i]).size();
			for (int j = 0; j < numPoints; j++) 
			{
				PointF newPoint = PointF(endPoint.X, endPoint.Y + get<1>(d[i])[j].Y);
				path.AddLine(endPoint, newPoint);
				endPoint = newPoint;
			}
			break;
		}
		case 'Z':
		case 'z': 
		{
			path.AddLine(endPoint, startPoint);
			endPoint = startPoint;
			path.CloseFigure();
			break;
		}
		default:
			break;
		}
	}

	if (stroke_width != 0) 
	{
		Pen pen(stroke_color, stroke_width);
		graphics.DrawPath(&pen, &path);
	}

	graphics.FillPath(&brush, &path);
}

#endif
