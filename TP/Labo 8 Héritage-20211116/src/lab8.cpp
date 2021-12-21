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
#include <memory> // shared_ptr

#if __cplusplus >= 202002L
#include <numbers>
using std::numbers::pi;
#else
const double pi = 3.141592653589793238462643383279502884L;
#endif

using namespace std;

const int windowWidth = 500;
const int windowHeight = 500;
const double refreshPerSecond = 60;


struct Point {
  int x, y;
};


/*--------------------------------------------------
Rectangle class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border
--------------------------------------------------*/
class Rectangle {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void draw();
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  void setWidth(int neww) {
    w = neww;
  }
  void setHeight(int newh) {
    h = newh;
  }
  int getWidth() const {
    return w;
  }
  int getHeight() const {
    return h;
  }
  bool contains(Point p) const;
  Point getCenter() const {
    return center;
  }
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     Fl_Color fillColor)
    : center{center}, w{w}, h{h}, fillColor{fillColor},
      frameColor{frameColor} {}

void Rectangle::draw() {
  array<Point, 5> points{
    Point{center.x-w/2, center.y-h/2},
    Point{center.x-w/2, center.y+h/2},
    Point{center.x+w/2, center.y+h/2},
    Point{center.x+w/2, center.y-h/2},
    Point{center.x-w/2, center.y-h/2}};
  fl_color(fillColor);
  fl_begin_polygon();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_polygon();
  fl_color(frameColor);
  fl_begin_line();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_line();
}

void Rectangle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Rectangle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Rectangle::contains(Point p) const {
  return p.x>=center.x-w/2 &&
         p.x<center.x+w/2 &&
         p.y>=center.y-h/2 &&
         p.y<center.y+h/2;
}


/*--------------------------------------------------
Circle class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border
--------------------------------------------------*/
class Circle {
  Point center;
  int r;
  Fl_Color fillColor, frameColor;
 public:
  Circle(Point center, int r,
         Fl_Color frameColor = FL_BLACK,
         Fl_Color fillColor = FL_WHITE);
  void draw();
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  bool contains(Point p) const;
  Point getCenter() const {
    return center;
  }
};

Circle::Circle(Point center, int r,
               Fl_Color frameColor,
               Fl_Color fillColor)
    : center{center}, r{r}, fillColor{fillColor},
      frameColor{frameColor} {}

void Circle::draw() {
  array<Point, 37> points;
  for (int i=0; i<36; i++)
    points[i]= {static_cast<int>(center.x+r*sin(i*10*pi/180)),
                static_cast<int>(center.y+r*cos(i*10*pi/180))
               };
  points[36]=points[0];
  fl_color(fillColor);
  fl_begin_polygon();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_polygon();
  fl_color(frameColor);
  fl_begin_line();
  for (auto& point : points) {
    fl_vertex(point.x, point.y);
  }
  fl_end_line();
}

void Circle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}

void Circle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}

bool Circle::contains(Point p) const {
  return ( (p.x-center.x)*(p.x-center.x)
          +(p.y-center.y)*(p.y-center.y)<=r*r);
}


/*--------------------------------------------------
Translation Class
--------------------------------------------------*/
struct Translation {
  Translation(Point p) {
    fl_push_matrix();
    fl_translate(p.x, p.y);
  }
  ~Translation() {
    fl_pop_matrix();
  }
};


/*--------------------------------------------------
Rotation Class
--------------------------------------------------*/
struct Rotation {
  Rotation(Point center, double angle) {
    fl_push_matrix();
    fl_translate(center.x, center.y);
    fl_rotate(angle);
    fl_translate(-1*center.x, -1*center.y);
  }
  ~Rotation() {
    fl_pop_matrix();
  }
};


/*--------------------------------------------------
Spin Class
--------------------------------------------------*/
template <typename Sketchable>
class Spin {
  int duration;
  Sketchable *toAnimate;
  int time{0};
  double currentRotation();
 public:
  Spin(Sketchable *toAnimate, int duration = 100)
      : duration{duration}, toAnimate{toAnimate} {}
  void draw();
  bool isComplete();
};

