#include "server.h"
#include <stdarg.h>

static FILE *log_file = NULL;
static int log_initialized = FALSE;

void init_logger(void) {
    if (log_initialized) {
        return;
    }

    /* Create logs directory if it doesn't exist */
    mkdir("./logs", 0755);

    /* Open log file */
    log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        fprintf(stderr, "Warning: Could not open log file %s\n", LOG_FILE);
        return;
    }

    log_initialized = TRUE;

    /* Log startup message */
    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; /* Remove newline */

    fprintf(log_file, "\n=== Server Started at %s ===\n", timestamp);
    fflush(log_file);
}

void log_message(int level, const char *format, ...) {
    if (!format) {
        return;
    }

    /* Get current time */
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    /* Determine log level string and color */
    const char *level_str;
    const char *color;

    switch (level) {
        case LOG_INFO:
            level_str = "INFO";
            color = COLOR_GREEN;
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            color = COLOR_RED;
            break;
        case LOG_DEBUG:
            level_str = "DEBUG";
            color = COLOR_BLUE;
            break;
        default:
            level_str = "UNKNOWN";
            color = COLOR_RESET;
            break;
    }

    /* Format the message */
    va_list args;
    va_start(args, format);

    char message[1024];
    vsnprintf(message, sizeof(message), format, args);

    va_end(args);

    /* Print to console with color */
    printf("%s[%s] %s - %s%s\n", color, timestamp, level_str, message, COLOR_RESET);

    /* Write to log file if available */
    if (log_file) {
        fprintf(log_file, "[%s] %s - %s\n", timestamp, level_str, message);
        fflush(log_file);
    }
}

void log_request(const char *method, const char *path, int status_code, const char *client_ip) {
    if (!method || !path || !client_ip) {
        return;
    }

    /* Get current time */
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    /* Determine status color for console output */
    const char *status_color;
    if (status_code >= 200 && status_code < 300) {
        status_color = COLOR_GREEN;
    } else if (status_code >= 400 && status_code < 500) {
        status_color = COLOR_YELLOW;
    } else if (status_code >= 500) {
        status_color = COLOR_RED;
    } else {
        status_color = COLOR_RESET;
    }

    /* Format request log entry */
    printf("%s[%s] REQUEST - %s %s %s%d%s from %s\n",
           COLOR_BLUE, timestamp, method, path,
           status_color, status_code, COLOR_RESET, client_ip);

    /* Write to log file if available */
    if (log_file) {
        fprintf(log_file, "[%s] REQUEST - %s %s %d from %s\n",
                timestamp, method, path, status_code, client_ip);
        fflush(log_file);
    }
}

void close_logger(void) {
    if (log_file) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        timestamp[strlen(timestamp) - 1] = '\0'; /* Remove newline */

        fprintf(log_file, "=== Server Stopped at %s ===\n\n", timestamp);
        fclose(log_file);
        log_file = NULL;
    }
    log_initialized = FALSE;
}
