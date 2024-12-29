#ifndef RADIAL_H
#define RADIAL_H


#include <iostream>
#include <vector>
#include "Stop.h"

using namespace std;

class RadialGradient {
private:
	string id;
	double cx, cy, r, fx, fy;
	vector<Stop> stops;

public:
	RadialGradient(string id, double cx, double cy, double r, double fx, double fy, vector<Stop> stops) : id(id), cx(cx), cy(cy), r(r), fx(fx), fy(fy), stops(stops) {}
	PathGradientBrush*& createBrush() {
		GraphicsPath path;
		REAL x = static_cast<REAL>(cx - r);
		REAL y = static_cast<REAL>(cy - r);
		REAL width = static_cast<REAL>(2 * r);
		REAL height = static_cast<REAL>(2 * r);
		path.AddEllipse(x, y, width, height);

		PathGradientBrush* brush = new PathGradientBrush(&path);

		const Stop& centerStop = stops.front();
		brush->SetCenterColor(Color(
			static_cast<BYTE>(centerStop.opacity * 255),
			static_cast<BYTE>(centerStop.color.r),
			static_cast<BYTE>(centerStop.color.g),
			static_cast<BYTE>(centerStop.color.b)
		));

		if (stops.size() > 1) {
			vector<Color> surroundColors;
			for (size_t i = 1; i < stops.size(); ++i) {
				const Stop& stop = stops[i];
				surroundColors.push_back(Color(
					static_cast<BYTE>(stop.opacity * 255),
					static_cast<BYTE>(stop.color.r),
					static_cast<BYTE>(stop.color.g),
					static_cast<BYTE>(stop.color.b)
				));
			}

			brush->SetSurroundColors(&surroundColors[0], nullptr);
		}

		brush->SetCenterPoint(PointF(static_cast<REAL>(fx), static_cast<REAL>(fy)));


		return brush;
	}
};
 
#endif