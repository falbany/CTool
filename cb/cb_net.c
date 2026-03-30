/**
 * @file cb_net.c
 * @author Florent ALBANY (FAL)
 * @brief Implementation of the cross-platform networking utilities.
 *
 * This file contains the platform-specific implementations of the networking
 * functions declared in `cb_net.h`. It uses Winsock for Windows and standard
 * POSIX sockets for other platforms.
 * 
 * @version 1.0
 * @date 2026-03-20
 */

#include "cb_net.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket close
#endif

/**
 * @brief Platform-specific implementation for initializing networking.
 * @see cb_net_namespace::init
 */
static bool impl_init(void) {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
#else
    return true;    // Not required on POSIX
#endif
}

/**
 * @brief Platform-specific implementation for cleaning up networking.
 * @see cb_net_namespace::cleanup
 */
static void impl_cleanup(void) {
#ifdef _WIN32
    WSACleanup();
#endif
}

/**
 * @brief Platform-specific implementation for connecting to a server.
 * @see cb_net_namespace::connect
 */
static cb_socket_t impl_connect(const char* host, int port, cb_net_proto_t proto) {
    struct addrinfo hints, *res;
    int             sock_type = (proto == CB_NET_TCP) ? SOCK_STREAM : SOCK_DGRAM;
    char            port_str[16];

    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;    // AF_INET or AF_INET6 to force version
    hints.ai_socktype = sock_type;

    if (getaddrinfo(host, port_str, &hints, &res) != 0) {
        return INVALID_SOCKET;
    }

    cb_socket_t sock = (cb_socket_t)socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }

    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        closesocket(sock);
        freeaddrinfo(res);
        return INVALID_SOCKET;
    }

    freeaddrinfo(res);
    return sock;
}

/**
 * @brief Platform-specific implementation for sending data.
 * @see cb_net_namespace::send
 */
static int impl_send(cb_socket_t sock, const char* data, size_t len) { return send(sock, data, (int)len, 0); }

/**
 * @brief Platform-specific implementation for receiving data.
 * @see cb_net_namespace::receive
 */
static int impl_receive(cb_socket_t sock, char* buffer, size_t max_len) { return recv(sock, buffer, (int)max_len, 0); }

/**
 * @brief Platform-specific implementation for closing a socket.
 * @see cb_net_namespace::close
 */
static void impl_close(cb_socket_t sock) { closesocket(sock); }

/**
 * @brief The global instance of the networking function table.
 */
const struct cb_net_namespace cb_net =
    {.init = impl_init, .cleanup = impl_cleanup, .connect = impl_connect, .send = impl_send, .receive = impl_receive, .close = impl_close};