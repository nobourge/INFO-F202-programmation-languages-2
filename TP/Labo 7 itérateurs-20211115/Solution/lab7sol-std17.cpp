// These should include everything you might use
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <array>

#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi=3.141592653589793238462643383279502884L;
#endif

using namespace std;

const int windowWidth = 450;
const int windowHeight = 450;
const double refreshPerSecond = 60;


struct Point {
  int x, y;
};


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
  Polygon(const vector<Point> &vertexes,
          Point center,
          Fl_Color frameColor = FL_BLACK,
          Fl_Color fillColor = FL_WHITE);
  void draw() const;

  class Iterator{
    const Polygon *polygon;
    vector<Point>::size_type index;
  public:
    Iterator(const Polygon *polygon,vector<Point>::size_type index):polygon{polygon},index{index}{};
    void operator++(){++index;}
    const Point &operator *() const {
      if (index==polygon->vertexes.size())
        return polygon->center;
      else return polygon->vertexes.at(index);}
    const Point &operator ->() const {
      if (index==polygon->vertexes.size())
        return polygon->center;
      else return polygon->vertexes.at(index);}
    bool operator!=(const Iterator &other) const {
      return !(polygon==other.polygon && index==other.index);
    }
  };
  Iterator begin(){return Iterator{this,0};}
  Iterator end(){return Iterator{this,vertexes.size()+1};}
};

Polygon::Polygon(const vector<Point> &vertexes,
                    Point center,
                    Fl_Color frameColor,
                    Fl_Color fillColor):
  vertexes{vertexes}, center{center}, fillColor{fillColor}, frameColor{frameColor} {}

void Polygon::draw() const {
  const Point v0=vertexes.at(0);
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


/*--------------------------------------------------

Canvas class.

One instance of the canvas class is made by the
MainWindow class.

The fltk system via MainWindow calls:

draw 60 times a second
mouseClick whenever the mouse is clicked
keyPressed whenever a key is pressed

Any drawing code should be called ONLY in draw
or methods called by draw. If you try to draw
elsewhere it will probably crash.
--------------------------------------------------*/


class Canvas {
  vector <Polygon> shapes;
 public:
  Canvas();
  void draw();
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};


Canvas::Canvas() {
  int r=70;
  for (int i=3;i<12;i++){
    vector<Point> vertexes;
    Point center{75+(i-3)%3*150,75+(i-3)/3*150};
    for (int j=0;j<i;j++){
      vertexes.push_back({static_cast<int>(center.x+r*sin(j*2*pi/i)),
       static_cast<int>(center.y+r*cos(j*2*pi/i))});
    }
    shapes.emplace_back(vertexes,center);
  }
}

void Canvas::draw() {
  const int r=4;
  for (auto &s: shapes) {
    s.draw();
    for (const Point &p:s)
      Polygon{{{p.x+r,p.y+r},{p.x-r,p.y+r},{p.x-r,p.y-r},{p.x+r,p.y-r}},p,FL_BLACK,FL_YELLOW}.draw();
  }
}

void Canvas::mouseClick(Point /*mouseLoc*/) {
}


void Canvas::keyPressed(int keyCode) {
  switch (keyCode) {
    case 'q':
      exit(0);
  }
}


/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
  Canvas canvas;
 public:
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 7") {
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    canvas.draw();
  }
  int handle(int event) override {
    switch (event) {
      case FL_PUSH:
        canvas.mouseClick(Point{Fl::event_x(), Fl::event_y()});
        return 1;
      case FL_KEYDOWN:
        canvas.keyPressed(Fl::event_key());
        return 1;
    }
    return 0;
  }
  static void Timer_CB(void *userdata) {
    MainWindow *o = (MainWindow*) userdata;
    o->redraw();
    Fl::repeat_timeout(1.0/refreshPerSecond, Timer_CB, userdata);
  }
};


/*--------------------------------------------------

main

Do not edit!!!!

--------------------------------------------------*/


int main(int argc, char *argv[]) {
  srand(time(0));
  MainWindow window;
  window.show(argc, argv);
  return Fl::run();
}
