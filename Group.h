#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "SVGElement.h"

class my_group : public SVGElement {
private:
	std::vector<SVGElement*> elements;
	string transform;

public:
	my_group(string name, string transform)
		: elements(elements), SVGElement(name), transform(transform) {}

	void addElement(SVGElement* element) { this->elements.push_back(element); }

	void render(Graphics& graphic, vector<LinearGradient> gradients) override {
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->getTransform() != "") {
				GraphicsState save = graphic.Save();
				Transform t;
				t.parseTransform(elements[i]->getTransform());
				t.appleMultipleTransforms(graphic);
				elements[i]->render(graphic, gradients);
				graphic.Restore(save);
				continue;
			}
			elements[i]->render(graphic, gradients);
		}
	}

	~my_group() override {
		for (SVGElement* element : this->elements) {
			delete element;
		}
	}

	string getTransform() override { return this->transform; }
};

#endif
