//#ifndef PATH_H
//#define PATH_H
//
//#include "SVGElement.h"
//#include <vector>
//
////<path fill = "none" stroke = "rgb(255,0,0)" stroke - width = "5" d = "M100,200 C100,100 250,100 250,200 C250,300 400,300 400,200" / >
//
//class my_path : public SVGElement
//{
//private:
//	vector<char> commands;
//	int num_points;
//	double* points;
//	Color fill_color;
//	double fill_opacity;
//	Color stroke_color;
//	double stroke_width;
//public:
//	my_path(vector<char> commands, double* points, int num_points, Color fill_color, double, Color stroke_color, double stroke_width);
//	void render(Graphics& graphics) override;
//};
//
//my_path::my_path(vector<char> commands, double* points, int num_points, Color fill_color, double fill_opacity, Color stroke_color, double stroke_width) : commands(commands), points(points), num_points(num_points), fill_color(fill_color), stroke_color(stroke_color), stroke_width(stroke_width), fill_opacity(fill_opacity) {}
//
//// M L H V C Z
//void my_path::render(Graphics& graphics)
//{
//	GraphicsPath path;
//	Pen pen(stroke_color, stroke_width);
//	Color fill_colorr = Color(fill_opacity * 255, fill_color.GetR(), fill_color.GetG(), fill_color.GetB());
//	SolidBrush brush(fill_colorr);
//	path.StartFigure();
//	int control_index = 0;
//	int endPoint1 = 0;
//	int endPoint2 = 0;
//	for (int i = 0; i < commands.size(); i++)
//	{
//		switch (commands[i])
//		{
//		case 'M':
//			path.StartFigure();
//			endPoint1 = points[control_index++];
//			endPoint2 = points[control_index++];
//			break;
//		case 'm':
//			path.StartFigure();
//			endPoint1 = points[control_index++];
//			endPoint2 = points[control_index++];
//			break;
//		case 'C':
//			path.AddBezier(endPoint1, endPoint2, points[control_index], points[control_index + 1], points[control_index + 2], points[control_index + 3], points[control_index + 4], points[control_index + 5]);
//			endPoint1 = points[control_index + 4];
//			endPoint2 = points[control_index + 5];
//			control_index += 6;
//			break;
//		case 'c':
//			path.AddBezier(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2 + points[control_index + 1], endPoint1 + points[control_index + 2], endPoint2 + points[control_index + 3], endPoint1 + points[control_index + 4], endPoint2 + points[control_index + 5]);
//			endPoint1 += points[control_index + 4];
//			endPoint2 += points[control_index + 5];
//			control_index += 6;
//			break;
//		case 'L':
//			path.AddLine(endPoint1, endPoint2, points[control_index], points[control_index + 1]);
//			endPoint1 = points[control_index];
//			endPoint2 = points[control_index + 1];
//			control_index += 2;
//			break;
//		case 'l':
//			path.AddLine(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2 + points[control_index + 1]);
//			endPoint1 += points[control_index];
//			endPoint2 += points[control_index + 1];
//			control_index += 2;
//			break;
//		case 'H':
//			path.AddLine(endPoint1, endPoint2, points[control_index], endPoint2);
//			endPoint1 = points[control_index];
//			control_index++;
//			break;
//		case 'h':
//			path.AddLine(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2);
//			endPoint1 += points[control_index];
//			control_index++;
//			break;
//		case 'V':
//			path.AddLine(endPoint1, endPoint2, endPoint1, points[control_index]);
//			endPoint2 = points[control_index];
//			control_index++;
//			break;
//		case 'v':
//			path.AddLine(endPoint1, endPoint2, endPoint1, endPoint2 + points[control_index]);
//			endPoint2 += points[control_index];
//			control_index++;
//			break;
//		case 'Z':
//			path.CloseFigure();
//			break;
//		default:
//			break;
//		}
//	}
//	graphics.FillPath(&brush, &path);
//	graphics.DrawPath(&pen, &path);
//}
//
//#endif
#ifndef PATH_H
#define PATH_H

