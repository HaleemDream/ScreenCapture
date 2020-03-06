#ifndef CLIENT_H
#define CLIENT_H

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

#define MIN_PACKET_THRESHOLD 0.90
#define MIN_IMG_PACKET_LOSS(PACKET_COUNT) (PACKET_COUNT / (width * height * 3) > MIN_PACKET_THRESHOLD)

class client
{
    public:
        client(const char* port, const char* host);
        ~client();
        void set_dimension(int width, int height);
        std::pair<int,int> get_dimension();
        void send_dimension();
        unsigned char* get_img();
        void close_socket();
        void recv_test_data();
        void send_test_data();

    private:
        int socket_server;
        struct addrinfo* peer_address;
        const char* host;
        const char* port;

        int width;
        int height;
        unsigned char* img;
};

#endif