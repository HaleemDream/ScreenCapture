#include "screen_capture.h"

#include <ctime>
#include <ratio>
#include <chrono>

using namespace cimg_library;

// TODO - need to handle case when user changes screen resolution
// Maybe this is fine, need to inspect later
screen_capture::screen_capture() :
display(XOpenDisplay(NULL)),
root(DefaultRootWindow(display)),
image_capture_threads(THREAD_COUNT)
{
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    WINDOW_WIDTH = gwa.width;
    WINDOW_HEIGHT = gwa.height;
}

screen_capture::~screen_capture()
{

}

screen_capture::screen screen_capture::fullscreen()
{
    XImage* image = XGetImage(display, root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, AllPlanes, ZPixmap);
    RGB* rgb_array = new RGB[WINDOW_HEIGHT * WINDOW_WIDTH];

    int row_offset = WINDOW_HEIGHT / THREAD_COUNT;
    int row_start = 0;
    int row_end = 0;
    
    // start threads to get sub-images
    for(size_t i = 0; i < image_capture_threads.size(); i++)
    {
        row_start = i * row_offset;
        row_end = i == image_capture_threads.size() - 1 ? WINDOW_HEIGHT : row_start + row_offset;
        image_capture_threads[i] = std::thread(&screen_capture::fill_rgb_array, this, image, rgb_array, 0, WINDOW_WIDTH, row_start, row_end);
    }
    
    // wait for threads to finish
    for(std::thread& thread : image_capture_threads)
    {
        thread.join();
    }

    XDestroyImage(image);
    
    return screen_capture::screen{rgb_array, WINDOW_WIDTH, WINDOW_HEIGHT};
}

void screen_capture::fill_rgb_array(XImage* image, RGB* arr, int row_start, int row_end, int col_start, int col_end)
{
   unsigned long red_mask = image->red_mask;
   unsigned long green_mask = image->green_mask;
   unsigned long blue_mask = image->blue_mask;

   for (int x = row_start; x < row_end; x++)
      for (int y = col_start; y < col_end ; y++)
      {
         unsigned long pixel = XGetPixel(image,x,y);

         unsigned char blue = pixel & blue_mask;
         unsigned char green = (pixel & green_mask) >> 8;
         unsigned char red = (pixel & red_mask) >> 16;

         // TODO - this only works when entire column is passed 
         // and we offset the rows. Should handle both
         arr[x * WINDOW_HEIGHT + y] = RGB{red, green, blue};
      }
}