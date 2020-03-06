#include "gui/Canvas.h"
#include "network/Server.h"
#include "screen/ScreenCapture.h"

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        // server
        const char* port = argv[1];
        Server server(port);
        ScreenCapture screenCapture;

        while(true)
        {
            printf("Waiting for dimension\n");
            std::pair<int,int> dimension = server.getDimension();
            
            int width = dimension.first;
            int height = dimension.second;
            printf("Received Dimension: (%d, %d) (w,h)\n", width, height);

            struct ScreenCapture::screen screen = screenCapture.getFullscreen();

            printf("Sending img\n");
            server.sendImg(screen.rgb_array, screen.width, screen.height, width, height);
            printf("Done sending img\n");
        }
    }
    else if(argc == 3)
    {
        //client
        QApplication app(argc, argv);
        const char* port = argv[1];
        const char* host = argv[2];
        Canvas canvas(port, host);
        canvas.show();
        return app.exec();
    }

    return 0;
}