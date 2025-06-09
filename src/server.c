#include "server.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

// Global server instance for signal handling
server_t g_server = {0};

int main(int argc, char *argv[])
{
    int port = DEFAULT_PORT;

    // Parse command line arguments
    if(argc > 1){
        port = atoi(argv[1]);
        if(port <=0 || port >= 65535)
        {
            fprintf(stderr, "Invalid port number: %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    // Initialize logger
    init_logger();
    log_message(LOG_INFO, "Starting HTTP Server on Port %d", port);

    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Create and start server
    if(create_server(port) != 0)
    {
        log_message(LOG_ERROR, "Failed to create server");
        return EXIT_FAILURE;
    }

    printf(COLOR_GREEN "HTTP Server started on port %d" COLOR_RESET "\n", port);
    printf("Press Ctrl+C to stop the server\n");

    start_server(&g_server);

    return EXIT_SUCCESS;
}

int create_server(int port)
{
    int opt = 1;

    // Create socket
    g_server.server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(g_server.server_fd == -1)
    {
        perror("Socket creation failed");
        return -1;
    }

    // Set socket options
    if(setsockopt(g_server.server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
        close(g_server.server_fd);
        return -1;
    }

    // Configure server address
    g_server.address.sin_family = AF_INET;
    g_server.address.sin_addr.s_addr = INADDR_ANY;
    g_server.address.sin_port = htons(port);
    g_server.port = port;
    g_server.running = TRUE;

    // Bind socket
    if(bind(g_server.server_fd, (struct sockaddr*) &g_server.address, sizeof(g_server.address)) < 0)
        {
            perror("Bind failed");
            close(g_server.server_fd);
            return -1;
        }

    if (listen(g_server.server_fd, SOMAXCONN) < 0)
        {
            perror("Listen failed");
            return -1;
        }

    log_message(LOG_INFO, "Server socket created and listening on port %d", port);
    return 0;
}

void start_server(server_t *server)
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len;

    while(server->running)
    {
        // Accept incoming connection
        client_len = sizeof(client_addr);
        client_fd = accept(server->server_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd < 0)
            {
                perror("Accept failed");
                log_message(LOG_ERROR, "Failed to accept client connection");
                break;
            }

        // Log client connection
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        log_message(LOG_INFO, "New connection from %s:%d", client_ip, ntohs(client_addr.sin_port));

        // handle client request
        handle_client(client_fd);

        // close client socket
        close(client_fd);
    }
}

void handle_client(int client_fd)
{
    char buffer[BUFFER_SIZE] = {0};
    char response_buffer[BUFFER_SIZE * 2] = {0};
    http_request_t request = {0};
    http_response_t response = {0};

    // Read request from client
    ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if(bytes_read <= 0)
    {
        log_message(LOG_ERROR, "Failed to read request from client");
        return;
    }

    buffer[bytes_read] = '\0';

    // Parse HTTP request
    if(parse_http_request(buffer, &request) != 0)
    {
        log_message(LOG_ERROR, "Failed to parse HTTP request");
        create_error_response(HTTP_BAD_REQUEST, &response);
    }
    else
    {
        // Handle GET request
        if(strcmp(request.method, "GET") == 0)
        {
            if(serve_static_file(request.path, &response) != 0)
            {
                create_error_response(HTTP_NOT_FOUND, &response);
            }
        }
        else
        {
            create_error_response(HTTP_BAD_REQUEST, &response);
        }
    }

    // Build and send response
    build_http_response(&response, response_buffer, sizeof(response_buffer));
    send(client_fd, response_buffer, strlen(response_buffer), 0);

    // Log the request
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getpeername(client_fd, (struct sockaddr*)&client_addr, &addr_len);
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    log_request(request.method, request.path, response.status_code, client_ip);

    free_response(&response);
}

void cleanup_server(server_t *server)
{
    if(server->server_fd > 0)
    {
        close(server->server_fd);
        server->server_fd = 0;
    }

    server->running = FALSE;
    log_message(LOG_INFO, "Server Shutdown Complete!");
    close_logger();
}

void signal_handler(int sig)
{
    printf(COLOR_YELLOW "\nRecieved signal %d, shutting down server ...\n" COLOR_RESET, sig);
    g_server.running = FALSE;
    cleanup_server(&g_server);
    exit(EXIT_SUCCESS);
}
