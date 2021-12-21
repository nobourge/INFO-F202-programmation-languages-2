#ifndef __POLYGON_H
#define __POLYGON_H

#include <Fl/Fl.H>

#include <vector>

#include "common.h"

using namespace std;
/*--------------------------------------------------

Polygon class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/
class Polygon {
  const vector<Point> vertexes;
  Point center;
  Fl_Color fillColor, frameColor;

 public:
  Polygon(const vector<Point> &vertexes, Point center,
          Fl_Color frameColor = FL_BLACK, Fl_Color fillColor = FL_WHITE);
  void draw() const;
};

#endif  // POLYGON_H