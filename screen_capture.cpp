#include "screen_capture.h"

using namespace cimg_library;

screen_capture::screen_capture() :
display(XOpenDisplay(NULL)),
root(DefaultRootWindow(display))
{
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    WINDOW_WIDTH = gwa.width;
    WINDOW_HEIGHT = gwa.height;
}

screen_capture::~screen_capture()
{

}

screen_capture::RGB* screen_capture::fullscreen()
{
    return 0;
}