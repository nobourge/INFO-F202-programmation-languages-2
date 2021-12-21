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

It should have all the features you need and you
should not need to edit it.

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
  int getWidth() {
    return w;
  }
  int getHeight() {
    return h;
  }
  bool contains(Point p);
  Point getCenter() {
    return center;
  }
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, w{w}, h{h}, fillColor{fillColor}, frameColor{frameColor} {}

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

bool Rectangle::contains(Point p) {
  return p.x>=center.x-w/2 &&
         p.x<center.x+w/2 &&
         p.y>=center.y-h/2 &&
         p.y<center.y+h/2;
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

Cell class declaration (implementations later)

The Canvas class below will have cells as instance
vraiables and call the methods of Cell
--------------------------------------------------*/

class Animation;

class Cell {
  Rectangle r;
  Animation *animation;
 public:
  // Constructor
  Cell(Point center, int w, int h);

  // Methods that draw and handle events
  void drawWithoutAnimate();
  void draw();
  void mouseMove(Point /* mouseLoc */) {};
  void mouseClick(Point mouseLoc);
  Point getCenter() {
    return r.getCenter();
  }
};

/*--------------------------------------------------

Animation Class
--------------------------------------------------*/

class Animation {
 public:
  enum AnimationType {spin, bounce, spinAndBounce};
 private:
  const int animationTime = 60;
  const int bounceHeight = 200;
  Cell *c;
  AnimationType animationType;
  int time{0};
  Point currentTranslation();
  double currentRotation();
 public:
  Animation(Cell *cellToAnimate, AnimationType animationType)
      : c{cellToAnimate}, animationType{animationType} {}
  void draw();
  bool isComplete();
};

void Animation::draw() {
  ++time;
  Translation t3{currentTranslation()};
  Rotation r{c->getCenter(), currentRotation()};
  c->drawWithoutAnimate();
}

Point Animation::currentTranslation() {
  if (animationType==bounce || animationType == spinAndBounce)
    return {0, static_cast<int>(-1*bounceHeight*sin(3.1415*time/animationTime))};
  else
    return {0, 0};
}
double Animation::currentRotation() {
  if (animationType==spin || animationType == spinAndBounce)
    return time*360.0/animationTime;
  else
    return 0;
}


bool Animation::isComplete() {
  return time>60;
}

/*--------------------------------------------------

Cell Class Implementation
--------------------------------------------------*/

Cell::Cell(Point center, int w, int h):
  r{center, w, h, FL_BLACK, FL_WHITE}, animation{nullptr} {}

void Cell::drawWithoutAnimate() {
  r.draw();
}

void Cell::draw() {
  if (animation && animation->isComplete()) {
    delete animation;
    animation = nullptr;
  }
  if (animation)
    animation->draw();
  else
    drawWithoutAnimate();
}


void Cell::mouseClick(Point mouseLoc) {
  if (!animation && r.contains(mouseLoc)) {
    animation = new Animation(this, static_cast<Animation::AnimationType>(random()%3));
  }
}

/*--------------------------------------------------

Canvas class.

One instance of the canvas class is made by the
MainWindow class.

The fltk system via MainWindow calls:

draw 60 times a second
mouseMove whenever the mouse is moved
mouseClick whenever the mouse is clicked
keyPressed whenever a key is pressed

Any drawing code should be called ONLY in draw
or methods called by draw. If you try to draw
elsewhere it will probably crash.
--------------------------------------------------*/


class Canvas {
  vector< Cell > cells;
 public:
  Canvas();
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};


Canvas::Canvas() {
  for (int x = 50; x<500; x+=50)
    cells.push_back({{x, 300}, 45, 90});
}

void Canvas::draw() {
  for (auto &c: cells) {
    c.draw();
  }
}

void Canvas::mouseMove(Point mouseLoc) {
  for (auto &c: cells)
    c.mouseMove(mouseLoc);
}

void Canvas::mouseClick(Point mouseLoc) {
  for (auto &c: cells)
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
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 4") {
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    canvas.draw();
  }
  int handle(int event) override {
    switch (event) {
      case FL_MOVE:
        canvas.mouseMove(Point{Fl::event_x(), Fl::event_y()});
        return 1;
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
