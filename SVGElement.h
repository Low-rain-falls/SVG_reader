#ifndef SVGElEMENT_H
#define SVGElEMENT_H

#include <gdiplus.h>
#include "objidl.h"
#include "stdafx.h"
#include "LinearGradient.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace Gdiplus;

class SVGElement {
private:
	string name;

public:
	SVGElement(const string& name) : name(name) {}
	virtual void render(Graphics& graphic, vector<LinearGradient> gradients) = 0;
	virtual ~SVGElement() = default;
	virtual string getTransform() = 0;
};

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
		{"purple", Gdiplus::Color(255, 160, 32, 240)},
		{"lightcoral", Gdiplus::Color(240, 128, 128)},
{"saddlebrown",  Gdiplus::Color(139,45,13)}
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

// "url(#fill0)" => "fill0"
string extractID(string id) {
	int pos = id.find("#");
	int end = id.find(")");
	return id.substr(pos + 1, end - pos - 1);
}


#endif
