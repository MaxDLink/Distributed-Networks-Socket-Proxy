#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "proxy.h"

void handle_connections(int server_fd) {

    int client_fd, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    while (1) {
        // Accept incoming connections
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Connection accepted...\n");

        // Connect to target website
        int target_fd = connect_to_target();

        // Forward HTTP request from client to target
        forward_request(client_fd, target_fd);

        // Forward HTTP response from target to client
        forward_response(client_fd, target_fd);

        // Close sockets
        close(client_fd);
        close(target_fd);
    }
}

int connect_to_target() {

    struct sockaddr_in serv_addr;
    char *target_website = "www.bom.gov.au";
    int target_port = 80;
    int client_fd;

    // Create client socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("client socket failed");
        exit(EXIT_FAILURE);
    }

    // Resolve IP address of target website
    struct hostent *he;
    if ((he = gethostbyname(target_website)) == NULL) {
        perror("gethostbyname failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = *(in_addr_t*)he->h_addr;
    // Copy IP address to server address struct
    memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);

    // Set server port
    serv_addr.sin_port = htons(target_port);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to target website...\n");

    return client_fd;
}


void forward_request(int client_fd, int target_fd) {

    char buffer[1024] = {0};
    int valread;

    // Read HTTP request from client
    valread = read(client_fd, buffer, 1024);
    printf("--- HTTP request from client ---\n%s\n", buffer);

    // Forward HTTP request to target
    send(target_fd, buffer, strlen(buffer), 0);
    printf("--- HTTP request forwarded to target ---\n");

}

void forward_response(int client_fd, int target_fd) {

    char buffer[1024] = {0};
    int valread;

    // Read HTTP response from target
    valread = read(target_fd, buffer, 1024);
    printf("--- HTTP response from target ---\n%s\n", buffer);

    // Forward HTTP response to client
    send(client_fd, buffer, strlen(buffer), 0);
    printf("--- HTTP response forwarded to client ---\n");

}

