#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include "SVGElement.h"
#include "Transform.h"

class SVGRenderer {
private:
  Transform transform;

public:
  void render(Graphics &graphic, vector<SVGElement *>);
  void renderTransform(Graphics &graphic, string transform);
};

void SVGRenderer::render(Graphics &graphic, vector<SVGElement *> element) {

  for (auto i : element) {
    string transform = i->getTransform();
    if (transform != "") {
      this->transform.parseTransform(transform);
      this->transform.applyTransform(graphic);
      i->render(graphic);
      this->transform.resetTransform(graphic);
      continue;
    }
    i->render(graphic);
  }
}

void SVGRenderer::renderTransform(Graphics &graphic, string transform) {
  this->transform.parseTransform(transform);
  this->transform.applyTransform(graphic);
}

#endif
