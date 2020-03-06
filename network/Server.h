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

#include "Payload.h"
#include "../screen/ScreenCapture.h"

class Server 
{
    public:
        Server(const char* port);
        ~Server();
        std::pair<int, int> getDimension(); // (width, height)
        void sendImg(struct ScreenCapture::RGB* img, int screenWidth, int screenHeight,int canvasWidth, int canvasHeight);
        void closeSocket();

        void sendTestData();
        void recvTestData();

    private:
        int socketClient;
        struct sockaddr_storage clientAddress;
        struct addrinfo *bindAddress;
        const char* port;
};

#endif