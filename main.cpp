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

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

string svgFileName = "";

std::unordered_map<std::string, Gdiplus::Color> colorMap = {
		{"red", Gdiplus::Color(255, 255, 0, 0)},
		{"green", Gdiplus::Color(255, 0, 255, 0)},
		{"blue", Gdiplus::Color(255, 0, 0, 255)},
		{"yellow", Gdiplus::Color(255, 255, 255, 0)},
		{"cyan", Gdiplus::Color(255, 0, 255, 255)},
		{"magenta", Gdiplus::Color(255, 255, 0, 255)},
		{"black", Gdiplus::Color(255, 0, 0, 0)},
		{"white", Gdiplus::Color(255, 255, 255, 255)},
		{"gray", Gdiplus::Color(255, 128, 128, 128)},
		{"orange", Gdiplus::Color(255, 255, 165, 0)},
		{"darkslategray", Gdiplus::Color(255, 47, 79, 79)},
		{"midnightblue", Gdiplus::Color(255, 25, 25, 112)},
		{"blueviolet", Gdiplus::Color(255, 138, 43, 226)},
		{"navy", Gdiplus::Color(255, 0, 0, 128)},
		{"grey", Gdiplus::Color(255, 128, 128, 128)},
		{"skyblue", Gdiplus::Color(255, 135, 206, 235)},
		{"darkmagenta", Gdiplus::Color(255, 139, 0, 139)},
		{"deepskyblue", Gdiplus::Color(255, 0, 191, 255)},
		{"purple", Gdiplus::Color(255, 160, 32, 240)}
};

Color hexatoRGB(string hex)
{
	// Check if the hex string is 4 characters long (including the '#')
	if (hex.length() == 4)
	{
		// Expand the 3-character shorthand to 6 characters
		string expandedHex = "#" + string(1, hex[1]) + string(1, hex[1]) + string(1, hex[2]) + string(1, hex[2]) + string(1, hex[3]) + string(1, hex[3]);
		hex = expandedHex;
	}

	int r, g, b;
	sscanf_s(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);

	return Color(r, g, b);
}

