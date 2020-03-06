#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QTimer>

#include <cstdio>
#include <thread>
#include <vector>

#include <CImg/CImg.h>
#include <X11/Xlib.h>
#include <X11/X.h>

#include "../network/Client.h"
#include "../screen/ScreenCapture.h"

#define FPS 30
#define REPAINT_TIME_MS 1000/FPS

class Canvas : public QMainWindow
{
    Q_OBJECT
    public:
        Canvas(const char* port, const char* host, QWidget* parent = 0);
        void paintEvent(QPaintEvent*);
        void resizeEvent(QResizeEvent*);
    private:
        void setupTimer();
        void displayImgFromRemote(QImage& background);
        void displayImgFromSelf(QImage& background);
        QTimer* timer;
        Client client;
        ScreenCapture screenCapture;
        int canvasWidth;
        int canvasHeight;
};

#endif