#include "server.h"
#include "payload.h"

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
    close_socket();
}

server::~server()
{
    delete bind_address;
}

// (width, height)
std::pair<int,int> server::get_dimension()
{
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    int dimension[CLIENT_REQUEST_MESSAGE_SIZE];
    recvfrom(socket_client, dimension, sizeof(dimension), 0, (struct sockaddr*) &client_address, &client_len);

    int width = dimension[WIDTH_INDEX];
    int height = dimension[HEIGHT_INDEX];

    return std::make_pair(width, height);
}

void server::close_socket()
{
    close(socket_client);
}

// for testing
// int main(int argc, char** argv)
// {
//     server server("8080");
// }