#include "SVGElement.h"
#include <vector>
#include "stdafx.h"
#include <gdiplus.h>
#include <objidl.h>

using namespace Gdiplus;
using namespace std;

//<path fill = "none" stroke = "rgb(255,0,0)" stroke - width = "5" d = "M100,200 C100,100 250,100 250,200 C250,300 400,300 400,200" / >

class my_path : public SVGElement
{
private:
	vector<char> commands;
	int num_points;
	double* points;
	Color fill_color;
	double fill_opacity;
	Color stroke_color;
	double stroke_width;
public:
	my_path(vector<char> commands, double* points, int num_points, Color fill_color, double, Color stroke_color, double stroke_width);
	void render(Graphics& graphics) override;
	void setTransform(const string& content) override;
};

my_path::my_path(vector<char> commands, double* points, int num_points, Color fill_color, double fill_opacity, Color stroke_color, double stroke_width) : commands(commands), points(points), num_points(num_points), fill_color(fill_color), stroke_color(stroke_color), stroke_width(stroke_width), fill_opacity(fill_opacity) {}

void my_path::setTransform(const string& content) {
	this->t.parseTransform(content);
}

// M L H V C Z
void my_path::render(Graphics& graphics)
{
	this->t.applyTransform(graphics);
	GraphicsPath path;
	Pen pen(stroke_color, stroke_width);
	Color fill_colorr = Color(fill_opacity * 255, fill_color.GetR(), fill_color.GetG(), fill_color.GetB());
	SolidBrush brush(fill_colorr);
	path.StartFigure();
	int control_index = 0;
	int endPoint1 = 0;
	int endPoint2 = 0;
	for (int i = 0; i < commands.size(); i++)
	{
		switch (commands[i])
		{
		case 'M':
			path.StartFigure();
			endPoint1 = points[control_index++];
			endPoint2 = points[control_index++];
			break;
		case 'm':
			path.StartFigure();
			endPoint1 = points[control_index++];
			endPoint2 = points[control_index++];
			break;
		case 'C':
			path.AddBezier(endPoint1, endPoint2, points[control_index], points[control_index + 1], points[control_index + 2], points[control_index + 3], points[control_index + 4], points[control_index + 5]);
			endPoint1 = points[control_index + 4];
			endPoint2 = points[control_index + 5];
			control_index += 6;
			break;
		case 'c':
			path.AddBezier(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2 + points[control_index + 1], endPoint1 + points[control_index + 2], endPoint2 + points[control_index + 3], endPoint1 + points[control_index + 4], endPoint2 + points[control_index + 5]);
			endPoint1 += points[control_index + 4];
			endPoint2 += points[control_index + 5];
			control_index += 6;
			break;
		case 'L':
			path.AddLine(endPoint1, endPoint2, points[control_index], points[control_index + 1]);
			endPoint1 = points[control_index];
			endPoint2 = points[control_index + 1];
			control_index += 2;
			break;
		case 'l':
			path.AddLine(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2 + points[control_index + 1]);
			endPoint1 += points[control_index];
			endPoint2 += points[control_index + 1];
			control_index += 2;
			break;
		case 'H':
			path.AddLine(endPoint1, endPoint2, points[control_index], endPoint2);
			endPoint1 = points[control_index];
			control_index++;
			break;
		case 'h':
			path.AddLine(endPoint1, endPoint2, endPoint1 + points[control_index], endPoint2);
			endPoint1 += points[control_index];
			control_index++;
			break;
		case 'V':
			path.AddLine(endPoint1, endPoint2, endPoint1, points[control_index]);
			endPoint2 = points[control_index];
			control_index++;
			break;
		case 'v':
			path.AddLine(endPoint1, endPoint2, endPoint1, endPoint2 + points[control_index]);
			endPoint2 += points[control_index];
			control_index++;
			break;
		case 'Z':
			path.AddLine(endPoint1, endPoint2, points[0], points[1]);
			break;
		default:
			break;
		}
	}
	graphics.FillPath(&brush, &path);
	graphics.DrawPath(&pen, &path);
	this->t.resetTransform(graphics);
}

#endif