#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "stdafx.h"
#include "gdiplus.h"
#include "objidl.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace Gdiplus;


class Transform {
public:
    double translateX = 0, translateY = 0;
    double rotateAngle = 0, rotateCenterX = 0, rotateCenterY = 0;
    double scaleX = 1, scaleY = 1;

    void applyTransform(Graphics& g);
    void parseTransform(const string& content);
    void resetTransform(Graphics& g);
	void appleMultipleTransforms(Graphics& g);
};

void Transform::applyTransform(Graphics& g) {
    Matrix matrix;
    if (translateX != 0 || translateY != 0) {
        matrix.Translate(translateX, translateY);
    }
    if (rotateAngle != 0) {
        matrix.RotateAt(rotateAngle, PointF(rotateCenterX, rotateCenterY));
    }
    if (scaleX != 1 || scaleY != 1) {
        matrix.Scale(scaleX, scaleY);
    }
    g.SetTransform(&matrix);
}

void Transform::parseTransform(const string& content) {
    if (content == "") {
        return;
    }
    istringstream stream(content);
    string token;

    while (std::getline(stream, token, ')')) {
        size_t pos = token.find('(');
        if (pos == std::string::npos) continue;

        std::string type = token.substr(0, pos);
        if (type[0] == ' ') {
            type = type.substr(1);
        }
        std::string params = token.substr(pos + 1);
        std::istringstream paramStream(params);
        std::vector<double> values;
        double value;

        while (paramStream >> value) {
            values.push_back(value);
            if (paramStream.peek() == ',') paramStream.ignore();
        }

        if (type == "translate") {
            translateX = values[0];
            if (values.size() > 1) translateY = values[1];
        }
        else if (type == "rotate") {
            rotateAngle = values[0];
            if (values.size() > 2) {
                rotateCenterX = values[1];
                rotateCenterY = values[2];
            }
        }
        else if (type == "scale") {
            scaleX = values[0];
            if (values.size() > 1) scaleY = values[1];
            else scaleY = scaleX;
        }
        else {
            /*throw std::runtime_error("Unsupported transform type: " + type);*/
        }
    }
}

void Transform::resetTransform(Graphics& g) {
    g.ResetTransform();
	translateX = translateY = 0;
	rotateAngle = rotateCenterX = rotateCenterY = 0;
	scaleX = scaleY = 1;
}

void Transform::appleMultipleTransforms(Graphics& g) {
	Matrix matrix;
	matrix.Translate(translateX, translateY);
	matrix.RotateAt(rotateAngle, PointF(rotateCenterX, rotateCenterY));
	matrix.Scale(scaleX, scaleY);
	g.MultiplyTransform(&matrix);
}

#endif
