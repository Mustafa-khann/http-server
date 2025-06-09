#include "server.h"

int parse_http_request(const char *raw_request, http_request_t *request) {
    if (!raw_request || !request) {
        return -1;
    }

    /* Initialize request structure */
    memset(request, 0, sizeof(http_request_t));

    /* Create a copy of the request for parsing */
    char *request_copy = strdup(raw_request);
    if (!request_copy) {
        return -1;
    }

    /* Parse the request line (GET /path HTTP/1.1) */
    char *line = strtok(request_copy, "\r\n");
    if (!line) {
        free(request_copy);
        return -1;
    }

    /* Extract method, path, and version */
    char *method = strtok(line, " ");
    char *path = strtok(NULL, " ");
    char *version = strtok(NULL, " ");

    if (!method || !path || !version) {
        free(request_copy);
        return -1;
    }

    /* Copy parsed components */
    strncpy(request->method, method, sizeof(request->method) - 1);
    strncpy(request->path, path, sizeof(request->path) - 1);
    strncpy(request->version, version, sizeof(request->version) - 1);

    /* Handle root path */
    if (strcmp(request->path, "/") == 0) {
        strcpy(request->path, "/index.html");
    }

    /* Parse headers (basic implementation for Phase 1) */
    char *header_line;
    int header_pos = 0;
    while ((header_line = strtok(NULL, "\r\n")) != NULL && strlen(header_line) > 0) {
        /* Store headers for potential future use */
        int remaining_space = MAX_HEADERS_SIZE - header_pos - 1;
        if (remaining_space > 0) {
            strncat(request->headers + header_pos, header_line, remaining_space);
            header_pos += strlen(header_line);
            if (header_pos < MAX_HEADERS_SIZE - 2) {
                strcat(request->headers + header_pos, "\n");
                header_pos++;
            }
        }

        /* Check for Content-Length header */
        if (strncasecmp(header_line, "Content-Length:", 15) == 0) {
            request->content_length = atoi(header_line + 15);
        }
    }

    free(request_copy);
    return 0;
}

void build_http_response(http_response_t *response, char *output_buffer, size_t buffer_size) {
    if (!response || !output_buffer) {
        return;
    }

    /* Build status line */
    const char *status_text;
    switch (response->status_code) {
        case HTTP_OK:
            status_text = "OK";
            break;
        case HTTP_NOT_FOUND:
            status_text = "Not Found";
            break;
        case HTTP_INTERNAL_ERROR:
            status_text = "Internal Server Error";
            break;
        case HTTP_BAD_REQUEST:
            status_text = "Bad Request";
            break;
        default:
            status_text = "Unknown";
            break;
    }

    /* Format response */
    snprintf(output_buffer, buffer_size,
        "%s %d %s\r\n"
        "Server: %s\r\n"
        "%s"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        HTTP_VERSION,
        response->status_code,
        status_text,
        SERVER_NAME,
        response->content_type,
        response->body_length
    );

    /* Append body if it fits */
    size_t header_len = strlen(output_buffer);
    size_t remaining_space = buffer_size - header_len - 1;

    if (response->body && response->body_length > 0 && response->body_length <= remaining_space) {
        memcpy(output_buffer + header_len, response->body, response->body_length);
        output_buffer[header_len + response->body_length] = '\0';
    }
}

void create_error_response(int status_code, http_response_t *response) {
    if (!response) {
        return;
    }

    response->status_code = status_code;
    strcpy(response->content_type, CONTENT_TYPE_HTML);

    /* Try to serve custom error pages first */
    char error_file[MAX_PATH_LENGTH];
    snprintf(error_file, sizeof(error_file), "/%d.html", status_code);

    if (serve_static_file(error_file, response) == 0) {
        return; /* Custom error page served successfully */
    }

    /* Generate default error page */
    const char *error_title;
    const char *error_message;

    switch (status_code) {
        case HTTP_NOT_FOUND:
            error_title = "404 Not Found";
            error_message = "The requested resource was not found on this server.";
            break;
        case HTTP_INTERNAL_ERROR:
            error_title = "500 Internal Server Error";
            error_message = "The server encountered an internal error.";
            break;
        case HTTP_BAD_REQUEST:
            error_title = "400 Bad Request";
            error_message = "The server could not understand the request.";
            break;
        default:
            error_title = "Error";
            error_message = "An error occurred.";
            break;
    }

    /* Create HTML error page */
    char *html_template =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>%s</title>\n"
        "    <style>\n"
        "        body { font-family: Arial, sans-serif; margin: 40px; }\n"
        "        h1 { color: #333; }\n"
        "        p { color: #666; }\n"
        "        hr { border: 1px solid #ddd; }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>%s</h1>\n"
        "    <p>%s</p>\n"
        "    <hr>\n"
        "    <p><em>%s</em></p>\n"
        "</body>\n"
        "</html>\n";

    size_t html_size = strlen(html_template) + strlen(error_title) * 2 +
                       strlen(error_message) + strlen(SERVER_NAME) + 100;

    response->body = malloc(html_size);
    if (response->body) {
        snprintf(response->body, html_size, html_template,
                error_title, error_title, error_message, SERVER_NAME);
        response->body_length = strlen(response->body);
    } else {
        /* Fallback minimal response */
        response->body = strdup("Error");
        response->body_length = 5;
    }
}

void free_response(http_response_t *response) {
    if (response && response->body) {
        free(response->body);
        response->body = NULL;
        response->body_length = 0;
    }
}
