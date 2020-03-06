#ifndef SERVER_H
#define SERVER_H

#include <utility>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "payload.h"
#include "../screen/screen_capture.h"

class server 
{
    public:
        server(const char* port);
        ~server();
        std::pair<int, int> get_dimension(); // (width, height)
        void send_img(struct screen_capture::RGB* img, int screen_width, int screen_height,int canvas_width, int canvas_height);
        void close_socket();

        void send_test_data();
        void recv_test_data();

    private:
        int socket_client;
        struct sockaddr_storage client_address;
        struct addrinfo *bind_address;
        const char* port;
};

#endif