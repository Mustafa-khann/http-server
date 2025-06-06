# HTTP Server in C - Phase 1

A simple HTTP server implementation in C, built from scratch to understand the fundamentals of web servers and the HTTP protocol.

## 🚀 Quick Start

### Prerequisites
- GCC compiler
- Make utility
- POSIX-compliant operating system (Linux, macOS, WSL)

### Building the Server
```bash
# Clone and navigate to the project directory
cd http-server

# Build the server
make

# Or build with debug symbols
make debug
```

### Running the Server
```bash
# Run on default port (8080)
./server

# Run on custom port
./server 3000

# Or use make targets
make run              # Runs on port 8080
make run-port         # Prompts for port number
```

### Testing the Server
1. Open your browser and navigate to `http://localhost:8080`
2. Try different URLs:
   - `http://localhost:8080/` - Homepage
   - `http://localhost:8080/nonexistent` - 404 error page
3. Check the console and `logs/server.log` for request logs

## 📁 Project Structure

```
http-server/
├── src/                    # Source code
│   ├── server.c           # Main server implementation
│   ├── server.h           # Header declarations
│   ├── http_parser.c      # HTTP request/response handling
│   ├── file_handler.c     # Static file serving
│   └── logger.c           # Logging functionality
├── include/
│   └── common.h           # Common definitions and constants
├── public/                # Static files directory
│   ├── index.html         # Default homepage
│   ├── 404.html          # 404 error page
│   └── 500.html          # 500 error page
├── logs/                  # Server logs (created automatically)
├── build/                 # Build artifacts (created automatically)
├── Makefile              # Build configuration
└── README.md             # This file
```

## ✨ Phase 1 Features

- **Single-threaded HTTP/1.1 server** - Handles one request at a time
- **GET request support** - Serves static files and handles routing
- **Static file serving** - Serves HTML files from the `public/` directory
- **Basic routing** - Routes `/` to `index.html` automatically
- **Error handling** - Custom 404 and 500 error pages
- **Request logging** - Console output with colors and file logging
- **Security features** - Directory traversal protection
- **Configurable port** - Command-line port specification
- **Graceful shutdown** - Handles SIGINT and SIGTERM signals

## 🛠️ Make Targets

| Target | Description |
|--------|-------------|
| `make` or `make all` | Build the server executable |
| `make debug` | Build with debug symbols and debugging enabled |
| `make release` | Build optimized release version |
| `make clean` | Remove build artifacts |
| `make run` | Run server on port 8080 |
| `make run-port` | Run server on custom port (prompts user) |
| `make install` | Install to `/usr/local/bin` (requires sudo) |
| `make uninstall` | Remove from `/usr/local/bin` (requires sudo) |
| `make valgrind` | Run with memory leak detection (requires valgrind) |
| `make check` | Run static analysis (requires cppcheck) |
| `make format` | Format source code (requires clang-format) |
| `make help` | Show all available targets |

## 🔧 Configuration

### Server Configuration
- **Default Port**: 8080
- **Max Connections**: 128 (listening queue)
- **Buffer Size**: 8KB for requests/responses
- **File Size Limit**: 10MB per file
- **Public Directory**: `./public/`
- **Log File**: `./logs/server.log`

### Supported HTTP Features
- **Methods**: GET only (Phase 1)
- **Protocol**: HTTP/1.1
- **Connection**: Close (no keep-alive in Phase 1)
- **Content Types**: HTML, text, CSS, JS, JSON, images

## 📝 Usage Examples

### Basic Usage
```bash
# Start server on default port
./server

# Start server on port 3000
./server 3000
```

### Adding Static Content
```bash
# Add HTML files to public directory
echo "<h1>Hello World</h1>" > public/hello.html

# Access via browser
# http://localhost:8080/hello.html
```

### Monitoring Logs
```bash
# Watch live logs
tail -f logs/server.log

# View colored console output
./server 8080
```

## 🐛 Debugging

### Memory Leaks
```bash
# Build debug version and run with valgrind
make debug
make valgrind
```

### Static Analysis
```bash
# Run cppcheck for code analysis
make check
```

### Verbose Logging
All requests are logged with:
- Timestamp
- HTTP method and path
- Response status code
- Client IP address
- Color-coded console output

## 🔒 Security Features

- **Directory Traversal Protection** - Prevents `../` attacks
- **File Size Limits** - Prevents memory exhaustion
- **Request Size Limits** - Buffer overflow protection
- **Input Validation** - Basic HTTP request validation

## ⚡ Performance Notes

Phase 1 is single-threaded and handles one request at a time. This is intentional for learning purposes. Performance improvements will be added in Phase 2 with multi-threading.

Current limitations:
- No concurrent request handling
- No connection keep-alive
- Basic file caching (OS-level only)

## 🚧 Coming in Phase 2

- Multi-threaded request handling
- POST request support
- Enhanced HTTP header parsing
- Configuration file support
- Multiple content types
- Improved error handling

## 🤝 Contributing

This is a learning project, but suggestions and improvements are welcome!

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is for educational purposes. Feel free to use and modify as needed.

## 🎯 Learning Objectives Achieved

- ✅ Socket programming in C
- ✅ HTTP protocol implementation
- ✅ Request parsing and response generation
- ✅ File I/O operations
- ✅ Error handling and logging
- ✅ Signal handling
- ✅ Memory management
- ✅ Build system (Makefile)

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


**Happy Coding!** 🚀
