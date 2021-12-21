// These should include everything you might use
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <time.h>

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "canvas.h"
#include "polygon.h"

using namespace std;

const int windowWidth = 450;
const int windowHeight = 450;
const double refreshPerSecond = 60;

/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
  Canvas canvas;

 public:
  MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 7") {
    Fl::add_timeout(1.0 / refreshPerSecond, Timer_CB, this);
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
    MainWindow *o = (MainWindow *)userdata;
    o->redraw();
    Fl::repeat_timeout(1.0 / refreshPerSecond, Timer_CB, userdata);
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
