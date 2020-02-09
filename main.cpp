#include "draw_canvas.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    draw_canvas draw;
    draw.show();
    return app.exec();
}