#include <iostream>
#include <thread>
#include <chrono>

#include "sys/time.h"

#include "Client.h"

Client::Client(const char* port, const char* host)
: host(host), port(port)
{
    width = 800;
    height = 600;

    img = new unsigned char[width * height * 3];

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE;    // listen on all network interfaces

    if(getaddrinfo(host, port, &hints, &peerAddress))
    {
        printf("Err: failed to configure remote address\n");
        return;
    }

    socketServer = socket(peerAddress->ai_family,
                         peerAddress->ai_socktype,
                         peerAddress->ai_protocol);

    if(socketServer < 0)
    {
        printf("Failed to create socket\n");
        return;
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 50000;

    if(setsockopt(socketServer, SOL_SOCKET, SO_RCVTIMEO,&tv, sizeof(tv)) < 0)
    {
        printf("Failed to set sock option\n");
    }
}

Client::~Client()
{
    delete[] img;
    closeSocket();
}

void Client::setDimension(int width, int height)
{
    this->width = width;
    this->height = height;
}

std::pair<int, int> Client::getDimension()
{
    return std::make_pair(width, height);
}

void Client::sendDimension()
{
    std::cout << "Sending dimensions" << std::endl;
    int dimension[2] = {width, height};
    sendto(socketServer, dimension, sizeof(dimension), 0, peerAddress->ai_addr, peerAddress->ai_addrlen);
}

unsigned char* Client::getImg()
{
    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);
    unsigned char payload[PAYLOAD_SIZE];
    printf("Recv img\n");
    int bytes_recv;
    int packet_count = 0;
    while((bytes_recv = recvfrom(socketServer, payload, PAYLOAD_SIZE, 0, (struct sockaddr*) &addr, &addr_len)) != -1)
    {
        for(int i = 0; i < PAYLOAD_SIZE; i++)
        {
            img[packet_count * PAYLOAD_SIZE + i] = payload[i];
        }
        packet_count++;
    }
    printf("img end\n");

    // min packet loss threshold
    return MIN_IMG_PACKET_LOSS(packet_count * PAYLOAD_SIZE) ? img : 0;
}

void Client::closeSocket()
{
    close(socketServer);
}

void Client::recvTestData()
{
    printf("Receiving data...\n");
    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);
    char data[1024];
    int bytes_recv = recvfrom(socketServer, data, 1024, 0, (struct sockaddr*) &addr, &addr_len);
    printf("bytes recv = %d\n", bytes_recv);
    printf("data recv - %.*s\n", bytes_recv, data);
}

void Client::sendTestData()
{
    printf("Sending msg, 'hello'\n");
    const char* msg = "hello";
    sendto(socketServer, msg, strlen(msg), 0, peerAddress->ai_addr, peerAddress->ai_addrlen);
    printf("message sent\n");
}