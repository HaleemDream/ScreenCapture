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

class ScreenCapture
{
    public:
        ScreenCapture();
        ~ScreenCapture();

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

        struct screen getFullscreen();

    private:
        // X11 components
        Display* display;
        Window root;
        int WINDOW_HEIGHT;
        int WINDOW_WIDTH;

        // threads to help read img
        std::vector<std::thread> image_capture_threads;

        void fillRGB(XImage* image, RGB* arr, int row_start, int row_end, int col_start, int col_end);
};

#endif