#ifndef SVGElEMENT_H
#define SVGElEMENT_H

#include "gdiplus.h"
#include "objidl.h"
#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace Gdiplus;

class SVGElement {
private:
  string name;

public:
  SVGElement(const string &name) : name(name) {}
  virtual void render(Graphics &graphic) = 0;
  virtual ~SVGElement() = default;
  virtual string getTransform() = 0;
};

#endif
