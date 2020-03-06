#include "stdio.h"

#include "Server.h"

Server::Server(const char* port) : port(port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE;    // listen on all network interfaces

    getaddrinfo(0, port, &hints, &bindAddress);
    socketClient = socket(bindAddress->ai_family,
                           bindAddress->ai_socktype,
                           bindAddress->ai_protocol);
    
    // invalid socket
    if(socketClient < 0)
    {
        printf("Error creating socket!\n");
        return;
    }
    
    if(bind(socketClient, bindAddress->ai_addr, bindAddress->ai_addrlen))
    {
        printf("Bind failed!\n");
        return;
    }

    freeaddrinfo(bindAddress);
}

Server::~Server()
{
    closeSocket();
    delete bindAddress;
}

// (width, height)
std::pair<int,int> Server::getDimension()
{
    socklen_t client_len = sizeof(clientAddress);
    int dimension[2];
    recvfrom(socketClient, dimension, sizeof(dimension), 0, (struct sockaddr*) &clientAddress, &client_len);

    int width = dimension[WIDTH_INDEX];
    int height = dimension[HEIGHT_INDEX];

    return std::make_pair(width, height);
}

void Server::sendImg(struct ScreenCapture::RGB* img, int screenWidth, int screenHeight,int canvasWidth, int canvasHeight)
{
    int rgb_count = 0;
    unsigned char payload[PAYLOAD_SIZE];

    socklen_t client_len = sizeof(clientAddress);

    int y_offset;
    int x_offset;

    for(int x = 0; x < canvasWidth; x++)
    {
        y_offset = ((int) ((double) x / canvasWidth * screenWidth)) * screenHeight;
        for(int y = 0; y < canvasHeight; y++)
        {
            x_offset = (int) ((double) y / canvasHeight * screenHeight);
            ScreenCapture::RGB rgb = img[x_offset + y_offset];
                        
            payload[rgb_count * 3] = rgb.red;
            payload[rgb_count * 3 + 1] = rgb.green;
            payload[rgb_count * 3 + 2] = rgb.blue;

            if(++rgb_count == PAYLOAD_SIZE / 3)
            {
                sendto(socketClient, payload, sizeof(payload), 0, (struct sockaddr*) &clientAddress, client_len);
                rgb_count = 0;
            }    
        }
    }
}

void Server::closeSocket()
{
    close(socketClient);
}

void Server::sendTestData()
{
    const char* payload = "hello";
    socklen_t client_len = sizeof(clientAddress);
    sendto(socketClient, payload, strlen(payload), 0, (struct sockaddr*) &clientAddress, client_len);
}

void Server::recvTestData()
{
    char data[1024];
    socklen_t client_len = sizeof(clientAddress);
    int bytes_recv = recvfrom(socketClient, data, 1024, 0, (struct sockaddr*) &clientAddress, &client_len);

    printf("received: %.*s\n", bytes_recv, data);
}

// for testing
// int main(int argc, char** argv)
// {
//     server server("8080");
//     screen_capture sc;

//     // server.recv_test_data();
//     // server.send_test_data();

//     while(true)
//     {
//         printf("Waiting for dimension\n");
//         std::pair<int,int> dimension = server.get_dimension();
        
//         int width = dimension.first;
//         int height = dimension.second;
//         printf("Received Dimension: (%d, %d) (w,h)\n", width, height);

//         struct screen_capture::screen screen = sc.get_fullscreen();

//         printf("Sending img\n");
//         server.send_img(screen.rgb_array, screen.width, screen.height, width, height);
//         printf("Done sending img\n");
//     }
// }