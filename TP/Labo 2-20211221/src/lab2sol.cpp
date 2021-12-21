
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
    Point center;
    int w,h;
    Fl_Color fillColor, frameColor;
public:
    Rectangle(Point center,int w, int h, 
        Fl_Color frameColor=FL_BLACK, 
        Fl_Color fillColor=FL_WHITE);
    void draw();
    void setFillColor(Fl_Color newFillColor);
    void setFrameColor(Fl_Color newFrameColor);
    bool contains(Point p);
};

Rectangle::Rectangle(Point center,int w, int h, 
        Fl_Color frameColor, 
        Fl_Color fillColor):
        center{center},w{w},h{h},fillColor{fillColor},frameColor{frameColor}{}

void Rectangle::draw(){
    fl_draw_box(FL_FLAT_BOX,center.x-w/2,center.y-h/2,w,h,fillColor);    
    fl_draw_box(FL_BORDER_FRAME,center.x-w/2,center.y-h/2,w,h,frameColor);   
}

void Rectangle::setFillColor(Fl_Color newFillColor){
    fillColor=newFillColor;
}

void Rectangle::setFrameColor(Fl_Color newFrameColor){
    frameColor=newFrameColor;
}

bool Rectangle::contains(Point p){
    return p.x>=center.x-w/2 &&
        p.x<center.x+w/2 &&
        p.y>=center.y-h/2 &&
        p.y<center.y+h/2;
}


class Cell {
    Rectangle r;
    bool on=false;
public:
    Cell(Point center,int w, int h);
    void draw();
    void mouseMove(Point mouseLoc);
    void mouseClick(Point mouseLoc);
};

Cell::Cell(Point center,int w, int h):
    r(center,w,h,FL_BLACK,FL_WHITE){}
void Cell::draw(){
    r.draw();
}
void Cell::mouseMove(Point mouseLoc){
    if (r.contains(mouseLoc)){
        r.setFrameColor(FL_RED);
    }
    else {
        r.setFrameColor(FL_BLACK);
    }

}
void Cell::mouseClick(Point mouseLoc){
    if (r.contains(mouseLoc)){
        on = !on;
        if (on)
            r.setFillColor(FL_YELLOW);
        else
            r.setFillColor(FL_WHITE);

    }
}

class Canvas{
    vector<Cell> cells;
public:
    Canvas();
    void draw();
    void mouseMove(Point mouseLoc);
    void mouseClick(Point mouseLoc);
    void keyPressed(int /*keyCode*/) {exit(0);}
};


Canvas::Canvas(){
    for (int i=0;i<100;i++)
        cells.push_back(Cell{Point{50*(i%10)+25,50*(i/10)+25},40,40});
// You could also write:
//        cells.push_back({Point{50*(i%10)+25,50*(i/10)+25},40,40});
}    
void Canvas::draw() {
    for (auto &c:cells) c.draw();
}
void Canvas::mouseMove(Point mouseLoc) {
    for (auto &c:cells) c.mouseMove(mouseLoc);
}
void Canvas::mouseClick(Point mouseLoc){
    for (auto &c:cells) c.mouseClick(mouseLoc);        
}

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