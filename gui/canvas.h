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

#include "../screen/screen_capture.h"

class canvas : public QMainWindow
{
    Q_OBJECT
    public:
        canvas(QWidget* parent = 0);
        void paintEvent(QPaintEvent*);
        void resizeEvent(QResizeEvent*);
    private:
        QTimer* timer;
        screen_capture sc;
        int canvas_width;
        int canvas_height;
};

#endif