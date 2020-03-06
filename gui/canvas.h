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

#include "../network/client.h"
#include "../screen/screen_capture.h"

#define FPS 30
#define REPAINT_TIME_MS 1000/FPS

class canvas : public QMainWindow
{
    Q_OBJECT
    public:
        canvas(const char* port, const char* host, QWidget* parent = 0);
        void paintEvent(QPaintEvent*);
        void resizeEvent(QResizeEvent*);
    private:
        void setup_timer();
        void display_img_from_remote(QImage& background);
        void display_img_from_self(QImage& background);
        client cli;
        QTimer* timer;
        screen_capture sc;
        int canvas_width;
        int canvas_height;
};

#endif