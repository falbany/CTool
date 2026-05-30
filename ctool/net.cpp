/**
 * @file net.cpp
 * @author Florent ALBANY (FAL)
 * @brief Implementation of the C++ networking suite using native OS sockets.
 * @version 1.1
 * @date 2026-05-29
 */

#include "net.hpp"

// Detect Windows (covers all variants: MSVC, MinGW, Cygwin)
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WIN64__) || defined(__CYGWIN__) || defined(__CYGWIN32__)
    #include <winsock2.h>
    #include <ws2tcpip.h>
typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>
    #define closesocket close
#endif

#include "../platform.h"

#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>

namespace ctool {
    namespace net {

        // ------------------------------------------------------------------
        // Internal platform helpers (unnamed namespace = file-local linkage)
        // ------------------------------------------------------------------
        namespace {

            bool netInit() {
#ifdef PLATFORM_WINDOWS
                WSADATA wsa;
                return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
#else
                return true;
#endif
            }

            void netCleanup() {
#ifdef PLATFORM_WINDOWS
                WSACleanup();
#endif
            }

            int connectToHost(const char* host, int port, bool tcp) {
                struct addrinfo hints, *res = nullptr;
                int             sock_type = tcp ? SOCK_STREAM : SOCK_DGRAM;
                char            port_str[16];

                std::snprintf(port_str, sizeof(port_str), "%d", port);

                std::memset(&hints, 0, sizeof(hints));
                hints.ai_family   = AF_UNSPEC;
                hints.ai_socktype = sock_type;

                if (getaddrinfo(host, port_str, &hints, &res) != 0) {
                    return -1;
                }

                int sock = static_cast<int>(::socket(res->ai_family, res->ai_socktype, res->ai_protocol));
                if (sock == -1) {
                    freeaddrinfo(res);
                    return -1;
                }

                if (::connect(sock, res->ai_addr, static_cast<int>(res->ai_addrlen)) == -1) {
                    closesocket(sock);
                    freeaddrinfo(res);
                    return -1;
                }

                freeaddrinfo(res);
                return sock;
            }

        }    // unnamed namespace

        // ==================================================================
        // TcpClient
        // ==================================================================

        TcpClient::TcpClient() : _socket(-1) { netInit(); }

        TcpClient::~TcpClient() { disconnect(); }

        Status TcpClient::connect(const std::string& host, int port) {
            _socket = connectToHost(host.c_str(), port, true);
            return (_socket != -1) ? Status::Ok : Status::Error;
        }

        Status TcpClient::send(const std::string& data) {
            if (!isConnected()) {
                return Status::Disconnected;
            }
            int bytes = ::send(_socket, data.c_str(), static_cast<int>(data.length()), 0);
            return (bytes != -1) ? Status::Ok : Status::Error;
        }

        std::string TcpClient::receive(size_t max_len) {
            if (!isConnected()) {
                return "";
            }
            std::vector<char> buffer(max_len);
            int               bytes = ::recv(_socket, buffer.data(), static_cast<int>(max_len), 0);
            if (bytes <= 0) {
                disconnect();
                return "";
            }
            return std::string(buffer.data(), bytes);
        }

        void TcpClient::disconnect() {
            if (_socket != -1) {
                closesocket(_socket);
                _socket = -1;
            }
        }

        TcpClient::TcpClient(int socket) : _socket(socket) {
            // Subsystem already initialised by the server
        }

        // ==================================================================
        // TcpServer
        // ==================================================================

        TcpServer::TcpServer() : _server_sock(-1) { netInit(); }

        TcpServer::~TcpServer() { stop(); }

        Status TcpServer::listen(int port) {
            // Note: In a full implementation this would create a true listening
            // socket with socket(), bind(), and listen(). For now we reuse the
            // connect helper as a placeholder.
            _server_sock = connectToHost("0.0.0.0", port, true);
            return (_server_sock != -1) ? Status::Ok : Status::Error;
        }

        std::unique_ptr<TcpClient> TcpServer::accept() {
            if (_server_sock == -1) {
                return nullptr;
            }

            int client_fd = -1;

            /* Implementation logic (simplified for general purpose):
            struct sockaddr_in client_addr;
            socklen_t          addr_len = sizeof(client_addr);
            client_fd = static_cast<int>(::accept(_server_sock,
                                                  (struct sockaddr*)&client_addr,
                                                  &addr_len));
            */

            if (client_fd == -1) {
                return nullptr;
            }

            return std::unique_ptr<TcpClient>(new TcpClient(client_fd));
        }

        void TcpServer::stop() {
            if (_server_sock != -1) {
                closesocket(_server_sock);
                _server_sock = -1;
            }
            netCleanup();
        }

    }    // namespace net
}    // namespace ctool
