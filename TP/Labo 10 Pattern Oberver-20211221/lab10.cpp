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
Clickable class.
--------------------------------------------------*/

class Clickable {
 public:
  virtual bool contains (Point p) const =0;
  virtual void onClick() {};
};

/*--------------------------------------------------
Drawable class.
--------------------------------------------------*/

class Drawable {
 public:
  virtual void draw ()=0;
  virtual ~Drawable()=default;
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
class SendMessessageWhenClicked: public virtual Clickable {
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

class Text: public virtual Drawable {
  string s;
  Point center;
  int fontSize;
  Fl_Color color;
 public:
  //Constructor
  Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_BLACK):
    s{s}, center{center}, fontSize{fontSize}, color{color} {}

  //Draw
  void draw();

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

void Text::draw() {
  fl_color(color);
  fl_font(FL_HELVETICA, fontSize);
  int width{0}, height{0}; //Note: in previous versions this was not set to 0
  // which could cause problems
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


class Rectangle: public virtual Drawable, public virtual Clickable {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_WHITE);
  void draw() override;
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
  bool contains(Point p) const override;
  Point getCenter() const {
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

class Circle: public virtual Drawable, public virtual Clickable {
  Point center;
  int r;
  Fl_Color fillColor, frameColor;
 public:
  Circle(Point center, int r,
         Fl_Color frameColor = FL_BLACK,
         Fl_Color fillColor = FL_WHITE);
  void draw() override;
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  bool contains(Point p) const override;
  Point getCenter() const {
    return center;
  }
};

Circle::Circle(Point center, int r,
               Fl_Color frameColor,
               Fl_Color fillColor):
  center{center}, r{r}, fillColor{fillColor}, frameColor{frameColor} {}

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

bool Circle::contains(Point p) const  {
  return ( (p.x-center.x)*(p.x-center.x)
           +(p.y-center.y)*(p.y-center.y)<=r*r);
}


/*--------------------------------------------------
TextRectangle
--------------------------------------------------*/

class TextRectangle: public Text, public Rectangle {
 public:
  TextRectangle(Point center, int w, int h, string text, int fontSize = 10)
      : Text{text, center, fontSize}, Rectangle{center, w, h} {}
  void draw() override {
    Rectangle::draw();
    Text::draw();
  }
  ~TextRectangle() override {}
};

/*--------------------------------------------------
IntRectangle

The CounterRectangle from lab9sol has been split into
IntRectangle and CounterRectangle, which inherits from IntRectangle

IntRectangle simply displays an integer in a retangle and has
getters and setters, and can be converted to an integer.

CounterRectangle responds to messages and increments or decrements
the integer.
--------------------------------------------------*/

class IntRectangle: 
    public TextRectangle { 
  int theInteger = 0;
  void update() {
    setString(to_string(theInteger));
  }
 public:
  IntRectangle(Point center, int w, int h, int theInteger, int textSize=10)
      : TextRectangle{center, w, h, "",textSize}, theInteger{theInteger} {update();}
  operator int(){return theInteger;}
  int getInteger(){return theInteger;}
  void setInteger(int newInteger){theInteger=newInteger;update();}
};

/*--------------------------------------------------
CounterRectangle
--------------------------------------------------*/

class CounterRectangle: 
    public IntRectangle, 
    public virtual MessageReceiver {
 public:
  CounterRectangle(Point center, int w, int h, int textSize=10)
      : IntRectangle{center, w, h, 0,textSize} {}
  void receiveMessage(const string &message) override {
    if (message=="increment") {
      setInteger(*this+1);
    } else if (message=="decrement") {
      setInteger(*this-1);
    } else {
      cerr << "Unkown message received" << endl;
      exit(1);
    }
  }
};


/*--------------------------------------------------
ClickableCircle class.
--------------------------------------------------*/

class ClickableCircle: public virtual Circle, public virtual SendMessessageWhenClicked {
 public:
  ClickableCircle(Point center, int r,
                  Fl_Color frameColor,
                  Fl_Color fillColor,
                  string message,
                  shared_ptr<MessageReceiver> receiver)
      : Circle{center, r, frameColor, fillColor},
      SendMessessageWhenClicked{message, receiver} {}
  ~ClickableCircle() {};
};

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


class Canvas : public Drawable {
  vector< shared_ptr<Drawable> > drawables;
 public:
  Canvas();
  void draw();
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};


Canvas::Canvas() {

  int textSize=20;

  // These are the three counters on the bottom and the buttons to increment/decrement
  // You don't need to change this
  // The counters will be at drawables[0], drawables[3] and drawables[6]
  for (int x=125,y=400;x<500;x+=125){
    auto counterRect=make_shared<CounterRectangle>(Point{x, y}, 50, 50,textSize);
    drawables.push_back(counterRect);
    drawables.push_back(
      make_shared<ClickableCircle>(
       Point{x-40, y}, 15, FL_BLACK, FL_RED, "decrement",counterRect));
    drawables.push_back(
      make_shared<ClickableCircle>(
        Point{x+40, y}, 15, FL_BLACK, FL_GREEN, "increment",counterRect));
  }

  // These are the two mid-level sums
  // Right now these are just 0
  // You need to change IntRectangle to a new class SumRectangle
  // that uses the observer design pattern to be updated
  // You will need to change the constructor parameters to whatever your class needs
  drawables.push_back(make_shared<IntRectangle>(Point{150,250},50,50,0,textSize));
  drawables.push_back(make_shared<IntRectangle>(Point{350,250},50,50,0,textSize));

  // This is the product on the top
  // Right now these are just 0
  // You need to change IntRectangle to a new class MultRectangle
  // that uses the observer design pattern to be updated
  // You will need to change the constructor parameters to whatever your class needs
  drawables.push_back(make_shared<IntRectangle>(Point{250,150},100,50,0,textSize));


  //These are the text labels
  drawables.push_back(
    make_shared<TextRectangle>(Point{250, 60}, 250, 50, "Welcome to Lab 10",textSize));


  drawables.push_back(
    make_shared<Text>("x",Point{125, 450},textSize));
  drawables.push_back(
    make_shared<Text>("y",Point{250, 450},textSize));
  drawables.push_back(
    make_shared<Text>("z",Point{375, 450},textSize));

  drawables.push_back(
    make_shared<Text>("x+y",Point{150, 300},textSize));
  drawables.push_back(
    make_shared<Text>("y+z",Point{350, 300},textSize));

  drawables.push_back(
    make_shared<Text>("(x+y)*(y+z)",Point{250, 200},textSize));

    
}

void Canvas::draw() {
  for (auto &c: drawables) {
    c->draw();
  }
}

void Canvas::mouseClick(Point mouseLoc) {
  for (auto &c: drawables) {
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
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 10") {
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
