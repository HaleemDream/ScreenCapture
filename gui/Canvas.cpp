#include "Canvas.h"

using namespace cimg_library;

Canvas::Canvas(const char* port, const char* host, QWidget* parent): 
QMainWindow(parent),
client(port, host)
{
    canvasWidth = 800;
    canvasHeight = 600;
    setGeometry(0, 0, canvasWidth, canvasHeight);
    setupTimer();
}

void Canvas::paintEvent(QPaintEvent*)
{
    QImage background(size(), QImage::Format_RGB888);
    displayImgFromRemote(background);
    QPainter paint(this);
    paint.drawImage(0, 0, background);
}

void Canvas::resizeEvent(QResizeEvent*)
{
    canvasWidth = width();
    canvasHeight = height();
}

//repaint
void Canvas::setupTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
    timer->start(REPAINT_TIME_MS);
}

void Canvas::displayImgFromRemote(QImage& background)
{
    client.sendDimension();
    unsigned char* img = client.getImg();

    if(img)
    {
        for(int x = 0; x < canvasWidth; x++)
        {
            for(int y = 0; y < canvasHeight; y++)
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

void Canvas::displayImgFromSelf(QImage&  background)
{
    ScreenCapture::screen screen = screenCapture.getFullscreen();
    ScreenCapture::RGB* img = screen.rgb_array;
    
    int screenWidth = screen.width;
    int screenHeight = screen.height;

    int x_offset;
    int y_offset;

    for(int x = 0; x < canvasWidth; x++)
    {
        y_offset = ((int) ((double) x / canvasWidth * screenWidth)) * screenHeight;
        for(int y = 0; y < canvasHeight; y++)
        {
            x_offset = (int) ((double) y / canvasHeight * screenHeight);
            ScreenCapture::RGB rgb = img[x_offset + y_offset];
            QRgb qrgb = qRgb(rgb.red, rgb.green, rgb.blue);
            background.setPixel(x, y, qrgb);
        }
    }

    delete[] img;
}