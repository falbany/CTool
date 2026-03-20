# `cb_net` - C Networking Utilities

The `cb_net` module provides a simplified, cross-platform interface for basic TCP and UDP networking operations. It abstracts away the platform-specific details of socket programming on Windows (Winsock) and POSIX-compliant systems (like Linux or macOS).

## `cb_socket_t`

A cross-platform socket descriptor. This type definition abstracts the underlying socket handle/file descriptor across different operating systems.

```c
typedef int cb_socket_t;
```

## `cb_net_proto_t`

An enumeration of supported network protocols.

```c
typedef enum {
    CB_NET_TCP, // Transmission Control Protocol (TCP).
    CB_NET_UDP  // User Datagram Protocol (UDP).
} cb_net_proto_t;
```

## API

The API is exposed through a single global namespace struct `cb_net`.

### `cb_net.init`

Initializes the networking subsystem.

-   **On Windows:** This function calls `WSAStartup` to initialize the Winsock library.
-   **On other platforms:** This function does nothing.

It is safe to call this function multiple times.

**Returns:**

-   `true` if initialization was successful or not needed.
-   `false` if an error occurred (Windows only).

### `cb_net.cleanup`

Cleans up the networking subsystem.

-   **On Windows:** This function calls `WSACleanup`.
-   **On other platforms:** This function does nothing.

This should be called when networking is no longer needed to free up resources.

### `cb_net.connect`

Creates a client socket and connects to a host.

**Parameters:**

-   `host`: The hostname or IP address of the server to connect to.
-   `port`: The port number on the server.
-   `proto`: The protocol to use (`CB_NET_TCP` or `CB_NET_UDP`).

**Returns:**

-   A valid `cb_socket_t` on success.
-   -1 on failure.

### `cb_net.send`

Sends data over a socket.

**Parameters:**

-   `sock`: The socket to send data through.
-   `data`: A pointer to the data to be sent.
-   `len`: The length of the data in bytes.

**Returns:**

-   The number of bytes sent on success.
-   -1 on error.

### `cb_net.receive`

Receives data from a socket.

**Parameters:**

-   `sock`: The socket to receive data from.
-   `buffer`: A pointer to the buffer where received data will be stored.
-   `max_len`: The maximum number of bytes to receive.

**Returns:**

-   The number of bytes received on success.
-   -1 on error.
-   0 if the peer has closed the connection.

### `cb_net.close`

Closes a socket.

**Parameters:**

-   `sock`: The socket to close.

## Example

```c
#include "cb_net.h"
#include <stdio.h>
#include <string.h>

int main() {
    if (!cb_net.init()) {
        fprintf(stderr, "Failed to initialize networking\n");
        return 1;
    }

    cb_socket_t sock = cb_net.connect("example.com", 80, CB_NET_TCP);
    if (sock == -1) {
        fprintf(stderr, "Failed to connect\n");
        cb_net.cleanup();
        return 1;
    }

    const char* request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
    cb_net.send(sock, request, strlen(request));

    char buffer[1024];
    int received = cb_net.receive(sock, buffer, sizeof(buffer) - 1);
    if (received > 0) {
        buffer[received] = '\0';
        printf("Received:\n%s\n", buffer);
    }

    cb_net.close(sock);
    cb_net.cleanup();
    return 0;
}
```
