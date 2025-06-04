#ifndef COMMON_H
#define COMMON_H

// Standard libraries includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

// Server configuration constants
#define DEFAULT_PORT 8080
#define MAX_CONNECTIONS 128
#define BUFFER_SIZE 8192
#define MAX_PATH_LENGTH 512
#define MAX_HEADERS_SIZE 4096
#define MAX_HEADER_LENGTH 256
#define PUBLIC_DIR "./public"
#define LOG_FILE "./logs/server.log"

// HTTP status codes
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500
#define HTTP_BAD_REQUEST 400

// HTTP response templates
#define HTTP_200_TEMPLATE "HTTP/1.1 200 OK\r\n"
#define HTTP_404_RESPONSE "HTTP/1.1 404 Not Found\r\n"
#define HTTP_500_RESPONSE "HTTP/1.1 500 Internal Server Error\r\n"
#define HTTP_400_RESPONSE "HTTP/1.1 400 Bad Request\r\n"

// Content type definition
#define CONTENT_TYPE_HTML "Content-Type: text/html; charset=UTF-8\r\n"
#define CONTENT_TYPE_TEXT "Content-Type: text/plain; charset=UTF-8\r\n"

// HTTP Protocol constants
#define HTTP_VERSION "HTTP/1.1"
#define SERVER_NAME "SimpleHTTP/1.0"

// Utility macros
#define LOG_INFO 0
#define LOG_ERROR 1
#define LOG_DEBUG 2

// Boolean definitions
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Color codes for terminal output
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

#endif
