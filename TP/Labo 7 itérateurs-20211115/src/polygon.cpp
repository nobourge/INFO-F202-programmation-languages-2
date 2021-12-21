#include "polygon.h"

#include <FL/fl_draw.H>

Polygon::Polygon(const vector<Point>& vertexes, Point center,
                 Fl_Color frameColor, Fl_Color fillColor)
    : vertexes{vertexes},
      center{center},
      fillColor{fillColor},
      frameColor{frameColor} {}

void Polygon::draw() const {
  const Point v0 = vertexes.at(0);
  fl_color(fillColor);
  fl_begin_polygon();
  for (auto& point : vertexes) {
    fl_vertex(point.x, point.y);
  }
  fl_vertex(v0.x, v0.y);
  fl_end_polygon();
  fl_color(frameColor);
  fl_begin_line();
  for (auto& point : vertexes) {
    fl_vertex(point.x, point.y);
  }
  fl_vertex(v0.x, v0.y);
  fl_end_line();
}
