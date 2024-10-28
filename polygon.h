#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <gdiplus.h>
#include <fstream>
using namespace Gdiplus;

class myPolygon {
    private:
        int stroke[3];
        float stroke_width;
        float stroke_opacity;
        int fill[3];
        float fill_opacity;
        Point* points;
        int size;
    public:
        myPolygon() {
            stroke[0] = stroke[1] = stroke[2] = 0;
            fill[0] = fill[1] = fill[2] = 0;
            size = 0;
            stroke_width = stroke_opacity = fill_opacity = 0.0;
            points = NULL;
        }
        myPolygon(int stroke[3], float stroke_width, float stroke_opacity, int fill[3], float fill_opacity, Point* points, int size) {
            for (int i = 0; i < 3; i++) {
                this->stroke[i] = stroke[i];
                this->fill[i] = fill[i];
            }
            this->stroke_width = stroke_width;
            this->stroke_opacity = stroke_opacity;
            this->fill_opacity = fill_opacity;
            this->size = size;
            this->points = new Point[size];
            for (int i = 0; i < size; i++)
                this->points[i] = points[i];
        }
        void draw(Graphics& graphic) const{
            Pen      pen(Color((BYTE)(stroke_opacity * 255), stroke[0], stroke[1], stroke[2]), stroke_width);
            SolidBrush      brush(Color((BYTE)(fill_opacity * 255), fill[0], fill[1], fill[2]));
            graphic.FillPolygon(&brush, points, size);
            graphic.DrawPolygon(&pen, points, size);
        }
        ~myPolygon() {
            delete[] points;
            points = NULL;
        }
};

#endif