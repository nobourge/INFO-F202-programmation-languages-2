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
using namespace std;

const int windowWidth = 600;
const int windowHeight = 200;
const double refreshPerSecond = 60;

struct Point {
  int x, y;
};

Point eventPoint() {
  return {Fl::event_x(),Fl::event_y()};
}
Point transformedEventPoint() {
  return {
    static_cast<int>(fl_transform_x(Fl::event_x(),Fl::event_y())),
    static_cast<int>(fl_transform_y(Fl::event_x(),Fl::event_y()))};
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
  DrawCanvas Class
  --------------------------------------------------*/


class DrawCanvas {
  struct ColoredStroke{
    Fl_Color color=FL_BLACK;
    vector <Point> stroke; 
    bool closed=false;
  };

  static const unsigned STROKE_COUNT=4;
  static constexpr Fl_Color colors[STROKE_COUNT]={FL_BLACK,FL_BLUE,FL_RED,FL_GREEN};

  int width,height;
  Point corner;
  array<ColoredStroke*, STROKE_COUNT>  strokes;

  bool contains(Point p) const {
    return (p.x>=corner.x && p.y >=corner.y &&
            p.x<=corner.x+width && p.y<=corner.y+height);
  }

  void drawRectangle() const {
    vector<Point> corners{
      {corner.x,corner.y},
      {corner.x+width,corner.y},
      {corner.x+width,corner.y+height},
      {corner.x,corner.y+height}};
    fl_begin_polygon();
    fl_begin_loop();
    for (auto &point:corners)
      fl_vertex(point.x,point.y);
    fl_color(FL_WHITE);
    fl_end_polygon();
    fl_color(FL_BLACK);
    fl_end_loop();
  }

 public:

  DrawCanvas(int width, int height, Point corner=Point{0,0})
    :width{width},height{height},corner{corner}{
      strokes.fill(nullptr);
    }

  void draw() const {
    drawRectangle();
    for (auto &coloredStroke:strokes)
      if (coloredStroke) {
        fl_begin_line();
        for (auto &point:coloredStroke->stroke)
          fl_vertex(point.x,point.y);
        fl_color(coloredStroke->color);
        fl_line_style(0, 3);
        fl_end_line();
        fl_line_style(0);
      }
  }

  bool processEvent(int event) {
    if (contains(transformedEventPoint())) {
      switch (event) {
        case FL_PUSH:
          {
            // Move stroke[i] to stroke[i-1] and recolor
            for (int i=strokes.size()-1;i>=1;i--) {
              strokes[i]=strokes[i-1];
              if (strokes[i]) strokes[i]->color=colors[i];
            }
            // Create a new stroke 0 and add the current point to it
            strokes[0]=new ColoredStroke();
            strokes[0]->stroke.push_back(transformedEventPoint());
            return true;
          }
        case FL_DRAG:
          // Add the current point to stroke 0
          // but only if we are the DrawCanvas in which
          // the stroke started
          if (strokes[0] && !strokes[0]->closed)
            strokes[0]->stroke.push_back(transformedEventPoint());
          return true;
      }
    }
    if ((!contains(transformedEventPoint()) || event==FL_RELEASE)
        && strokes[0]) {
      strokes[0]->closed=true;
      return true;
    }
    return false;
  }
};

/*--------------------------------------------------
  MainWindow class.
  --------------------------------------------------*/

class MainWindow : public Fl_Window {
  const int canvasesCount=4;
  vector<DrawCanvas> drawCanvases;
  vector<Point> offsets;
 public:
  MainWindow() 
    : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 12")
  { 
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    resizable(this);
    // emplace_back may avoid copying/moving only if the vector already
    // contains enough pre-allocated space. reserve pre-allocates space.
    drawCanvases.reserve(canvasesCount);
    for (int i=0;i<canvasesCount;i++){
      drawCanvases.emplace_back(100,150);
      offsets.push_back({25+150*i,25});
    }
  }
  void draw() override {
    Fl_Window::draw();
    for (unsigned i=0;i<drawCanvases.size();++i){
      Translation t{offsets[i]};
      drawCanvases[i].draw();
    } 
  }
  int handle(int event) override {
    bool processed=false;
    if (event==FL_KEYDOWN) {
      switch (Fl::event_key()) {
        case 'q':
          processed=true;
          exit(0);
        case '1': // THIS IS THE COPY
          for (unsigned i=1; i<drawCanvases.size(); i++)
            drawCanvases[i]=drawCanvases[0];
          processed=true;
          break;
        case '2': // THIS IS THE MOVE
          for (unsigned i=drawCanvases.size()-1; i>=1; i--)
            drawCanvases[i]=drawCanvases[i-1];
          drawCanvases[0]=DrawCanvas{100,150};
          processed=true;
          break;
      }
    }
    for (unsigned i=0; i<drawCanvases.size(); ++i) {
      Translation t{{-offsets[i].x,-offsets[i].y}};
      if (drawCanvases[i].processEvent(event))
        processed=true;
    }
    return processed;
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
