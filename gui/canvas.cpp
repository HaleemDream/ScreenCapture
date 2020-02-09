#include "canvas.h"

using namespace cimg_library;

canvas::canvas(QWidget* parent): 
QMainWindow(parent)
{
    canvas_width = 800;
    canvas_height = 600;
    
    setGeometry(0, 0, canvas_width, canvas_height);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start();
}

void canvas::paintEvent(QPaintEvent*)
{
    QImage background(size(), QImage::Format_RGB888);
    screen_capture::screen screen = sc.fullscreen();

    screen_capture::RGB* image_array = screen.rgb_array;
    // TODO - better names
    int screen_width = screen.width;
    int screen_height = screen.height;

    int x_offset;
    int y_offset;

    for(int x = 0; x < canvas_width; x++)
    {
        y_offset = ((int) ((double) x / canvas_width * screen_width)) * screen_height;
        for(int y = 0; y < canvas_height; y++)
        {
            x_offset = (int) ((double) y / canvas_height * screen_height);
            screen_capture::RGB rgb = image_array[x_offset + y_offset];
            QRgb qrgb = qRgb(rgb.red, rgb.green, rgb.blue);
            background.setPixel(x, y, qrgb);
        }
    }

    delete[] image_array;
    QPainter paint(this);
    paint.drawImage(0, 0, background);
}

void canvas::resizeEvent(QResizeEvent*)
{
    canvas_width = width();
    canvas_height = height();
}