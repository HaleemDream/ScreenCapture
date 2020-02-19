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

class client
{
    public:
        client(const char* host, const char* port);
        ~client();
    private:
        struct addrinfo* peer_address;
        const char* host;
        const char* port;
};

#endif