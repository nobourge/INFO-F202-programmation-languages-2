
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


/*-- EDIT THE FUNCTIONS HERE AND ADD YOUR OWN */

// Init is called once a the beginning of the program
// Do not draw anything in init
void init(){
    
}


// Draw is called 60 times a second.
// You should draw what the use should see here
// Every time it starts with a blank screen
void draw(){

}

// mouseMove is called every time the mouse moves
// It is called with the current mouse position
void mouseMove(int x, int y){

}

// keyPressed is called when a key is pressed
// A key code is passed indicating which key
// This is the ASCII value for normal keys
// For special keys like arrow, find the key codes:
// https://www.fltk.org/doc-1.3/enumerations.html
void keyPressed(int keyCode){
    exit(0);
}

/* ------ DO NOT EDIT BELOW HERE (FOR NOW) ------ */
class MainWindow : public Fl_Window {
    public:
    MainWindow() : Fl_Window(000, 000, windowWidth, windowHeight, "Lab 1") {
        Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
        resizable(this);
    }
    void draw() override {
        Fl_Window::draw();
        ::draw(); //Global draw function
    }
    int handle(int event) override {
        switch (event) {
            case FL_MOVE:
                mouseMove(Fl::event_x(),Fl::event_y());
                return 1;
            case FL_KEYDOWN:
                keyPressed(Fl::event_key());
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
    init();
    MainWindow window;
    window.show(argc, argv);
    return Fl::run();
}