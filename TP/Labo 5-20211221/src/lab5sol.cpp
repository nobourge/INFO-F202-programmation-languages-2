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

const int windowWidth = 1000;
const int windowHeight = 500;
const double refreshPerSecond = 60;


struct Point {
  int x, y;
};

/*--------------------------------------------------

Text class.

Use to display text on the screen. For example:

Text("Hello!", {250, 250}).draw();

will create a text temporary instance with the
string "Hello!" centered at 250, 250 and will
call draw on the temporary.

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

class Text {
  Point center;
  string s;
  int fontSize;
  Fl_Color color;
 public:
  //Constructor
  Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_BLACK):
    center{center}, s{s}, fontSize{fontSize}, color{color} {}

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
  int width, height;
  fl_measure(s.c_str(), width, height, false);
  float x = fl_transform_x(center.x-width/2, center.y-fl_descent()+height/2);
  float y = fl_transform_y(center.x-width/2, center.y-fl_descent()+height/2);
  fl_draw(s.c_str(), x, y);
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

Line Class
--------------------------------------------------*/


class Line {
  Point p1, p2;
  Fl_Color color;
 public:
  Line(Point p1, Point p2, Fl_Color color = FL_BLACK);
  void draw();
  void setColor(Fl_Color newColor);
};

Line::Line(Point p1, Point p2, Fl_Color color)
    : p1{p1}, p2{p2}, color{color} {}

void Line::draw() {
  fl_color(color);
  fl_begin_line();
  for (auto &point: {p1, p2})
    fl_vertex(point.x, point.y);
  fl_end_line();
}

void Line::setColor(Fl_Color newColor) {
  color = newColor;
}


/*--------------------------------------------------

Tree Class
--------------------------------------------------*/

class Tree {
  Tree *left, *right;
  Line line;
 public:
  Tree();
  Tree(Tree *left, Tree *right, Line = Line{{0, 0}, {0, -30}});
  void draw();
  void setColor(Fl_Color newColor);
//Solution starts here
  Tree(const Tree &other);
  Tree& operator=(const Tree &other);
  ~Tree();
};

Tree::Tree(): Tree{nullptr, nullptr} {}

Tree::Tree(Tree *left, Tree *right, Line line)
    : left{left ? new Tree{*left} : nullptr},
      right{right ? new Tree{*right} : nullptr},
      line{line}
{}

Tree::Tree(const Tree& other)
    : Tree(other.left, other.right, other.line)
{}

Tree::~Tree() {
  delete left;
  delete right;
}

Tree& Tree::operator=(const Tree &other) {
  delete left;
  delete right;
  left=other.left ? new Tree{*(other.left)} : nullptr;
  right=other.right ? new Tree{*(other.right)} : nullptr;
  line=other.line;
  return *this;
}

void Tree::draw() {
  line.draw();
  Translation t({0, -30});
  if (left) {
    Rotation r({0, 0}, -10);
    left->draw();
  }
  if (right) {
    Rotation r({0, 0}, 10);
    right->draw();
  }
}

void Tree::setColor(Fl_Color newColor) {
  line.setColor(newColor);
  if (left) left->setColor(newColor);
  if (right) right->setColor(newColor);
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
  vector<Tree *> T;
  Fl_Window *fltkWindow;
 public:
  Canvas(Fl_Window *fltkWindow);
  ~Canvas();
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
};


Canvas::Canvas(Fl_Window *fltkWindow): fltkWindow{fltkWindow} {
  T.push_back(new Tree);
  T.push_back(new Tree);
  for (int i=0; i<9; i++)
    T.push_back(new Tree{T[i], T[i+1]});

  //TASK 1:
  T[5]->setColor(FL_RED);

  //TASK 2:
  T.push_back(new Tree(*T[3]));
  T[11]->setColor(FL_BLUE);

  //TASK 3:
  *T[8]=*T[2];
  T[8]->setColor(FL_MAGENTA);

  //TASK 4:
  swap(*T[1], *T[3]);

  //TASK 5:
  //press q to quit without generating errors
}

void Canvas::draw() {
  // Do not change this
  auto x = 25;
  for (auto i=0u; i<T.size(); ++i) {
    Translation t({x, 400});
    T[i]->draw();
    x+=20+i*10;
    Text{"T["+to_string(i)+"]", {0, 50}}.draw();
  }
}

Canvas::~Canvas() {
  while (!T.empty()) {
    delete T.back();
    T.pop_back();
  }
}

void Canvas::mouseMove(Point /*mouseLoc*/) {
}

void Canvas::mouseClick(Point /*mouseLoc*/) {
}

void Canvas::keyPressed(int keyCode) {
  switch (keyCode) {
    case 'q':
      fltkWindow->hide();
      break;
  }
}


/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
  Canvas canvas{this};
 public:
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 5") {
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
