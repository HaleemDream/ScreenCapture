#include "stdio.h"

#include "server.h"
#include "../screen/screen_capture.h"

server::server(const char* port) : port(port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE;    // listen on all network interfaces

    getaddrinfo(0, port, &hints, &bind_address);
    socket_client = socket(bind_address->ai_family,
                           bind_address->ai_socktype,
                           bind_address->ai_protocol);
    
    // invalid socket
    if(socket_client < 0)
    {
        printf("Error creating socket!\n");
        return;
    }
    
    if(bind(socket_client, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        printf("Bind failed!\n");
        return;
    }

    freeaddrinfo(bind_address);
}

server::~server()
{
    close_socket();
    delete bind_address;
}

// (width, height)
std::pair<int,int> server::get_dimension()
{
    socklen_t client_len = sizeof(client_address);
    int dimension[2];
    recvfrom(socket_client, dimension, sizeof(dimension), 0, (struct sockaddr*) &client_address, &client_len);

    int width = dimension[WIDTH_INDEX];
    int height = dimension[HEIGHT_INDEX];

    return std::make_pair(width, height);
}

void server::send_img(struct screen_capture::RGB* img, int screen_width, int screen_height,int canvas_width, int canvas_height)
{
    int rgb_count = 0;
    unsigned char payload[PAYLOAD_SIZE];

    socklen_t client_len = sizeof(client_address);

    int y_offset;
    int x_offset;

    for(int x = 0; x < canvas_width; x++)
    {
        y_offset = ((int) ((double) x / canvas_width * screen_width)) * screen_height;
        for(int y = 0; y < canvas_height; y++)
        {
            x_offset = (int) ((double) y / canvas_height * screen_height);
            screen_capture::RGB rgb = img[x_offset + y_offset];
                        
            payload[rgb_count * 3] = rgb.red;
            payload[rgb_count * 3 + 1] = rgb.green;
            payload[rgb_count * 3 + 2] = rgb.blue;

            if(++rgb_count == PAYLOAD_SIZE / 3)
            {
                sendto(socket_client, payload, sizeof(payload), 0, (struct sockaddr*) &client_address, client_len);
                rgb_count = 0;
            }    
        }
    }
}

void server::close_socket()
{
    close(socket_client);
}

void server::send_test_data()
{
    const char* payload = "hello";
    socklen_t client_len = sizeof(client_address);
    sendto(socket_client, payload, strlen(payload), 0, (struct sockaddr*) &client_address, client_len);
}

void server::recv_test_data()
{
    char data[1024];
    socklen_t client_len = sizeof(client_address);
    int bytes_recv = recvfrom(socket_client, data, 1024, 0, (struct sockaddr*) &client_address, &client_len);

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