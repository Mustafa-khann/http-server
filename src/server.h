#ifndef SERVER_H
#define SERVER_H

#include "../include/common.h"

// Server Structure
typedef struct {
    int server_fd;
    int port;
    struct sockaddr_in address;
    int running;
} server_t;

// HTTP request structure
typedef struct {
    char method[16];
    char path[MAX_PATH_LENGTH];
    char version[16];
    char headers[MAX_HEADERS_SIZE];
    char body[BUFFER_SIZE];
    int content_length;
} http_request_t;

// HTTP response structure
typedef struct {
    int status_code;
    char content_type[128];
    char *body;
    size_t body_length;
} http_response_t;

// Function prototypes - server.c
int create_server(int port);
void start_server(server_t *server);
void handle_client(int client_fd);
void cleanup_server(server_t *server);
void signal_handler(int sig);

/* Function prototypes - http_parser.c */
int parse_http_request(const char *raw_request, http_request_t *request);
void build_http_response(http_response_t *response, char *output_buffer, size_t buffer_size);
void create_error_response(int status_code, http_response_t *response);
void free_response(http_response_t *response);

// Function prototypes - file_handler.c
int serve_static_file(const char *path, http_response_t *response);
int file_exists(const char *path);
const char* get_content_type(const char *path);
char* read_file(const char *path ,size_t *file_size);

// Function prototypes logger.c
void init_logger(void);
void log_message(int level, const char *format, ...);
void log_request(const char *method, const char *path, int status_code, const char *client_ip);
void close_logger(void);

// Global server_t g_server;

#endif
