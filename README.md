# HTTP Server in C

A simple HTTP server implementation in C, built from scratch to understand the fundamentals of web servers and the HTTP protocol.

## Features (Planned)
- Basic HTTP/1.1 server implementation
- Support for GET requests
- Static file serving
- Basic error handling
- Configurable port number

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
