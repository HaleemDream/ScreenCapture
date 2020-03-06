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

#include "Payload.h"

#define MIN_PACKET_THRESHOLD 0.90
#define MIN_IMG_PACKET_LOSS(PACKET_COUNT) (PACKET_COUNT / (width * height * 3) > MIN_PACKET_THRESHOLD)

class Client
{
    public:
        Client(const char* port, const char* host);
        ~Client();
        void setDimension(int width, int height);
        std::pair<int,int> getDimension();
        void sendDimension();
        unsigned char* getImg();
        void closeSocket();
        void recvTestData();
        void sendTestData();

    private:
        int socketServer;
        struct addrinfo* peerAddress;
        const char* host;
        const char* port;

        int width;
        int height;
        unsigned char* img;
};

#endif