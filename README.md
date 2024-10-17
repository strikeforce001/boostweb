# Multithreaded HTTP Server in C++ (Boost.Asio)

## Overview

This project demonstrates a simple multithreaded HTTP server built using C++ and the Boost.Asio library. The server supports basic HTTP GET and POST requests, responds with simple HTML content, and can handle multiple clients concurrently using threads.

## Features

- **GET Request Handling**: Responds with an HTML message when a GET request is received.
- **POST Request Handling**: Responds with an HTML message when a POST request is received.
- **404 Not Found**: Returns a "404 Not Found" error for unsupported HTTP methods.
- **Multithreading**: Each client request is handled in a separate thread.

## Requirements

- C++11 or higher
- Boost library (`boost::asio`)

### Installing Boost

If you don't have Boost installed, you can install it using the following commands:

For Ubuntu/Debian:
```bash
sudo apt-get install libboost-all-dev
