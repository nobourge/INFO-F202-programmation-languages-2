
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

using namespace std;

const int windowWidth = 500;
const int windowHeight = 500;
const double refreshPerSecond = 60;

/*-- EDIT THE FUNCTIONS HERE AND ADD YOUR OWN */

// Init is called once a the beginning of the program
// Do not draw anything in init
void init() {
}

// Global variables
int times = 0;           // Incremented every draw
const int boxSize = 20;  // Size of the box we will draw

void draw1() {
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(Fl_Boxtype::FL_FLAT_BOX,
                windowWidth / 2,
                windowHeight / 2, boxSize, boxSize, boxColor);
    times += 1;
}

void draw2() {
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX,
                windowWidth / 2 + 100.0 * sin(times / 50.0),
                windowHeight / 2 + 100.0 * cos(times / 50.0), boxSize, boxSize, boxColor);
    times += 1;
}

struct Point {
    float x;
    float y;
};
Point mousePos;  // Need to remember where the mouse is

void draw3() {
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX, mousePos.x - boxSize / 2, mousePos.y - boxSize / 2, boxSize, boxSize, boxColor);
}

Point boxPos = {0.0, 0.0};  // Need to remember where the box is

void draw4() {
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX, boxPos.x, boxPos.y, boxSize, boxSize, boxColor);
    boxPos.x -= (boxPos.x - mousePos.x + boxSize / 2) / 100;
    boxPos.y -= (boxPos.y - mousePos.y + boxSize / 2) / 100;
    times += 1;
}

void draw5() {
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX, boxPos.x, boxPos.y, boxSize, boxSize, boxColor);
    boxPos.x -= (boxPos.x - mousePos.x + boxSize / 2) / 100;
    boxPos.y -= (boxPos.y - mousePos.y + boxSize / 2) / 100;
    times += 1;
    if (mousePos.x - boxPos.x < boxSize &&
        mousePos.x - boxPos.x > 0 &&
        mousePos.y - boxPos.y < boxSize &&
        mousePos.y - boxPos.y > 0) {
        boxPos.x = rand() % windowWidth;
        boxPos.y = rand() % windowHeight;
    }
}

vector<Point> craters;  // Need to store the craters

void draw6() {
    Fl_Color craterColor = fl_rgb_color(200, 100, 100);
    for (auto crater : craters) {
        fl_draw_box(FL_FLAT_BOX, crater.x, crater.y, boxSize, boxSize, craterColor);
    }
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX, boxPos.x, boxPos.y, boxSize, boxSize, boxColor);
    boxPos.x -= (boxPos.x - mousePos.x + boxSize / 2) / 100;
    boxPos.y -= (boxPos.y - mousePos.y + boxSize / 2) / 100;
    times += 1;
    if (mousePos.x - boxPos.x < boxSize &&
        mousePos.x - boxPos.x > 0 &&
        mousePos.y - boxPos.y < boxSize &&
        mousePos.y - boxPos.y > 0) {
        craters.push_back(boxPos);
        boxPos.x = rand() % windowWidth;
        boxPos.y = rand() % windowHeight;
    }
}

int explode = 0;   // Status of the explosion 0=over, 20=just started
Point explodePos;  // Location of the explosion

void draw8() {
    Fl_Color explodeColor = fl_rgb_color(250, 200, 200);
    if (explode) {
        int explodeRad = 5 * (20 - explode);
        fl_draw_box(FL_FLAT_BOX,
                    explodePos.x - explodeRad,
                    explodePos.y - explodeRad,
                    2 * explodeRad,
                    2 * explodeRad, explodeColor);
        explode--;
    }
    Fl_Color craterColor = fl_rgb_color(200, 100, 100);
    for (auto crater : craters) {
        fl_draw_box(FL_FLAT_BOX, crater.x, crater.y, boxSize, boxSize, craterColor);
    }
    Fl_Color boxColor = fl_rgb_color(100, 200, 100);
    fl_draw_box(FL_FLAT_BOX, boxPos.x, boxPos.y, boxSize, boxSize, boxColor);
    boxPos.x -= (boxPos.x - mousePos.x + boxSize / 2) / 100;
    boxPos.y -= (boxPos.y - mousePos.y + boxSize / 2) / 100;
    times += 1;
    if (mousePos.x - boxPos.x < boxSize &&
        mousePos.x - boxPos.x > 0 &&
        mousePos.y - boxPos.y < boxSize &&
        mousePos.y - boxPos.y > 0) {
        explodePos = boxPos;
        explode = 20;
        craters.push_back(boxPos);
        boxPos.x = rand() % windowWidth;
        boxPos.y = rand() % windowHeight;
    }
}

char whichDemo = '1';  // Remembers which demo is currently running

// Draw is called 60 times a second.
// You should draw what the use should see here
// Every time it starts with a blank screen
void draw() {
    switch (whichDemo) {
        case '1':
            draw1();
            break;
        case '2':
            draw2();
            break;
        case '3':
            draw3();
            break;
        case '4':
            draw4();
            break;
        case '5':
            draw5();
            break;
        case '6':
        case '7':
            draw6();
            break;
        case '8':
            draw8();
            break;
    }
}

// mouseMove is called every time the mouse moves
// It is called with the current mouse position
void mouseMove(int x, int y) {
    mousePos.x = x;
    mousePos.y = y;
}

// keyPressed is called when a key is pressed
// A key code is passed indicating which key
// This is the ASCII value for normal keys
// For special keys like arrow, find the key codes:
// https://www.fltk.org/doc-1.3/enumerations.html
void keyPressed(int keyCode) {
    switch (keyCode) {
        case ' ':
            if (whichDemo >= '7') craters.clear();
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            whichDemo = keyCode;
            break;
        default:
            exit(0);
    }
}

/* ------ DO NOT EDIT BELOW HERE (FOR NOW) ------ */
class MainWindow : public Fl_Window {
   public:
    MainWindow() : Fl_Window(000, 000, windowWidth, windowHeight, "Lab 1") {
        Fl::add_timeout(1.0 / refreshPerSecond, Timer_CB, this);
        resizable(this);
    }
    void draw() override {
        Fl_Window::draw();
        ::draw();  //Global draw function
    }
    int handle(int event) override {
        switch (event) {
            case FL_MOVE:
                mouseMove(Fl::event_x(), Fl::event_y());
                return 1;
            case FL_KEYDOWN:
                keyPressed(Fl::event_key());
        }
        return 0;
    }
    static void Timer_CB(void *userdata) {
        MainWindow *o = (MainWindow *)userdata;
        o->redraw();
        Fl::repeat_timeout(1.0 / refreshPerSecond, Timer_CB, userdata);
    }
};

int main(int argc, char *argv[]) {
    init();
    MainWindow window;
    window.show(argc, argv);
    return Fl::run();
}