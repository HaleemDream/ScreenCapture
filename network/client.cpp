#include "client.h"

client::client(const char* host, const char* port)
: host(host), port(port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if(getaddrinfo(host, port, &hints, &peer_address))
    {
        printf("Err: failed to configure remote address\n");
        return;
    }

    socket_server = socket(peer_address->ai_family,
                         peer_address->ai_socktype,
                         peer_address->ai_protocol);

    if(socket_server < 0)
    {
        printf("Failed to create socket\n");
        return;
    }
}

client::~client()
{

}

void client::send_dimension(int width, int height)
{
    int dimension[2] = {width, height};
    sendto(socket_server, dimension, sizeof(dimension), 0, peer_address->ai_addr, peer_address->ai_addrlen);
}

void client::close_socket()
{
    close(socket_server);
}

// int main(int argc, char** argv)
// {
//     client client("localhost", "8080");
// }
