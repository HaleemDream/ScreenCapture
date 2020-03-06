#include "gui/canvas.h"
#include "network/server.h"
#include "screen/screen_capture.h"

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        // server
        const char* port = argv[1];
        server server(port);
        screen_capture sc;

        while(true)
        {
            printf("Waiting for dimension\n");
            std::pair<int,int> dimension = server.get_dimension();
            
            int width = dimension.first;
            int height = dimension.second;
            printf("Received Dimension: (%d, %d) (w,h)\n", width, height);

            struct screen_capture::screen screen = sc.get_fullscreen();

            printf("Sending img\n");
            server.send_img(screen.rgb_array, screen.width, screen.height, width, height);
            printf("Done sending img\n");
        }
    }
    else if(argc == 3)
    {
        //client
        QApplication app(argc, argv);
        const char* port = argv[1];
        const char* host = argv[2];
        canvas gui(port, host);
        gui.show();
        return app.exec();
    }

    return 0;
}