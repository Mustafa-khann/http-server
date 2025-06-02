#include "server.h"

int init_server(server_config_t *config) {
    // Create socket
    if ((config->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(config->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        return -1;
    }

    // Configure address structure
    config->address.sin_family = AF_INET;
    config->address.sin_addr.s_addr = INADDR_ANY;
    config->address.sin_port = htons(config->port);

    // Bind socket to address
    if (bind(config->server_fd, (struct sockaddr *)&config->address, sizeof(config->address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listen for connections
    if (listen(config->server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        return -1;
    }

    printf("Server listening on port %d\n", config->port);
    return 0;
}

void parse_http_request(char *buffer, char *method, char *path) {
    sscanf(buffer, "%s %s", method, path);
}

void send_http_response(int client_socket, int status_code, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    char status_text[50];

    // Set status text based on status code
    switch (status_code) {
        case HTTP_OK:
            strcpy(status_text, "200 OK");
            break;
        case HTTP_NOT_FOUND:
            strcpy(status_text, "404 Not Found");
            break;
        case HTTP_INTERNAL_ERROR:
            strcpy(status_text, "500 Internal Server Error");
            break;
        default:
            strcpy(status_text, "500 Internal Server Error");
    }

    // Create HTTP response
    snprintf(response, BUFFER_SIZE,
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        status_text,
        content_type,
        strlen(body),
        body
    );

    send(client_socket, response, strlen(response), 0);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char method[10] = {0};
    char path[256] = {0};

    // Read client request
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Received request: %s", buffer);

    // Parse HTTP request
    parse_http_request(buffer, method, path);

    // Simple routing
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            send_http_response(client_socket, HTTP_OK, "text/html",
                "<html><body><h1>Welcome to the HTTP Server!</h1></body></html>");
        } else {
            send_http_response(client_socket, HTTP_NOT_FOUND, "text/html",
                "<html><body><h1>404 Not Found</h1></body></html>");
        }
    } else {
        send_http_response(client_socket, HTTP_INTERNAL_ERROR, "text/html",
            "<html><body><h1>501 Not Implemented</h1></body></html>");
    }

    close(client_socket);
}

void cleanup_server(server_config_t *config) {
    close(config->server_fd);
}

int main(int argc, char *argv[]) {
    server_config_t config;
    config.port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;

    if (init_server(&config) < 0) {
        return 1;
    }

    while (1) {
        int client_socket;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);

        // Accept incoming connection
        if ((client_socket = accept(config.server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle client request
        handle_client(client_socket);
    }

    cleanup_server(&config);
    return 0;
} 