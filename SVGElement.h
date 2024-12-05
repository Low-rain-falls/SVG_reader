#ifndef SVGElEMENT_H
#define SVGElEMENT_H

#include "stdafx.h"
#include "gdiplus.h"
#include "objidl.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace Gdiplus;

class SVGElement
{
private:
	string name;
public:
	string getName() {
		return this->name;
	}
	SVGElement(const string& name) : name(name) {}
	virtual void render(Graphics& graphic) = 0;
	virtual ~SVGElement() = default;
	virtual string getTransform() = 0;
};

#endif