#include <chrono>
#include <cstdint>

#include "ScreenCapture.h"

using namespace cimg_library;

// TODO - need to handle case when user changes screen resolution
// Maybe this is fine, need to inspect later
ScreenCapture::ScreenCapture() :
display(XOpenDisplay(NULL)),
root(DefaultRootWindow(display)),
image_capture_threads(THREAD_COUNT)
{
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    WINDOW_WIDTH = gwa.width;
    WINDOW_HEIGHT = gwa.height;
}

ScreenCapture::~ScreenCapture()
{

}

struct ScreenCapture::screen ScreenCapture::getFullscreen()
{
    XImage* image = XGetImage(display, root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, AllPlanes, ZPixmap);
    RGB* rgb = new RGB[WINDOW_HEIGHT * WINDOW_WIDTH];

    int row_offset = WINDOW_HEIGHT / THREAD_COUNT;
    int row_start = 0;
    int row_end = 0;
    
    // start threads to get sub-images
    for(size_t i = 0; i < image_capture_threads.size(); i++)
    {
        row_start = i * row_offset;
        row_end = i == image_capture_threads.size() - 1 ? WINDOW_HEIGHT : row_start + row_offset;
        image_capture_threads[i] = std::thread(&ScreenCapture::fillRGB, this, image, rgb, 0, WINDOW_WIDTH, row_start, row_end);
    }
    
    // wait for threads to finish
    for(std::thread& thread : image_capture_threads)
    {
        thread.join();
    }

    XDestroyImage(image);
    return ScreenCapture::screen{rgb, WINDOW_WIDTH, WINDOW_HEIGHT};
}

void ScreenCapture::fillRGB(XImage* image, RGB* arr, int row_start, int row_end, int col_start, int col_end)
{
   unsigned long red_mask = image->red_mask;
   unsigned long green_mask = image->green_mask;
   unsigned long blue_mask = image->blue_mask;

   for (int x = row_start; x < row_end; x++)
      for (int y = col_start; y < col_end ; y++)
      {
         uint32_t pixel = XGetPixel(image,x,y);

         unsigned char blue = pixel & blue_mask;
         unsigned char green = (pixel & green_mask) >> 8;
         unsigned char red = (pixel & red_mask) >> 16;

         // TODO - this only works when entire column is passed 
         // and we offset the rows. Should handle both
         arr[x * WINDOW_HEIGHT + y] = RGB{red, green, blue};
      }
}