#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include "SVGElement.h"

class SVGRenderer {
public:
	vector<SVGElement*> elements;

	void addElement(SVGElement* element);
	void render(Graphics& graphic);
	~SVGRenderer();
};

void SVGRenderer::addElement(SVGElement* element) {
	this->elements.push_back(element);
}

void SVGRenderer::render(Graphics& graphic) {
	for (SVGElement* element : this->elements) {
		element->render(graphic);
	}
}

SVGRenderer::~SVGRenderer() {
	for (SVGElement* element : this->elements) {
		delete element;
	}
}

#endif 
