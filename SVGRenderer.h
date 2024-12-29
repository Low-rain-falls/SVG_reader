#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include "LinearGradient.h"
#include "SVGElement.h"
#include "Transform.h"

class SVGRenderer {
private:
  Transform transform;
  vector<LinearGradient> linearGradients;

public:
  SVGRenderer(vector<LinearGradient> v) : linearGradients(v) {}
  void render(Graphics &graphic, vector<SVGElement *>);
  void renderTransform(Graphics &graphic, string transform);
};

void SVGRenderer::render(Graphics &graphic, vector<SVGElement *> element) {

  for (auto i : element) {
    string transforms = i->getTransform();
    if (transforms != "") {
      this->transform.parseTransform(transforms);
      this->transform.appleMultipleTransforms(graphic);
      i->render(graphic, linearGradients);
      this->transform.resetTransform(graphic);
      continue;
    }
    i->render(graphic, linearGradients);
  }
}

// void SVGRenderer::renderTransform(Graphics &graphic, string transform) {
//   this->transform.parseTransform(transform);
//   this->transform.applyTransform(graphic);
// }

#endif
