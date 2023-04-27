#ifndef PROXY_H
#define PROXY_H

void handle_connections(int server_fd);
int connect_to_target();
void forward_request(int client_fd, int target_fd);
void forward_response(int client_fd, int target_fd);

#endif
