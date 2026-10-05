*This project has been created as part of the 42 curriculum by dzapata, gdero and thomvan*

# webserv

## Description

A custom HTTP server built from scratch in C++ as part of the 42 curriculum (3-person team).
The project recreates core HTTP server functionality without relying on any existing web server
libraries or frameworks, and is tested for compatibility with real web browsers.

## Features

- Handles multiple simultaneous client connections using non-blocking I/O.
- Supports GET, POST, and DELETE HTTP methods.
- Configurable via a configuration file (server blocks, routes, error pages), inspired by NGINX.
- Serves static files and supports CGI execution.

## Usage

```bash
make
./webserv [configuration_file]
```
