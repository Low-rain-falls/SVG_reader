#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "SVGElement.h"

void applyTransform(Graphics& graphics, string transform) {
    std::istringstream ss(transform);
    std::string token;

    while (std::getline(ss, token, ')')) {
        if (token.empty()) continue;

        size_t openParen = token.find('(');
        if (openParen == std::string::npos) {
            throw std::invalid_argument("Invalid transform format");
        }

        std::string type = token.substr(0, openParen);
        std::string values = token.substr(openParen + 1);
        std::istringstream valueStream(values);
        std::vector<float> params;
        float value;

        while (valueStream >> value) {
            params.push_back(value);
            if (valueStream.peek() == ',' || valueStream.peek() == ' ') {
                valueStream.ignore();
            }
        }

        if (type == "translate") {
            float tx = params[0];
            float ty = (params.size() > 1) ? params[1] : 0.0f;
            graphics.TranslateTransform(tx, ty);
        }
        else if (type == "rotate") {
            float angle = params[0];
            float cx = 0.0f, cy = 0.0f;

            if (params.size() > 1) {
                cx = params[1];
                cy = params[2];
            }

            graphics.TranslateTransform(cx, cy);

            graphics.RotateTransform(angle);

            graphics.TranslateTransform(-cx, -cy);
        }
        else if (type == "scale") {
            float sx = params[0];
            float sy = (params.size() > 1) ? params[1] : sx;
            graphics.ScaleTransform(sx, sy);
        }
    }
}

class my_group : public SVGElement {
private:
	std::vector<SVGElement*> elements; 
    string transform;
public:
	my_group(string name, string transform) : elements(elements), SVGElement(name), transform(transform) {}

	void addElement(SVGElement* element) {
		this->elements.push_back(element);
	}

	void render(Graphics& graphic) override {
		for (auto& element : elements) {
            if (element->getTransform() != "")
            {
				GraphicsState save = graphic.Save();
                Transform t;
				t.parseTransform(element->getTransform());
				t.appleMultipleTransforms(graphic);
				element->render(graphic);
				graphic.Restore(save);
                continue;
            }
			element->render(graphic);
		}
	}

    ~my_group() override {
        for (SVGElement* element : this->elements) {
            delete element;
        }
    }

	string getTransform() override {
		return this->transform;
	}
};

#endif