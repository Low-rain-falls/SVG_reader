#ifndef LINEAR_GRADIENT_H
#define LINEAR_GRADIENT_H

#include <gdiplus.h>
#include <iostream>
#include "Stop.h"

using namespace std;


class LinearGradient {
private:
    string id;
    double x1, x2, y1, y2;
	string gradientUnits;
    vector<Stop> stops;
public:
    LinearGradient(string id, double x1, double x2, double y1, double y2, string gradientUnits = "objectBoundingBox", vector<Stop> stops) : id(id), x1(x1), x2(x2), y1(y1), y2(y2), gradientUnits(gradientUnits), stops(stops) {}
	LinearGradientBrush*& createBrush() {
		LinearGradientBrush* brush = new LinearGradientBrush(PointF(x1, y1), PointF(x2, y2), stops[0].color, stops[2].color);
		Color colors[] = { stops[0].color, stops[1].color, stops[2].color };
		REAL positions[] = { static_cast<REAL>(stops[0].offset), static_cast<REAL>(stops[1].offset), static_cast<REAL>(stops[2].offset) };
		brush->SetInterpolationColors(colors, positions, 3);
		return brush;
	}	
	string getId() {
		return id;
	}
	
};

#endif