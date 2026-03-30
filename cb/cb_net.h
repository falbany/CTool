/**
 * @file cb_net.h
 * @author Florent ALBANY (FAL)
 * @brief Cross-platform C networking utilities (TCP/UDP).
 * 
 * This module provides a simplified, cross-platform interface for basic TCP and
 * UDP networking operations. It abstracts away the platform-specific details
 * of socket programming on Windows (Winsock) and POSIX-compliant systems
 * (like Linux or macOS).
 *
 * @version 1.0
 * @date 2026-03-20
 */

#ifndef CB_NET_H
#define CB_NET_H

#include <stdbool.h>
#include <stddef.h>
#include "../libctool.h"

/**
 * @brief Cross-platform socket descriptor.
 * 
 * This type definition abstracts the underlying socket handle/file descriptor
 * across different operating systems.
 */
typedef int cb_socket_t;

/**
 * @brief Enumeration of supported network protocols.
 */
typedef enum {
    CB_NET_TCP, /**< Transmission Control Protocol (TCP). */
    CB_NET_UDP  /**< User Datagram Protocol (UDP). */
} cb_net_proto_t;

/**
 * @brief A namespace for networking functions.
 * 
 * This struct groups networking functions to provide a clean and organized API.
 * Call these functions through the global `cb_net` instance.
 * @see cb_net
 */
struct cb_net_namespace {
    /**
     * @brief Initializes the networking subsystem.
     * 
     * On Windows, this function calls `WSAStartup` to initialize the Winsock
     * library. On other platforms, this function does nothing. It is safe to
     * call this function multiple times.
     * 
     * @return true if initialization was successful or not needed.
     * @return false if an error occurred (Windows only).
     */
    bool (*init)(void);

    /**
     * @brief Cleans up the networking subsystem.
     * 
     * On Windows, this function calls `WSACleanup`. On other platforms, it
     * does nothing. This should be called when networking is no longer needed
     * to free up resources.
     */
    void (*cleanup)(void);

    /**
     * @brief Creates a client socket and connects to a host.
     * 
     * @param[in] host The hostname or IP address of the server to connect to.
     * @param[in] port The port number on the server.
     * @param[in] proto The protocol to use (TCP or UDP).
     * @return A valid `cb_socket_t` on success.
     * @return -1 on failure.
     */
    cb_socket_t (*connect)(const char* host, int port, cb_net_proto_t proto);

    /**
     * @brief Sends data over a socket.
     * 
     * @param[in] sock The socket to send data through.
     * @param[in] data A pointer to the data to be sent.
     * @param[in] len The length of the data in bytes.
     * @return The number of bytes sent on success.
     * @return -1 on error.
     */
    int (*send)(cb_socket_t sock, const char* data, size_t len);

    /**
     * @brief Receives data from a socket.
     * 
     * @param[in] sock The socket to receive data from.
     * @param[out] buffer A pointer to the buffer where received data will be stored.
     * @param[in] max_len The maximum number of bytes to receive.
     * @return The number of bytes received on success.
     * @return -1 on error. Returns 0 if the peer has closed the connection.
     */
    int (*receive)(cb_socket_t sock, char* buffer, size_t max_len);

    /**
     * @brief Closes a socket.
     * 
     * @param[in] sock The socket to close.
     */
    void (*close)(cb_socket_t sock);
};

/**
 * @brief Global instance of the networking function namespace.
 * 
 * Use this instance to call the networking functions.
 * @example
 * @code
 * #include "cb_net.h"
 * #include <stdio.h>
 * #include <string.h>
 * 
 * int main() {
 *     if (!cb_net.init()) {
 *         fprintf(stderr, "Failed to initialize networking\n");
 *         return 1;
 *     }
 * 
 *     cb_socket_t sock = cb_net.connect("example.com", 80, CB_NET_TCP);
 *     if (sock == -1) {
 *         fprintf(stderr, "Failed to connect\n");
 *         cb_net.cleanup();
 *         return 1;
 *     }
 * 
 *     const char* request = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
 *     cb_net.send(sock, request, strlen(request));
 * 
 *     char buffer[1024];
 *     int received = cb_net.receive(sock, buffer, sizeof(buffer) - 1);
 *     if (received > 0) {
 *         buffer[received] = '\0';
 *         printf("Received:\n%s\n", buffer);
 *     }
 * 
 *     cb_net.close(sock);
 *     cb_net.cleanup();
 *     return 0;
 * }
 * @endcode
 */
LIBCTOOL_API extern const struct cb_net_namespace cb_net;

#endif