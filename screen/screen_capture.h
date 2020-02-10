#ifndef _SCREEN_CAPTURE_H_
#define _SCREEN_CAPTURE_H_

#include <cstdio>
#include <thread>
#include <vector>

#include <CImg/CImg.h>
#include <X11/Xlib.h>
#include <X11/X.h>

// TODO - improve threading
#define THREAD_COUNT 5

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

        struct screen
        {
            RGB* rgb_array;
            int width;
            int height;
        };

        screen fullscreen();

    private:
        // X11 components
        Display* display;
        Window root;
        int WINDOW_HEIGHT;
        int WINDOW_WIDTH;

        // threads to help read img
        std::vector<std::thread> image_capture_threads;

        void fill_rgb_array(XImage* image, RGB* arr, int row_start, int row_end, int col_start, int col_end);
};

#endif