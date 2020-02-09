#ifndef _DRAW_CANVAS_H_
#define _DRAW_CANVAS_H_

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

#include "screen_capture.h"

class draw_canvas : public QMainWindow
{
    Q_OBJECT
    public:
        draw_canvas(QWidget* parent = 0);
        void paintEvent(QPaintEvent*);
    private:
        QTimer* timer;
        screen_capture sc;
        int canvas_width;
        int canvas_height;
};

#endif