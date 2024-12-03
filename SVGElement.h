#ifndef SVGElEMENT_H
#define SVGElEMENT_H

#include "stdafx.h"
#include "gdiplus.h"
#include "objidl.h"
#include "Transform.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace Gdiplus;

class SVGElement
{
	public:
		Transform t;
		vector<SVGElement*> getG();
		virtual void setTransform(const string& content) = 0;
		virtual void render(Graphics& graphic) = 0;
		virtual ~SVGElement() = default;
};

#endif