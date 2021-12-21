#include "canvas.h"

#include <math.h>

Canvas::Canvas() {
  int r = 70;
  for (int i = 3; i < 12; i++) {
    vector<Point> vertexes;
    Point center{75 + (i - 3) % 3 * 150, 75 + (i - 3) / 3 * 150};
    for (int j = 0; j < i; j++) {
      vertexes.push_back(
          {static_cast<int>(center.x + r * sin(j * 2 * pi / i)),
           static_cast<int>(center.y + r * cos(j * 2 * pi / i))});
    }
    shapes.emplace_back(vertexes, center);
  }
}

void Canvas::draw() {
  // const int r = 4;
  for (Polygon &s : shapes) {
    s.draw();
    // You need to make this for loop work
    // DO NOT EDIT OTHER THAN UNCOMMENTING!!!!!
    /*
    for (const Point &p : s) {
      Polygon{{{p.x + r, p.y + r},
               {p.x - r, p.y + r},
               {p.x - r, p.y - r},
               {p.x + r, p.y - r}},
              p,
              FL_BLACK,
              FL_YELLOW}
          .draw();
    }
    */
  }
}

void Canvas::mouseClick(Point /*mouseLoc*/) {}

void Canvas::keyPressed(int keyCode) {
  switch (keyCode) {
    case 'q':
      exit(0);
  }
}