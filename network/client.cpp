#include "client.h"

client::client(const char* host, const char* port)
: host(host), port(port)
{
    printf("configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if(getaddrinfo(host, port, &hints, &peer_address))
    {
        printf("Err: failed to configure remote address\n");
        return;
    }

    printf("Creating socket...\n");
    int socket_peer;
    socket_peer = socket(peer_address->ai_family,
                         peer_address->ai_socktype,
                         peer_address->ai_protocol);

    if(socket_peer < 0)
    {
        printf("Failed to create socket\n");
        return;
    }

    const char* message = "Yo meng";
    printf("Sending message: %s\n", message);
    int bytes_sent = sendto(socket_peer,
                            message,
                            strlen(message),
                            0,
                            peer_address->ai_addr,
                            peer_address->ai_addrlen);

    printf("Send bytes...\n");

    close(socket_peer);
}

client::~client()
{

}

int main(int argc, char** argv)
{
    client client("localhost", "8080");
}