Color stoc(string rgb)
{
	if (rgb == "none")
	{
		return Color(255, 255, 255);
	}
	if (rgb[0] == '#')
	{
		return hexatoRGB(rgb);
	}

	string check = rgb.substr(0, 3);
	if (check != "rgb") {
		auto it = colorMap.find(rgb);
		if (it != colorMap.end()) {
			return it->second;
		}
		else {
			return Color(255, 255, 255);
		}
	}

	int r, g, b;
	sscanf_s(rgb.c_str(), "rgb(%d, %d, %d)", &r, &g, &b);
	if (r > 255)
	{
		r = 255;
	}
	if (g > 255)
	{
		g = 255;
	}
	if (b > 255)
	{
		b = 255;
	}
	return Color(r, g, b);
}

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
		if (d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z' || d[i] == 's' || d[i] == 'S')
		{
			char command = d[i];
			vector<PointF> points;
			if (command == 'z' || command == 'Z')
			{
				result.push_back(make_tuple(command, points));
				continue;
			}
			i++;

			while (!(d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z' || d[i] == 's' || d[i] == 'S') && i < length)
			{
				string num = "";
				while ((d[i] >= '0' && d[i] <= '9') || d[i] == '-' || d[i] == '.')
				{
					num += d[i];
					i++;
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
				while (d[i] >= '0' && d[i] <= '9' || d[i] == '-' || d[i] == '.')
				{
					num += d[i];
					i++;
					if (d[i] == '-')
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
		Color fill_color = stoc(fill);
		Color stroke_color = stoc(stroke);
		string transform = child->first_attribute("transform") ? child->first_attribute("transform")->value() : "";
		double stroke_width = child->first_attribute("stroke-width") ? stod(child->first_attribute("stroke-width")->value()) : group_stroke_width;
		double stroke_opacity = child->first_attribute("stroke-opacity") ? stod(child->first_attribute("stroke-opacity")->value()) : group_stroke_opacity;
		double fill_opacity = child->first_attribute("fill-opacity") ? stod(child->first_attribute("fill-opacity")->value()) : group_fill_opacity;
		fill_opacity = child->first_attribute("opacity") ? stod(child->first_attribute("opacity")->value()) : fill_opacity;
		double fontSize = child->first_attribute("font-size") ? stoi(child->first_attribute("font-size")->value()) : group_fontSize;

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

			SVGElement* element = new my_circle(string(child->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		// Process <rect> elements
		else if (string(child->name()) == "rect") {
			double x = child->first_attribute("x") ? stod(child->first_attribute("x")->value()) : 0;
			double y = child->first_attribute("y") ? stod(child->first_attribute("y")->value()) : 0;
			double width = child->first_attribute("width") ? stod(child->first_attribute("width")->value()) : 0;
			double height = child->first_attribute("height") ? stod(child->first_attribute("height")->value()) : 0;

			SVGElement* element = new my_rect(string(child->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
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

			SVGElement* element = new my_text(string(child->name()), transform, x, y - fontSize, fill_color, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		//<polyline>
		else if (string(child->name()) == "polyline") {
			string points = child->first_attribute("points")->value();
			double num_points = 0;
			double* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polyline(string(child->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<ellipse>
		else if (string(child->name()) == "ellipse") {
			double cx = child->first_attribute("cx") ? stod(child->first_attribute("cx")->value()) : 0;
			double cy = child->first_attribute("cy") ? stod(child->first_attribute("cy")->value()) : 0;
			double rx = stod(child->first_attribute("rx")->value());
			double ry = stod(child->first_attribute("ry")->value());

			SVGElement* element = new my_ellipse(string(child->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<polygon>
		else if (string(child->name()) == "polygon") {
			string points = child->first_attribute("points")->value();
			double num_points = 0;
			double* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polygon(string(child->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<line>
		else if (string(child->name()) == "line") {
			double x1 = child->first_attribute("x1") ? stod(child->first_attribute("x1")->value()) : 0;
			double y1 = child->first_attribute("y1") ? stod(child->first_attribute("y1")->value()) : 0;
			double x2 = child->first_attribute("x2") ? stod(child->first_attribute("x2")->value()) : 0;
			double y2 = child->first_attribute("y2") ? stod(child->first_attribute("y2")->value()) : 0;

			SVGElement* element = new my_line(string(child->name()), transform, x1, y1, x2, y2, stroke_color, stroke_width, stroke_opacity, NULL, NULL);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<path>
		else if (string(child->name()) == "path") {
			string d = child->first_attribute("d")->value();
			vector<tuple<char, vector<PointF>>> path = parsePath(d);

			if (!child->first_attribute("stroke") && !node->first_attribute("stroke")) {
				stroke_width = 0;
			}

			SVGElement* element = new my_path(string(child->name()), transform, path, fill_color, fill_opacity, stroke_color, stroke_width);
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

vector<SVGElement*> parseSVG(string filePath, vector<double>& boxValues, string& widthS, string& heightS) {
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
		for (rapidxml::xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling()) {
			string fill = node->first_attribute("fill") ? node->first_attribute("fill")->value() : "rgb(0,0,0)";
			string stroke = node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "rgb(255,255,255)";
			Color fill_color = stoc(fill);
			Color stroke_color = stoc(stroke);
			string transform = node->first_attribute("transform") ? node->first_attribute("transform")->value() : "";
			double stroke_width = node->first_attribute("stroke-width") ? stod(node->first_attribute("stroke-width")->value()) : 1.0;
			double stroke_opacity = node->first_attribute("stroke-opacity") ? stod(node->first_attribute("stroke-opacity")->value()) : 1.0;
			double fill_opacity = node->first_attribute("fill-opacity") ? stod(node->first_attribute("fill-opacity")->value()) : 1.0;
			fill_opacity = node->first_attribute("opacity") ? stod(node->first_attribute("opacity")->value()) : fill_opacity;
			double fontSize = node->first_attribute("font-size") ? stoi(node->first_attribute("font-size")->value()) : 10;

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

				SVGElement* element = new my_circle(string(node->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			// Process <rect> elements
			else if (string(node->name()) == "rect") {
				double x = node->first_attribute("x") ? stod(node->first_attribute("x")->value()) : 0;
				double y = node->first_attribute("y") ? stod(node->first_attribute("y")->value()) : 0;
				double width = node->first_attribute("width") ? stod(node->first_attribute("width")->value()) : 0;
				double height = node->first_attribute("height") ? stod(node->first_attribute("height")->value()) : 0;

				SVGElement* element = new my_rect(string(node->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

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

				SVGElement* element = new my_text(string(node->name()), transform, x, y - fontSize, fill_color, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);

				elements.push_back(element);
			}
			//<polyline>
			else if (string(node->name()) == "polyline") {
				string points = node->first_attribute("points")->value();
				double num_points = 0;
				double* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polyline(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			//<ellipse>
			else if (string(node->name()) == "ellipse") {
				double cx = node->first_attribute("cx") ? stod(node->first_attribute("cx")->value()) : 0;
				double cy = node->first_attribute("cy") ? stod(node->first_attribute("cy")->value()) : 0;
				double rx = stoi(node->first_attribute("rx")->value());
				double ry = stoi(node->first_attribute("ry")->value());

				SVGElement* element = new my_ellipse(string(node->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			//<polygon>
			else if (string(node->name()) == "polygon") {
				string points = node->first_attribute("points")->value();
				double num_points = 0;
				double* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polygon(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

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

				SVGElement* element = new my_path(string(node->name()), transform, path, fill_color, fill_opacity, stroke_color, stroke_width);

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


// 1 2 3 4 5 7 8 9 10 11 12 15 16
// nearly: 13 14 17 18
// not done: 6
VOID OnPaint(HDC hdc)
{
	vector<double> boxValues;
	string width, height;
	svgFileName = "svg/svg-17.svg";
	vector<SVGElement*> element = parseSVG(svgFileName, boxValues, width, height);
	Graphics graphics(hdc);


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
			graphics.TranslateTransform(static_cast<float>(translateX), static_cast<float>(translateY));
		}
		graphics.ScaleTransform(static_cast<float>(scale), static_cast<float>(scale));
		graphics.TranslateTransform(static_cast<float>(-boxValues[0]), static_cast<float>(-boxValues[1]));
	}

	SVGRenderer renderer;
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
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,      // window style
		CW_USEDEFAULT,            // initial x position
		CW_USEDEFAULT,            // initial y position
		CW_USEDEFAULT,            // initial x size
		CW_USEDEFAULT,            // initial y size
		NULL,                     // parent window handle
		NULL,                     // window menu handle
		hInstance,                // program instance handle
		NULL);                    // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(gdiplusToken);
	return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC          hdc;
	PAINTSTRUCT  ps;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
} // WndProc
