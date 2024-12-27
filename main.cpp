#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>
#include <cstring>
#include <unordered_map>
#include <algorithm>

#include "SVGElement.h"
#include "SVGRenderer.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "polygon.h"
#include "polyline.h"
#include "Rect.h"
#include "Text.h"
#include "Path.h"
#include "Group.h"
#include "LinearGradient.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

string svgFileName = "";

double* stoArr(string points, double& num_points)
{
	vector<double> arr;
	while (points != "")
	{
		string point = "";
		while (points[0] == ' ' || points[0] == ',' || points[0] == '\n' || points[0] == '\t')
		{
			points.erase(0, 1);
		}
		while ((points[0] >= '0' && points[0] <= '9') || points[0] == '.' || points[0] == '-')
		{
			point += points[0];
			points.erase(0, 1);
		}
		if (point == "")
		{
			break;
		}
		double pointValue = stod(point);
		arr.push_back(pointValue);
	}

	num_points = arr.size();
	double* arrr = new double[num_points];

	for (int i = 0; i < arr.size(); i++)
	{
		arrr[i] = arr[i];
	}

	return arrr;
}

vector<tuple<char, vector<PointF>>> parsePath(string d)
{
	int length = d.length();
	vector<tuple<char, vector<PointF>>> result;
	for (int i = 0; i < length; i++)
	{
		if (d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z' || d[i] == 's' || d[i] == 'S' || d[i] == 'A' || d[i] == 'a')
		{
			char command = d[i];
			vector<PointF> points;
			if (command == 'z' || command == 'Z')
			{
				result.push_back(make_tuple(command, points));
				continue;
			}
			i++;

			while (!(d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z' || d[i] == 's' || d[i] == 'S' || d[i] == 'A' || d[i] == 'a') && i < length)
			{
				string num = "";
				while ((d[i] >= '0' && d[i] <= '9') || d[i] == '-' || d[i] == '.')
				{
					if (d[i] == '.' && num == "")
					{
						num += '0';
					}
					num += d[i];
					i++;
					if (d[i] == '.' && num == "-")
					{
						num += '0';
						continue;
					}
					if (d[i] == '-')
					{
						break;
					}
				}
				double x = stod(num);
				if (command == 'V' || command == 'v')
				{
					points.push_back(PointF(0, x));
					break;
				}
				if (command == 'h' || command == 'H')
				{
					points.push_back(PointF(x, 0));
					break;
				}
				num = "";
				while (d[i] == ' ' || d[i] == ',' || d[i] == '\n' || d[i] == '\t')
				{
					i++;
				}
				while ((d[i] >= '0' && d[i] <= '9') || d[i] == '-' || d[i] == '.')
				{
					if (d[i] == '.' && num == "")
					{
						num += '0';
					}
					num += d[i];
					i++;
					if (d[i] == '.' && num == "-")
					{
						num += '0';
						continue;
					}
					if ((d[i] == '-' || d[i] == '.') && num.find('.') != -1)
					{
						break;
					}
				}
				double y = stod(num);
				points.push_back(PointF(x, y));
				while (d[i] == ' ' || d[i] == ',' || d[i] == '\n' || d[i] == '\t')
				{
					i++;
				}
			}
			tuple<char, vector<PointF>> temp = make_tuple(command, points);
			result.push_back(temp);
			i--;
		}
	}
	return result;
}

void handleGroup(SVGElement* elements, xml_node<>* node, string group_stroke, double group_stroke_width, double group_stroke_opacity, string group_fill, double group_fill_opacity, int group_fontSize) {
	for (xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {

		string fill = child->first_attribute("fill") ? child->first_attribute("fill")->value() : group_fill;
		string stroke = child->first_attribute("stroke") ? child->first_attribute("stroke")->value() : group_stroke;
		string transform = child->first_attribute("transform") ? child->first_attribute("transform")->value() : "";
		double stroke_width = child->first_attribute("stroke-width") ? stod(child->first_attribute("stroke-width")->value()) : group_stroke_width;
		double stroke_opacity = child->first_attribute("stroke-opacity") ? stod(child->first_attribute("stroke-opacity")->value()) : group_stroke_opacity;
		double fill_opacity = child->first_attribute("fill-opacity") ? stod(child->first_attribute("fill-opacity")->value()) : group_fill_opacity;
		fill_opacity = child->first_attribute("opacity") ? stod(child->first_attribute("opacity")->value()) : fill_opacity;
		double fontSize = child->first_attribute("font-size") ? stoi(child->first_attribute("font-size")->value()) : group_fontSize;
		Color stroke_color = stoc(stroke);
		if (fill == "none")
		{
			fill_opacity = 0;
		}

		if (stroke == "none")
		{
			stroke_opacity = 0;
			stroke_width = 0;
		}

		// Process <circle> elements
		if (string(child->name()) == "circle") {
			double cx = child->first_attribute("cx") ? stod(child->first_attribute("cx")->value()) : 0;
			double cy = child->first_attribute("cy") ? stod(child->first_attribute("cy")->value()) : 0;
			double r = stod(child->first_attribute("r")->value());

			SVGElement* element = new my_circle(string(child->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		// Process <rect> elements
		else if (string(child->name()) == "rect") {
			double x = child->first_attribute("x") ? stod(child->first_attribute("x")->value()) : 0;
			double y = child->first_attribute("y") ? stod(child->first_attribute("y")->value()) : 0;
			double width = child->first_attribute("width") ? stod(child->first_attribute("width")->value()) : 0;
			double height = child->first_attribute("height") ? stod(child->first_attribute("height")->value()) : 0;

			SVGElement* element = new my_rect(string(child->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		// Process <text> elements
		else if (string(child->name()) == "text") {
			double dx = child->first_attribute("dx") ? stod(child->first_attribute("dx")->value()) : 0;
			double dy = child->first_attribute("dy") ? stod(child->first_attribute("dy")->value()) : 0;
			double x = child->first_attribute("x") ? stod(child->first_attribute("x")->value()) + dx : 0 - dx;
			double y = child->first_attribute("y") ? stod(child->first_attribute("y")->value()) + dy : 0 - dy;
			string font_style = child->first_attribute("font-style") ? child->first_attribute("font-style")->value() : "FontStyleRegular";
			string text_anchor = child->first_attribute("text-anchor") ? child->first_attribute("text-anchor")->value() : "start";
			string font_family = child->first_attribute("font-family") ? child->first_attribute("font-family")->value() : "Consolas";
			string content = child->value();

			if (!child->first_attribute("stroke") && !node->first_attribute("stroke")) {
				stroke_width = 0;

			}

			SVGElement* element = new my_text(string(child->name()), transform, x, y - fontSize, fill, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		//<polyline>
		else if (string(child->name()) == "polyline") {
			string points = child->first_attribute("points")->value();
			double num_points = 0;
			double* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polyline(string(child->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<ellipse>
		else if (string(child->name()) == "ellipse") {
			double cx = child->first_attribute("cx") ? stod(child->first_attribute("cx")->value()) : 0;
			double cy = child->first_attribute("cy") ? stod(child->first_attribute("cy")->value()) : 0;
			double rx = stod(child->first_attribute("rx")->value());
			double ry = stod(child->first_attribute("ry")->value());

			SVGElement* element = new my_ellipse(string(child->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<polygon>
		else if (string(child->name()) == "polygon") {
			string points = child->first_attribute("points")->value();
			double num_points = 0;
			double* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polygon(string(child->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<line>
		else if (string(child->name()) == "line") {
			double x1 = child->first_attribute("x1") ? stod(child->first_attribute("x1")->value()) : 0;
			double y1 = child->first_attribute("y1") ? stod(child->first_attribute("y1")->value()) : 0;
			double x2 = child->first_attribute("x2") ? stod(child->first_attribute("x2")->value()) : 0;
			double y2 = child->first_attribute("y2") ? stod(child->first_attribute("y2")->value()) : 0;

			SVGElement* element = new my_line(string(child->name()), transform, x1, y1, x2, y2, stroke_color, stroke_width, stroke_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<path>
		else if (string(child->name()) == "path") {
			string d = child->first_attribute("d")->value();
			vector<tuple<char, vector<PointF>>> path = parsePath(d);

			if (!child->first_attribute("stroke") && !node->first_attribute("stroke")) {
				stroke_width = 0;
			}

			SVGElement* element = new my_path(string(child->name()), transform, path, fill, fill_opacity, stroke_color, stroke_width);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		// Xử lý thẻ <g> (group) lồng vào nhau
		else if (string(child->name()) == "g") {
			SVGElement* element = new my_group(string(child->name()), transform);
			handleGroup(element, child, stroke, stroke_width, stroke_opacity, fill, fill_opacity, fontSize);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
	}
}

vector<SVGElement*> parseSVG(string filePath, vector<double>& boxValues, string& widthS, string& heightS, vector<LinearGradient>& gradients) {
	vector<SVGElement*> elements;
	ifstream file(filePath);
	if (!file.is_open()) {
		cerr << "Error opening file: " << filePath << endl;
		return elements;
	}

	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	rapidxml::xml_document<> doc;
	doc.parse<0>(&buffer[0]);

	rapidxml::xml_node<>* svgNode = doc.first_node("svg");

	widthS = svgNode->first_attribute("width") ? svgNode->first_attribute("width")->value() : "";
	if (widthS != "") {
		string temp;
		for (char c : widthS) {
			if (isdigit(c) || c == '.' || c == '-') {
				temp += c;
			}
			else {
				break;
			}
		}
		widthS = temp;
	}
	heightS = svgNode->first_attribute("height") ? svgNode->first_attribute("height")->value() : "";
	if (heightS != "") {
		string temp;
		for (char c : heightS) {
			if (isdigit(c) || c == '.' || c == '-') {
				temp += c;
			}
			else {
				break;
			}
		}
		heightS = temp;
	}

	string viewBox = svgNode->first_attribute("viewBox") ? svgNode->first_attribute("viewBox")->value() : "";

	if (viewBox != "") {
		std::istringstream ss(viewBox);
		double value;
		while (ss >> value) {
			boxValues.push_back(value);
			if (ss.peek() == ' ') ss.ignore();
		}
	}


	if (svgNode) {
		if (string(svgNode->first_node()->name()) == "defs") {
			rapidxml::xml_node<>* gradientNode = svgNode->first_node();
			for (rapidxml::xml_node<>* node = gradientNode->first_node(); node; node = node->next_sibling())
			{
				if (string(node->name()) == "linearGradient") {
					string id = node->first_attribute("id")->value();
					double x1 = stod(node->first_attribute("x1")->value());
					double y1 = stod(node->first_attribute("y1")->value());
					double x2 = stod(node->first_attribute("x2")->value());
					double y2 = stod(node->first_attribute("y2")->value());

					vector<Stop> stops;
					for (auto* stopNode = node->first_node("stop"); stopNode; stopNode = stopNode->next_sibling("stop")) {
						double offset = stod((stopNode->first_attribute("offset"))->value());
						std::string color = stopNode->first_attribute("stop-color")->value();
						Color stop_color = stoc(color);
						double opacity = stopNode->first_attribute("stop-opacity") ? std::stod(stopNode->first_attribute("stop-opacity")->value()) : 1.0;
						Stop stop(offset, opacity, stop_color);
						stops.push_back(stop);
					}
					LinearGradient gradient(id, x1, y1, x2, y2, stops);
					gradients.push_back(gradient);
				}
			}
		}
		else if (string(svgNode->first_node()->name()) == "linearGradient") {
			for (rapidxml::xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling())
			{
				if (string(node->name()) == "linearGradient") {
					string id = node->first_attribute("id")->value();
					double x1 = stod(node->first_attribute("x1")->value());
					double y1 = stod(node->first_attribute("y1")->value());
					double x2 = stod(node->first_attribute("x2")->value());
					double y2 = stod(node->first_attribute("y2")->value());

					vector<Stop> stops;
					for (auto* stopNode = node->first_node("stop"); stopNode; stopNode = stopNode->next_sibling("stop")) {
						double offset = stod((stopNode->first_attribute("offset"))->value());
						std::string color = stopNode->first_attribute("stop-color")->value();
						Color stop_color = stoc(color);
						double opacity = stopNode->first_attribute("stop-opacity") ? std::stod(stopNode->first_attribute("stop-opacity")->value()) : 1.0;
						Stop stop(offset, opacity, stop_color);
						stops.push_back(stop);
					}
					LinearGradient gradient(id, x1, y1, x2, y2, stops);
					gradients.push_back(gradient);
				}
			}
		}

		for (rapidxml::xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling()) {
			string fill = node->first_attribute("fill") ? node->first_attribute("fill")->value() : "rgb(0,0,0)";
			string stroke = node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "rgb(255,255,255)";
			string transform = node->first_attribute("transform") ? node->first_attribute("transform")->value() : "";
			double stroke_width = node->first_attribute("stroke-width") ? stod(node->first_attribute("stroke-width")->value()) : 1.0;
			double stroke_opacity = node->first_attribute("stroke-opacity") ? stod(node->first_attribute("stroke-opacity")->value()) : 1.0;
			double fill_opacity = node->first_attribute("fill-opacity") ? stod(node->first_attribute("fill-opacity")->value()) : 1.0;
			fill_opacity = node->first_attribute("opacity") ? stod(node->first_attribute("opacity")->value()) : fill_opacity;
			double fontSize = node->first_attribute("font-size") ? stoi(node->first_attribute("font-size")->value()) : 10;
			Color stroke_color = stoc(stroke);

			if (fill == "none") {
				fill_opacity = 0;
			}

			if (stroke == "none") {
				stroke_opacity = 0;
				stroke_width = 0;
			}

			// Process <circle> elements
			if (string(node->name()) == "circle") {
				double cx = node->first_attribute("cx") ? stod(node->first_attribute("cx")->value()) : 0;
				double cy = node->first_attribute("cy") ? stod(node->first_attribute("cy")->value()) : 0;
				double r = stod(node->first_attribute("r")->value());

				SVGElement* element = new my_circle(string(node->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);

				elements.push_back(element);
			}
			// Process <rect> elements
			else if (string(node->name()) == "rect") {
				double x = node->first_attribute("x") ? stod(node->first_attribute("x")->value()) : 0;
				double y = node->first_attribute("y") ? stod(node->first_attribute("y")->value()) : 0;
				double width = node->first_attribute("width") ? stod(node->first_attribute("width")->value()) : 0;
				double height = node->first_attribute("height") ? stod(node->first_attribute("height")->value()) : 0;

				SVGElement* element = new my_rect(string(node->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);

				elements.push_back(element);
			}
			// Process <text> elements
			else if (string(node->name()) == "text") {
				double dx = node->first_attribute("dx") ? stod(node->first_attribute("dx")->value()) : 0;
				double dy = node->first_attribute("dy") ? stod(node->first_attribute("dy")->value()) : 0;
				double x = node->first_attribute("x") ? stod(node->first_attribute("x")->value()) + dx : 0 - dx;
				double y = node->first_attribute("y") ? stod(node->first_attribute("y")->value()) + dy : 0 - dy;
				string font_style = node->first_attribute("font-style") ? node->first_attribute("font-style")->value() : "FontStyleRegular";
				string text_anchor = node->first_attribute("text-anchor") ? node->first_attribute("text-anchor")->value() : "start";
				string font_family = node->first_attribute("font-family") ? node->first_attribute("font-family")->value() : "Consolas";
				string content = node->value();

				if (!node->first_attribute("stroke")) {
					stroke_width = 0;
				}

				SVGElement* element = new my_text(string(node->name()), transform, x, y - fontSize, fill, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);

				elements.push_back(element);
			}
			//<polyline>
			else if (string(node->name()) == "polyline") {
				string points = node->first_attribute("points")->value();
				double num_points = 0;
				double* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polyline(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);

				elements.push_back(element);
			}
			//<ellipse>
			else if (string(node->name()) == "ellipse") {
				double cx = node->first_attribute("cx") ? stod(node->first_attribute("cx")->value()) : 0;
				double cy = node->first_attribute("cy") ? stod(node->first_attribute("cy")->value()) : 0;
				double rx = stoi(node->first_attribute("rx")->value());
				double ry = stoi(node->first_attribute("ry")->value());

				SVGElement* element = new my_ellipse(string(node->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);

				elements.push_back(element);
			}
			//<polygon>
			else if (string(node->name()) == "polygon") {
				string points = node->first_attribute("points")->value();
				double num_points = 0;
				double* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polygon(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill, fill_opacity);

				elements.push_back(element);
			}
			//<line>
			else if (string(node->name()) == "line") {
				double x1 = node->first_attribute("x1") ? stod(node->first_attribute("x1")->value()) : 0;
				double y1 = node->first_attribute("y1") ? stod(node->first_attribute("y1")->value()) : 0;
				double x2 = node->first_attribute("x2") ? stod(node->first_attribute("x2")->value()) : 0;
				double y2 = node->first_attribute("y2") ? stod(node->first_attribute("y2")->value()) : 0;

				SVGElement* element = new my_line(string(node->name()), transform, x1, y1, x2, y2, stroke_color, stroke_width, stroke_opacity, NULL, NULL);

				elements.push_back(element);
			}
			//<path>
			else if (string(node->name()) == "path") {
				string d = node->first_attribute("d")->value();
				vector<tuple<char, vector<PointF>>> path = parsePath(d);

				if (!node->first_attribute("stroke"))
				{
					stroke_width = 0;
				}

				SVGElement* element = new my_path(string(node->name()), transform, path, fill, fill_opacity, stroke_color, stroke_width);

				elements.push_back(element);
			}
			// Xử lý thẻ <g> (group) lồng vào nhau
			else if (string(node->name()) == "g") {
				SVGElement* element = new my_group(string(node->name()), transform);
				handleGroup(element, node, stroke, stroke_width, stroke_opacity, fill, fill_opacity, fontSize);
				elements.push_back(element);
			}
		}
	}
	else {
		cout << "<svg> element not found in file." << endl;
	}
}

VOID OnPaint(HDC hdc, Graphics& graphics)
{
	vector<double> boxValues;
	vector<LinearGradient> gradients;
	string width, height;
	svgFileName = "kali.svg";
	vector<SVGElement*> element = parseSVG(svgFileName, boxValues, width, height, gradients);
	gradients.size();
	if (!boxValues.empty()) {

		RECT clientRect;
		GetClientRect(WindowFromDC(hdc), &clientRect);
		double viewportWidth = clientRect.right - clientRect.left;
		double viewportHeight = clientRect.bottom - clientRect.top;

		if (!width.empty() || !height.empty())
		{
			viewportHeight = stod(height);
			viewportWidth = stod(width);
		}

		double viewBoxWidth = boxValues[2];
		double viewBoxHeight = boxValues[3];

		double scaleX = viewportWidth / viewBoxWidth;
		double scaleY = viewportHeight / viewBoxHeight;
		double scale = min(scaleX, scaleY);


		if (width.empty() || height.empty())
		{
			double translateX = (viewportWidth - viewBoxWidth * scale) / 2.0;
			double translateY = (viewportHeight - viewBoxHeight * scale) / 2.0;
			//graphics.TranslateTransform(static_cast<float>(translateX), static_cast<float>(translateY));
			graphics.MultiplyTransform(new Matrix(1, 0, 0, 1, translateX, translateY));
		}
		/*graphics.ScaleTransform(static_cast<float>(scale), static_cast<float>(scale));
		graphics.TranslateTransform(static_cast<float>(-boxValues[0]), static_cast<float>(-boxValues[1]));*/
		graphics.MultiplyTransform(new Matrix(static_cast<float>(scale), 0, 0, static_cast<float>(scale), 0, 0));
		graphics.MultiplyTransform(new Matrix(1, 0, 0, 1, -boxValues[0], -boxValues[1]));
	}

	SVGRenderer renderer(gradients);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	renderer.render(graphics, element);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR pCommandLine, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	if (strlen(pCommandLine) > 0) {
		svgFileName = pCommandLine;
	}

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = TEXT("GettingStarted");

	RegisterClass(&wndClass);

	hWnd = CreateWindow(
		TEXT("GettingStarted"),   // window class name
		TEXT("SVG Demo"),  // window caption
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;
	static int scrollPosV = 0;
	static int scrollPosH = 0;
	switch (message)
	{
	case WM_CREATE:
	{
		SCROLLINFO si = { sizeof(si) };
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = 0;
		si.nMax = 100000;
		si.nPage = 1000;
		si.nPos = 0;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		break;
	}

	case WM_VSCROLL:
	{
		int oldPos = scrollPosV;
		SCROLLINFO si = { sizeof(si) };
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		switch (LOWORD(wParam))
		{
		case SB_LINEUP: scrollPosV -= 10; break;
		case SB_LINEDOWN: scrollPosV += 10; break;
		case SB_PAGEUP: scrollPosV -= si.nPage; break;
		case SB_PAGEDOWN: scrollPosV += si.nPage; break;
		case SB_THUMBTRACK:
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(hWnd, SB_VERT, &si);
			scrollPosV = si.nTrackPos;
			break;
		}
		scrollPosV = max(0, min(scrollPosV, 100000));

		if (scrollPosV != oldPos)
		{
			SetScrollPos(hWnd, SB_VERT, scrollPosV, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}

	case WM_HSCROLL:
	{
		int oldPos = scrollPosH; // Track old position
		SCROLLINFO si = { sizeof(si) };
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_HORZ, &si);

		switch (LOWORD(wParam))
		{
		case SB_LINEUP: scrollPosH -= 10; break;
		case SB_LINEDOWN: scrollPosH += 10; break;
		case SB_PAGEUP: scrollPosH -= si.nPage; break;
		case SB_PAGEDOWN: scrollPosH += si.nPage; break;
		case SB_THUMBTRACK:
			si.fMask = SIF_TRACKPOS;
			GetScrollInfo(hWnd, SB_HORZ, &si);
			scrollPosH = si.nTrackPos;
			break;
		}
		scrollPosH = max(0, min(scrollPosH, 100000));

		if (scrollPosH != oldPos)
		{
			SetScrollPos(hWnd, SB_HORZ, scrollPosH, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		Graphics graphics(hdc);
		graphics.TranslateTransform(0, -scrollPosV, MatrixOrderAppend);
		graphics.TranslateTransform(-scrollPosH, 0, MatrixOrderAppend);
		OnPaint(hdc, graphics);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc