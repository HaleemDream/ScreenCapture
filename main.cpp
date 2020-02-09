#include "gui/canvas.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    canvas gui;
    gui.show();
    return app.exec();
}