#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Server configuration structure
typedef struct {
    int port;
    int server_fd;
    struct sockaddr_in address;
} server_config_t;

// Function declarations
int init_server(server_config_t *config);
void handle_client(int client_socket);
void cleanup_server(server_config_t *config);
void parse_http_request(char *buffer, char *method, char *path);
void send_http_response(int client_socket, int status_code, const char *content_type, const char *body);

// HTTP status codes
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500

#endif // SERVER_H 