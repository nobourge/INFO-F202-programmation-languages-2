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
#include <memory>

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
Printable class.
--------------------------------------------------*/

class Printable {
 public:
  virtual void print()=0;
  virtual ~Printable() {};
};

/*--------------------------------------------------
Clickable class.
--------------------------------------------------*/

class Clickable {
 public:
  virtual bool contains (Point p) const =0;
  virtual void onClick() {};
};


/*--------------------------------------------------
Message Receiver
--------------------------------------------------*/

class MessageReceiver {
 public:
  MessageReceiver() {}
  virtual void receiveMessage(const string &message)=0;
  virtual ~MessageReceiver()=default;
};


/*--------------------------------------------------
Send messages when clicked
--------------------------------------------------*/
class SendMessessageWhenClicked: public Clickable {
  string message;
  shared_ptr<MessageReceiver> receiver;
 public:
  SendMessessageWhenClicked(string message, shared_ptr<MessageReceiver> receiver)
      : message{message}, receiver{receiver} {}
  void onClick() override {
    receiver->receiveMessage(message);
  }
};

/*--------------------------------------------------
Text class.

Use to display text on the screen. For example:

Text("Hello!", {250, 250}).print();

will create a text temporary instance with the
string "Hello!" centered at 250, 250 and will
call print on the temporary.

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

class Text: public Printable {
  string s;
  Point center;
  int fontSize;
  Fl_Color color;
 public:
  //Constructor
  Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_BLACK):
    s{s}, center{center}, fontSize{fontSize}, color{color} {}

  //Draw
  void print();

  //Setters and getters
  string getString() {
    return s;
  }
  void setString(const string &newString) {
    s = newString;
  }
  int getFontSize() {
    return fontSize;
  }
  void setFontSize(int newFontSize) {
    fontSize = newFontSize;
  }
  Point getCenter() {
    return center;
  }
  void setCenter(Point newCenter) {
    center = newCenter;
  }
};

void Text::print() {
  fl_color(color);
  fl_font(FL_HELVETICA, fontSize);
  int width, height;
  fl_measure(s.c_str(), width, height, false);
  float x = fl_transform_x(center.x-width/2, center.y-fl_descent()+height/2);
  float y = fl_transform_y(center.x-width/2, center.y-fl_descent()+height/2);
  fl_draw(s.c_str(), x, y);
}



/*--------------------------------------------------
Rectangle class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border
--------------------------------------------------*/


class Rectangle: public Printable {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void print() override;
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
  bool contains(Point p) const ;
  Point getCenter() const {
    return center;
  }
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, w{w}, h{h}, fillColor{fillColor}, frameColor{frameColor} {}

void Rectangle::print() {
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

bool Rectangle::contains(Point p) const  {
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

class Circle: public Printable {
  Point center;
  int r;
  Fl_Color fillColor, frameColor;
 public:
  Circle(Point center, int r,
         Fl_Color frameColor = FL_BLACK,
         Fl_Color fillColor = FL_WHITE);
  void print() override;
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
               Fl_Color fillColor):
  center{center}, r{r}, fillColor{fillColor}, frameColor{frameColor} {}

void Circle::print() {
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

bool Circle::contains(Point p) const  {
  return ( (p.x-center.x)*(p.x-center.x)
           +(p.y-center.y)*(p.y-center.y)<=r*r);
}


/*--------------------------------------------------
TextRectangle
--------------------------------------------------*/



/*--------------------------------------------------
CounterRectangle
--------------------------------------------------*/



/*--------------------------------------------------
ClickableCircle class.
--------------------------------------------------*/


/*--------------------------------------------------

Canvas class.

One instance of the canvas class is made by the
MainWindow class.

The fltk system via MainWindow calls:

print 60 times a second
mouseClick whenever the mouse is clicked
keyPressed whenever a key is pressed

Any drawing code should be called ONLY in print
or methods called by print. If you try to print
elsewhere it will probably crash.
--------------------------------------------------*/


class Canvas {
  vector< shared_ptr<Printable> > printables;
 public:
  Canvas();
  void print();
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};


Canvas::Canvas() {
  printables.push_back(
    make_shared<Text>("Welcome to Lab 9", Point{250, 100}));
}

void Canvas::print() {
  for (auto &c: printables) {
    c->print();
  }
}

void Canvas::mouseClick(Point mouseLoc) {
  for (auto &c: printables) {
    auto x = dynamic_pointer_cast<Clickable>(c);
    if (x)
      if (x->contains(mouseLoc))
        x->onClick();
  }
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
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 9") {
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    canvas.print();
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
