#ifndef PROXY_H
#define PROXY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int connect_to_target(FILE *output_file);
void forward_request(int client_fd, int target_fd, FILE *output_file);
void forward_response(int client_fd, int target_fd, FILE *output_file);
void handle_connections(int server_fd, FILE *output_file);

#endif /* PROXY_H */

