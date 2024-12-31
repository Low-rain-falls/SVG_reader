#ifndef LINEAR_GRADIENT_H
#define LINEAR_GRADIENT_H

#include "Stop.h"
#include <gdiplus.h>
#include <iostream>

using namespace std;


class LinearGradient {
private:
    string id;
    double x1, x2, y1, y2;
    vector<Stop> stops;
public:
    LinearGradient(string id, double x1, double x2, double y1, double y2, vector<Stop> stops) : id(id), x1(x1), x2(x2), y1(y1), y2(y2), stops(stops) {}
	LinearGradientBrush*& createBrush() {
		LinearGradientBrush* brush = new LinearGradientBrush(PointF(x1, y1), PointF(x2, y2), stops[0].color, stops[stops.size() - 1].color);
		Color* color = new Color[stops.size()];
		for (int i = 0; i < stops.size(); i++) {
			color[i] = stops[i].color;
		}
		float* positions = new float[stops.size()];
		for (int i = 0; i < stops.size(); i++) {
			positions[i] = stops[i].offset / stops[stops.size() - 1].offset;
		}
		brush->SetInterpolationColors(color, positions, stops.size());
		return brush;
	}	
	string getId() {
		return id;
	}
	
};

#endif