
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

using namespace std;

const int windowWidth=500;
const int windowHeight=500;
const double refreshPerSecond=60;

struct Point {
    int x,y;
};

class Rectangle{
    // Add instance variables here
public:
    Rectangle(Point center,int w, int h, 
        Fl_Color frameColor=FL_BLACK, 
        Fl_Color fillColor=FL_WHITE);
    void draw();

    // ADD these methods later
    //void setFillColor(Fl_Color newFillColor);
    //void setFrameColor(Fl_Color newFrameColor);
    //bool contains(Point p);
};

Rectangle::Rectangle(Point center,int w, int h, 
        Fl_Color frameColor, 
        Fl_Color fillColor){}

void Rectangle::draw(){
    // Add drawing code here

    // This draws a filled-in rectangle:
    // fl_draw_box(FL_FLAT_BOX,x,y,width,height,fillColor);

    // This draws the border only:    
    // fl_draw_box(FL_BORDER_FRAME,x,y,width,height,frameColor);   
}



class Cell {
    Rectangle r;
public:
    Cell(Point center,int w, int h);
    void draw();
    void mouseMove(Point mouseLoc);
    void mouseClick(Point mouseLoc);
};

Cell::Cell(Point center,int w, int h):
    r(center,w,h,FL_BLACK,FL_WHITE){}

void Cell::draw(){
    r.draw(); // Draw the rectangle
}
void Cell::mouseMove(Point mouseLoc){
    // Do nothing for now
}
void Cell::mouseClick(Point mouseLoc){
    // Do nothing for now
}

class Canvas{
    Cell c;
public:
    Canvas():c{Point{250,250},100,100}{}; 
    void draw() {
        c.draw();
    }
    void mouseMove(Point mouseLoc) {
        c.mouseMove(mouseLoc);
    }
    void mouseClick(Point mouseLoc){
        c.mouseClick(mouseLoc);        
    }
    void keyPressed(int /*keyCode*/) {exit(0);}
};


/* ------ DO NOT EDIT BELOW HERE (FOR NOW) ------ */
class MainWindow : public Fl_Window {
    Canvas canvas;
    public:
    MainWindow() : Fl_Window(500, 500, windowWidth, windowHeight, "Lab 2") {
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
                canvas.mouseMove(Point{Fl::event_x(),Fl::event_y()});
                return 1;
            case FL_PUSH:
                canvas.mouseClick(Point{Fl::event_x(),Fl::event_y()});
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

int main(int argc, char *argv[]) {
    MainWindow window;
    window.show(argc, argv);
    return Fl::run();
}