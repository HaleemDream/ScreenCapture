#include "draw_canvas.h"
#include "screen_capture.h"

using namespace cimg_library;

// TODO - remove display, root once we don't need dimensions
draw_canvas::draw_canvas(QWidget* parent): 
QMainWindow(parent),
display(XOpenDisplay(NULL)),
root(DefaultRootWindow(display))
{
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);

    // TODO - remove once pass in dimensions from screen_capture
    WINDOW_WIDTH = gwa.width;
    WINDOW_HEIGHT = gwa.height;

    // need var for canvas x,y
    setGeometry(0, 0, 800, 600);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start();
}

void draw_canvas::paintEvent(QPaintEvent*)
{
    QImage background(size(), QImage::Format_RGB888);
    screen_capture::RGB* array = sc.fullscreen();

    int width = 0;
    int height = 0;

    // TODO - pass window dimensions from screen_capture
    for(int x = 0; x < 800; x++)
    {
        width = ((int) (x / 800.0 * WINDOW_WIDTH)) * WINDOW_HEIGHT;
        for(int y = 0; y < 600; y++)
        {
            height = (int) (y / 600.0 * WINDOW_HEIGHT);
            screen_capture::RGB rgb = array[width + height];
            QRgb qrgb = qRgb(rgb.red, rgb.green, rgb.blue);
            background.setPixel(x, y, qrgb);
        }
    }

    delete[] array;
    QPainter paint(this);
    paint.drawImage(0, 0, background);
}