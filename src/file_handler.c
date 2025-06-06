#include "server.h"
#include "ctype.h"

int serve_static_file(const char *path, http_response_t *response) {
    if (!path || !response) {
        return -1;
    }

    /* Build full file path */
    char full_path[MAX_PATH_LENGTH];
    snprintf(full_path, sizeof(full_path), "%s%s", PUBLIC_DIR, path);

    /* Security check: prevent directory traversal */
    if (strstr(path, "..") != NULL) {
        return -1;
    }

    /* Check if file exists and is readable */
    if (!file_exists(full_path)) {
        return -1;
    }

    /* Read file content */
    size_t file_size;
    char *file_content = read_file(full_path, &file_size);
    if (!file_content) {
        return -1;
    }

    /* Set response properties */
    response->status_code = HTTP_OK;
    response->body = file_content;
    response->body_length = file_size;

    /* Set content type based on file extension */
    const char *content_type = get_content_type(path);
    strncpy(response->content_type, content_type, sizeof(response->content_type) - 1);

    return 0;
}

int file_exists(const char *path) {
    if (!path) {
        return FALSE;
    }

    struct stat st;
    if (stat(path, &st) == 0) {
        /* Check if it's a regular file */
        return S_ISREG(st.st_mode) ? TRUE : FALSE;
    }

    return FALSE;
}

const char* get_content_type(const char *path) {
    if (!path) {
        return CONTENT_TYPE_TEXT;
    }

    /* Find the file extension */
    const char *extension = strrchr(path, '.');
    if (!extension) {
        return CONTENT_TYPE_TEXT;
    }

    /* Convert to lowercase for comparison */
    char ext_lower[16] = {0};
    strncpy(ext_lower, extension, sizeof(ext_lower) - 1);
    for (int i = 0; ext_lower[i]; i++) {
        ext_lower[i] = tolower(ext_lower[i]);
    }

    /* Determine content type */
    if (strcmp(ext_lower, ".html") == 0 || strcmp(ext_lower, ".htm") == 0) {
        return CONTENT_TYPE_HTML;
    } else if (strcmp(ext_lower, ".txt") == 0) {
        return CONTENT_TYPE_TEXT;
    } else if (strcmp(ext_lower, ".css") == 0) {
        return "Content-Type: text/css\r\n";
    } else if (strcmp(ext_lower, ".js") == 0) {
        return "Content-Type: application/javascript\r\n";
    } else if (strcmp(ext_lower, ".json") == 0) {
        return "Content-Type: application/json\r\n";
    } else if (strcmp(ext_lower, ".png") == 0) {
        return "Content-Type: image/png\r\n";
    } else if (strcmp(ext_lower, ".jpg") == 0 || strcmp(ext_lower, ".jpeg") == 0) {
        return "Content-Type: image/jpeg\r\n";
    } else if (strcmp(ext_lower, ".gif") == 0) {
        return "Content-Type: image/gif\r\n";
    } else if (strcmp(ext_lower, ".ico") == 0) {
        return "Content-Type: image/x-icon\r\n";
    }

    /* Default to plain text */
    return CONTENT_TYPE_TEXT;
}

char* read_file(const char *path, size_t *file_size) {
    if (!path || !file_size) {
        return NULL;
    }

    FILE *file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }

    /* Get file size */
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* Check for reasonable file size (prevent memory exhaustion) */
    if (*file_size > 10 * 1024 * 1024) { /* 10MB limit */
        fclose(file);
        return NULL;
    }

    /* Allocate buffer */
    char *buffer = malloc(*file_size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    /* Read file content */
    size_t bytes_read = fread(buffer, 1, *file_size, file);
    fclose(file);

    if (bytes_read != *file_size) {
        free(buffer);
        return NULL;
    }

    buffer[*file_size] = '\0'; /* Null terminate for text files */
    return buffer;
}
