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
#include <unordered_map>

#include "SVGElement.h"
#include "SVGRenderer.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Rect.h"
#include "Text.h"
#include "Path.h"
#include "Group.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

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
		{"skyblue", Gdiplus::Color(255, 135, 206, 235)}
};
Color hexatoRGB(string hex)
{
	if (hex == "none")
	{
		return Color(255, 255, 255, 255);  // Return white if "none"
	}

	// Check if the hex string is 4 characters long (including the '#')
	if (hex.length() == 4)
	{
		// Expand the 3-character shorthand to 6 characters
		string expandedHex = "#" + string(1, hex[1]) + string(1, hex[1]) + string(1, hex[2]) + string(1, hex[2]) + string(1, hex[3]) + string(1, hex[3]);
		hex = expandedHex;
	}

	int r, g, b;
	sscanf_s(hex.c_str(), "#%02x%02x%02x", &r, &g, &b);

	return Color(255, r, g, b);
}

Color stoc(string rgb)
{
	if (rgb[0] == '#')
	{
		return hexatoRGB(rgb);
	}
	if (rgb == "none")
	{
		return Color(255, 255, 255, 255);
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
	return Color(r, g, b);
}

int* stoArr(string points, int& num_points)
{
	vector<int> arr;
	while (points != "")
	{
		int pos = points.find(",");
		arr.push_back(stoi(points));
		points = points.substr(pos + 1);
		arr.push_back(stoi(points));
		pos = points.find(" ");
		if (pos == -1)
		{
			break;
		}
		points = points.substr(pos + 1);
	}

	num_points = arr.size();
	int* arrr = new int[num_points];

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
		if (d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z')
		{
			char command = d[i];
			vector<PointF> points;
			if (command == 'z' || command == 'Z')
			{
				result.push_back(make_tuple(command, points));
				continue;
			}
			i++;

			while (!(d[i] == 'm' || d[i] == 'M' || d[i] == 'L' || d[i] == 'l' || d[i] == 'V' || d[i] == 'v' || d[i] == 'h' || d[i] == 'H' || d[i] == 'c' || d[i] == 'C' || d[i] == 'z' || d[i] == 'Z') && i < length)
			{
				string num = "";
				while ((d[i] >= '0' && d[i] <= '9') || d[i] == '-')
				{
					num += d[i];
					i++;
					if (d[i] == '-')
					{
						break;
					}
				}
				float x = stof(num);
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
				while (d[i] == ' ' || d[i] == ',' || d[i] == '\n')
				{
					i++;
				}
				while (d[i] >= '0' && d[i] <= '9' || d[i] == '-')
				{
					num += d[i];
					i++;
					if (d[i] == '-')
					{
						break;
					}
				}
				float y = stof(num);
				points.push_back(PointF(x, y));
				while (d[i] == ' ' || d[i] == ',' || d[i] == '\n')
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
		double stroke_width = child->first_attribute("stroke-width") ? stof(child->first_attribute("stroke-width")->value()) : group_stroke_width;
		double stroke_opacity = child->first_attribute("stroke-opacity") ? stof(child->first_attribute("stroke-opacity")->value()) : group_stroke_opacity;
		double fill_opacity = child->first_attribute("fill-opacity") ? stof(child->first_attribute("fill-opacity")->value()) : group_fill_opacity;
		fill_opacity = child->first_attribute("opacity") ? stof(child->first_attribute("opacity")->value()) : fill_opacity;
		int fontSize = node->first_attribute("font-size") ? stoi(node->first_attribute("font-size")->value()) : group_fontSize;

		// Process <circle> elements
		if (string(child->name()) == "circle") {
			int cx = stoi(child->first_attribute("cx")->value());
			int cy = stoi(child->first_attribute("cy")->value());
			int r = stoi(child->first_attribute("r")->value());

			SVGElement* element = new my_circle(string(node->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		// Process <rect> elements
		else if (string(child->name()) == "rect") {
			int x = stoi(child->first_attribute("x")->value());
			int y = stoi(child->first_attribute("y")->value());
			int width = stoi(child->first_attribute("width")->value());
			int height = stoi(child->first_attribute("height")->value());

			SVGElement* element = new my_rect(string(node->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		// Process <text> elements
		else if (string(child->name()) == "text") {
			int dx = child->first_attribute("dx") ? stoi(child->first_attribute("dx")->value()) : 0;
			int dy = child->first_attribute("dy") ? stoi(child->first_attribute("dy")->value()) : 0;
			int x = stoi(child->first_attribute("x")->value()) + dx;
			int y = stoi(child->first_attribute("y")->value()) + dy;
			int fontSize = child->first_attribute("font-size") ? stoi(child->first_attribute("font-size")->value()) : 10;
			string font_style = child->first_attribute("font-style") ? child->first_attribute("font-style")->value() : "";
			string text_anchor = child->first_attribute("text-anchor") ? child->first_attribute("text-anchor")->value() : "";
			string font_family = child->first_attribute("font-family") ? child->first_attribute("font-family")->value() : "Consolas";
			string content = child->value();
			
			SVGElement* element = new my_text(string(node->name()), transform, x, y - fontSize, fill_color, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
		//<polyline>
		else if (string(child->name()) == "polyline") {
			string points = child->first_attribute("points")->value();
			int num_points = 0;
			int* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polyline(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<ellipse>
		else if (string(child->name()) == "ellipse") {
			int cx = stoi(child->first_attribute("cx")->value());
			int cy = stoi(child->first_attribute("cy")->value());
			int rx = stoi(child->first_attribute("rx")->value());
			int ry = stoi(child->first_attribute("ry")->value());

			SVGElement* element = new my_ellipse(string(node->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<polygon>
		else if (string(child->name()) == "polygon") {
			string points = child->first_attribute("points")->value();
			int num_points = 0;
			int* pointsArr = stoArr(points, num_points);

			SVGElement* element = new my_polygon(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<line>
		else if (string(child->name()) == "line") {
			int x1 = stoi(child->first_attribute("x1")->value());
			int y1 = stoi(child->first_attribute("y1")->value());
			int x2 = stoi(child->first_attribute("x2")->value());
			int y2 = stoi(child->first_attribute("y2")->value());

			SVGElement* element = new my_line(string(node->name()), transform, x1, y1, x2, y2, stroke_color, stroke_width, stroke_opacity, NULL, NULL);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		//<path>
		else if (string(child->name()) == "path") {
			string d = child->first_attribute("d")->value();
			vector<tuple<char, vector<PointF>>> path = parsePath(d);

			SVGElement* element = new my_path(string(node->name()), transform, path, fill_color, fill_opacity, stroke_color, stroke_width);
			dynamic_cast<my_group*>(elements)->addElement(element);

		}
		// Xử lý thẻ <g> (group) lồng vào nhau
		else if (string(child->name()) == "g") {
			SVGElement* element = new my_group(string(node->name()), transform);
			handleGroup(element, child, stroke, stroke_width, stroke_opacity, fill, fill_opacity, fontSize);
			dynamic_cast<my_group*>(elements)->addElement(element);
		}
	}
}

vector<SVGElement*> parseSVG(string filePath, vector<double> &boxValues, string &width, string &height) {
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

	width = svgNode->first_attribute("width") ? svgNode->first_attribute("width")->value() : "";
	height = svgNode->first_attribute("height") ? svgNode->first_attribute("height")->value() : "";

	string viewBox = svgNode->first_attribute("viewBox") ? svgNode->first_attribute("viewBox")->value() : "";

	if (viewBox != "") {
		std::istringstream ss(viewBox);
		double value;
		while (ss >> value) {
			boxValues.push_back(value);
			if (ss.peek() == ' ') ss.ignore(); // Bỏ qua khoảng trắng
		}
	}
	
	
	if (svgNode) {
		for (rapidxml::xml_node<>* node = svgNode->first_node(); node; node = node->next_sibling()) {
			string fill = node->first_attribute("fill") ? node->first_attribute("fill")->value() : "rgb(255,255,255)";
			string stroke = node->first_attribute("stroke") ? node->first_attribute("stroke")->value() : "rgb(255,255,255)";
			Color fill_color = stoc(fill);
			Color stroke_color = stoc(stroke);
			string transform = node->first_attribute("transform") ? node->first_attribute("transform")->value() : "";
			double stroke_width = node->first_attribute("stroke-width") ? stof(node->first_attribute("stroke-width")->value()) : 0;
			double stroke_opacity = node->first_attribute("stroke-opacity") ? stof(node->first_attribute("stroke-opacity")->value()) : 1.0;
			double fill_opacity = node->first_attribute("fill-opacity") ? stof(node->first_attribute("fill-opacity")->value()) : 1.0;
			fill_opacity = node->first_attribute("opacity") ? stof(node->first_attribute("opacity")->value()) : fill_opacity;
			int fontSize = node->first_attribute("font-size") ? stoi(node->first_attribute("font-size")->value()) : 10;

			// Process <circle> elements
			if (string(node->name()) == "circle") {
				int cx = stoi(node->first_attribute("cx")->value());
				int cy = stoi(node->first_attribute("cy")->value());
				int r = stoi(node->first_attribute("r")->value());

				SVGElement* element = new my_circle(string(node->name()), transform, cx, cy, r, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			// Process <rect> elements
			else if (string(node->name()) == "rect") {
				int x = stoi(node->first_attribute("x")->value());
				int y = stoi(node->first_attribute("y")->value());
				int width = stoi(node->first_attribute("width")->value());
				int height = stoi(node->first_attribute("height")->value());

				SVGElement* element = new my_rect(string(node->name()), transform, x, y, width, height, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			// Process <text> elements
			else if (string(node->name()) == "text") {
				int dx = node->first_attribute("dx") ? stoi(node->first_attribute("dx")->value()) : 0;
				int dy = node->first_attribute("dy") ? stoi(node->first_attribute("dy")->value()) : 0;
				int x = stoi(node->first_attribute("x")->value()) + dx;
				int y = stoi(node->first_attribute("y")->value()) + dy;
				int fontSize = node->first_attribute("font-size") ? stoi(node->first_attribute("font-size")->value()) : 10;
				string font_style = node->first_attribute("font-style") ? node->first_attribute("font-style")->value() : "";
				string text_anchor = node->first_attribute("text-anchor") ? node->first_attribute("text-anchor")->value() : "";
				string font_family = node->first_attribute("font-family") ? node->first_attribute("font-family")->value() : "Consolas";
				string content = node->value();
				
				SVGElement* element = new my_text(string(node->name()), transform, x, y - fontSize, fill_color, stroke_color, stroke_width, fontSize, font_style, text_anchor, font_family, content);
				
				elements.push_back(element);
			}
			//<polyline>
			else if (string(node->name()) == "polyline") {
				string points = node->first_attribute("points")->value();
				int num_points = 0;
				int* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polyline(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			//<ellipse>
			else if (string(node->name()) == "ellipse") {
				int cx = stoi(node->first_attribute("cx")->value());
				int cy = stoi(node->first_attribute("cy")->value());
				int rx = stoi(node->first_attribute("rx")->value());
				int ry = stoi(node->first_attribute("ry")->value());

				SVGElement* element = new my_ellipse(string(node->name()), transform, cx, cy, rx, ry, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			//<polygon>
			else if (string(node->name()) == "polygon") {
				string points = node->first_attribute("points")->value();
				int num_points = 0;
				int* pointsArr = stoArr(points, num_points);

				SVGElement* element = new my_polygon(string(node->name()), transform, pointsArr, num_points, stroke_color, stroke_width, stroke_opacity, fill_color, fill_opacity);

				elements.push_back(element);
			}
			//<line>
			else if (string(node->name()) == "line") {
				int x1 = stoi(node->first_attribute("x1")->value());
				int y1 = stoi(node->first_attribute("y1")->value());
				int x2 = stoi(node->first_attribute("x2")->value());
				int y2 = stoi(node->first_attribute("y2")->value());

				SVGElement* element = new my_line(string(node->name()), transform, x1, y1, x2, y2, stroke_color, stroke_width, stroke_opacity, NULL, NULL);

				elements.push_back(element);
			}
			//<path>
			else if (string(node->name()) == "path") {
				string d = node->first_attribute("d")->value();
				vector<tuple<char, vector<PointF>>> path = parsePath(d);
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

VOID OnPaint(HDC hdc)
{
	vector<double> boxValues;
	string width, height;
	vector<SVGElement*> element = parseSVG("svg-02.svg", boxValues, width, height);
	Gdiplus::Graphics graphics(hdc);
	
	if (!boxValues.empty()) {
		double viewportHeight, viewportWidth;
		if (width == "" || height == "") {
			viewportWidth = 800;
			viewportHeight = 800;
		}
		else {
			viewportWidth = stof(width);
			viewportHeight = stof(height);
		}

		double scaleX = viewportWidth / boxValues[2];
		double scaleY = viewportHeight / boxValues[3];

		graphics.TranslateTransform(boxValues[0], boxValues[1]);
		graphics.ScaleTransform(scaleX, scaleY);
	}
	

	SVGRenderer renderer;
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	renderer.render(graphics, element);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
	HWND                hWnd;
	MSG                 msg;
	WNDCLASS            wndClass;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;


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
		WS_OVERLAPPEDWINDOW,      // window style
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
