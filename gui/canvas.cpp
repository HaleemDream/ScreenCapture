#include "canvas.h"

using namespace cimg_library;

canvas::canvas(const char* port, const char* host, QWidget* parent): 
QMainWindow(parent),
cli(port, host)
{
    canvas_width = 800;
    canvas_height = 600;
    setGeometry(0, 0, canvas_width, canvas_height);
    setup_timer();
}

void canvas::paintEvent(QPaintEvent*)
{
    QImage background(size(), QImage::Format_RGB888);
    display_img_from_remote(background);
    QPainter paint(this);
    paint.drawImage(0, 0, background);
}

void canvas::resizeEvent(QResizeEvent*)
{
    canvas_width = width();
    canvas_height = height();
}

//repaint
void canvas::setup_timer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(REPAINT_TIME_MS);
}

void canvas::display_img_from_remote(QImage& background)
{
    cli.send_dimension();
    unsigned char* img = cli.get_img();

    if(img)
    {
        for(int x = 0; x < canvas_width; x++)
        {
            for(int y = 0; y < canvas_height; y++)
            {
                unsigned char red = *img++;
                unsigned char green = *img++;
                unsigned char blue = *img++;

                QRgb qrgb = qRgb(red, green, blue);
                background.setPixel(x, y, qrgb);
            }
        }
    }
}

void canvas::display_img_from_self(QImage&  background)
{
    screen_capture::screen screen = sc.get_fullscreen();
    screen_capture::RGB* image_array = screen.rgb_array;
    
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
}