template <class Sketchable>
void Spin<Sketchable>::draw() {
  ++time;
  Rotation r{toAnimate->getCenter(), currentRotation()};
  toAnimate->draw();
}

template <class Sketchable>
double Spin<Sketchable>::currentRotation() {
  if (!isComplete())
    return time*360.0/duration;
  else
    return 0;
}

template <class Sketchable>
bool Spin<Sketchable>::isComplete() {
  return time > duration;
}


/*--------------------------------------------------
Bounce Class
--------------------------------------------------*/
template <class Sketchable>
class Bounce {
  int duration;
  int bounceHeight;
  Sketchable *toAnimate;
  int time{0};
  Point currentTranslation();
 public:
  Bounce(Sketchable *toAnimate,
         int duration = 100, int bounceHeight = 100)
      : duration{duration},
        bounceHeight{bounceHeight},
        toAnimate{toAnimate} {}
  void draw();
  bool isComplete();
};

template <class Sketchable>
void Bounce<Sketchable>::draw() {
  ++time;
  Translation t3{currentTranslation()};
  toAnimate->draw();
}

template <class Sketchable>
Point Bounce<Sketchable>::currentTranslation() {
  if (isComplete()) 
    return {0,0};
  else
    return {0, static_cast<int>(-1*bounceHeight*sin(pi*time/duration))};
}

template <class Sketchable>
bool Bounce<Sketchable>::isComplete() {
  return time > duration;
}


/*--------------------------------------------------
ClickableCell class

The Canvas class below will have ClickableCells as instance
variables and call the methods of ClickableCell
--------------------------------------------------*/
template <typename Sketchable,typename Animation>
class ClickableCell {
  Sketchable sketchable;
  Animation *animation;
 public:
  // Constructor
  ClickableCell(Sketchable sketchable);

  // Methods that draw and handle events
  void draw();
  void mouseClick(Point mouseLoc);
};

template <typename Sketchable,typename Animation>
ClickableCell<Sketchable,Animation>::ClickableCell(Sketchable sketchable):
  sketchable{sketchable}, animation{nullptr} {}

template <typename Sketchable,typename Animation>
void ClickableCell<Sketchable,Animation>::draw() {
  if (animation && animation->isComplete()) {
    delete animation;
    animation = nullptr;
  }
  if (animation)
    animation->draw();
  else
    sketchable.draw();
}

template <typename Sketchable,typename Animation>
void ClickableCell<Sketchable,Animation>::mouseClick(Point mouseLoc) {
  if (!animation && sketchable.contains(mouseLoc)) {
    animation = new Animation(&sketchable);
  }
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
  vector< ClickableCell< Rectangle, Spin<Rectangle>> > spinners;
  vector< ClickableCell< Rectangle, Bounce<Rectangle> > > bouncingRectangles;
  vector< ClickableCell< Circle, Bounce<Circle> > > bouncingCircles;
 public:
  Canvas();
  void draw();
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};

Canvas::Canvas() {
  for (int x = 50; x<500; x+=100)
    spinners.push_back({{{x, 400},50,100}});
  for (int x = 50; x<500; x+=100)
    bouncingRectangles.push_back({{{x, 250},75,75}});
  for (int x = 50; x<500; x+=100)
    bouncingCircles.push_back({{{x, 150},30}});
}

void Canvas::draw() {
  for (auto &c: spinners) {
    c.draw();
  }
  for (auto &c: bouncingRectangles) {
    c.draw();
  }
  for (auto &c: bouncingCircles) {
    c.draw();
  }
}

void Canvas::mouseClick(Point mouseLoc) {
  for (auto &c: spinners)
    c.mouseClick(mouseLoc);
  for (auto &c: bouncingRectangles)
    c.mouseClick(mouseLoc);
  for (auto &c: bouncingCircles)
    c.mouseClick(mouseLoc);
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
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 8") {
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
