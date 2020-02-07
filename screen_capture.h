#ifndef _SCREEN_CAPTURE_H_
#define _SCREEN_CAPTURE_H_

#include <cstdio>
#include <thread>
#include <vector>

#include <CImg/CImg.h>
#include <X11/Xlib.h>
#include <X11/X.h>

#define MAX_THREADS_PER_ROW   500
#define MAX_THRADS_PER_COLUMN 500

class screen_capture
{
    public:
        screen_capture();
        ~screen_capture();

        // struct to hold RGB values
        struct RGB
        {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
        };

        // pointer array of XImage
        static RGB* fullscreen();

    private:
        // X11 components
        Display* display;
        Window root;
        int WINDOW_HEIGHT;
        int WINDOW_WIDTH;

        // threads to help read img
        std::vector<std::thread>> image_capture_threads;
};

#endif