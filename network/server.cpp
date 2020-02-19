#include "server.h"

server::server(const char* port) : port(port)
{
    printf("Configuring Server...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE; // listen on all network interfaces

    getaddrinfo(0, port, &hints, &bind_address);

      printf("Creating socket...\n");
    int socket_listen;
    socket_listen = socket(bind_address->ai_family,
                           bind_address->ai_socktype,
                           bind_address->ai_protocol);
    
    // invalid socket
    if(socket_listen < 0)
    {
        printf("Error creating socket!\n");
        return;
    }
    
    printf("Binding server...\n");
    if(bind(socket_listen, 
            bind_address->ai_addr, 
            bind_address->ai_addrlen))
    {
        printf("Bind failed!\n");
        return;
    }

    freeaddrinfo(bind_address);

    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char read[1024];
    int bytes_received = recvfrom(socket_listen,
                                  read,
                                  1024,
                                  0,
                                  (struct sockaddr*) &client_address,
                                   &client_len);

    printf("Received bytes...\n");
}

server::~server()
{
    delete bind_address;
}

// for testing
int main(int argc, char** argv)
{
    server server("8080");
}