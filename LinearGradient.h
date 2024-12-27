#ifndef LINEAR_GRADIENT_H
#define LINEAR_GRADIENT_H

#include "Stop.h"
#include <gdiplus.h>

class LinearGradient {
private:
    string id;
    double x1, x2, y1, y2;
    vector<Stop> stops;
public:
    LinearGradient(string id, double x1, double x2, double y1, double y2, vector<Stop> stops) : SVGElement(name), id(id), x1(x1), x2(x2), y1(y1), y2(y2), stops(stops) {}

    ~LinearGradient () override {

    }
};

#endif