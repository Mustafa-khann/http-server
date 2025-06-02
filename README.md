# HTTP Server in C

A simple HTTP server implementation in C, built from scratch to understand the fundamentals of web servers and the HTTP protocol. This project is divided into three phases, each building upon the previous one with increasing complexity.

## Project Phases

### Phase 1: Basic HTTP Server
- Single-threaded server implementation
- Support for basic HTTP/1.1 GET requests
- Simple routing system
- Static file serving for HTML files
- Basic error handling (404, 500)
- Command-line configuration for port number
- Simple logging of requests

### Phase 2: Enhanced Server Features
- Multi-threaded request handling
- Support for POST requests
- Basic request parsing and validation
- Support for common HTTP headers
- Improved error handling and status codes
- Basic security features (request size limits)
- Support for serving different file types (HTML, CSS, images)
- Configuration file support
- Request logging with timestamps

### Phase 3: Advanced Features
- Connection pooling
- Keep-alive connections
- HTTP/1.1 pipelining support
- Basic caching mechanism
- Compression support (gzip)
- Basic authentication
- URL parameter parsing
- Custom error pages
- Performance monitoring
- Support for HTTP methods (PUT, DELETE, HEAD)
- Basic security features (request validation, basic DoS protection)

## Building the Project
```bash
make
```

## Running the Server
```bash
./server [port]
```
If no port is specified, the server will default to port 8080.

## Project Structure
- `server.c` - Main server implementation
- `server.h` - Header file containing function declarations and constants
- `Makefile` - Build configuration

## Dependencies
- C compiler (GCC recommended)
- Make
- POSIX-compliant operating system

## Learning Objectives
- Understanding of socket programming
- HTTP protocol implementation
- Multi-threading and concurrency
- Network programming best practices
- Error handling and debugging
- Performance optimization
- Security considerations in web servers
