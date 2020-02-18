#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

class server 
{
    public:
        server(const char* port);
        ~server();

    private:
        struct addrinfo *bind_address;
        const char* port;
};

#endif