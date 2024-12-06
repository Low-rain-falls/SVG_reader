#pragma once
#ifndef GROUP_H
#define GROUP_H

#include "SVGElement.h"



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