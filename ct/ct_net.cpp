#include "ct_net.hpp"
#include "../CBridge.h"
#include <stdexcept>
#include <memory>

namespace ct {
    namespace net {

        // --- TcpClient Implementation ---

        TcpClient::TcpClient() : _socket(-1) { cb_net.init(); }

        TcpClient::~TcpClient() { disconnect(); }

        Status TcpClient::connect(const std::string& host, int port) {
            _socket = cb_net.connect(host.c_str(), port, CB_NET_TCP);
            return (_socket != -1) ? Status::Ok : Status::Error;
        }

        Status TcpClient::send(const std::string& data) {
            if (!isConnected()) return Status::Disconnected;
            int bytes = cb_net.send(_socket, data.c_str(), data.length());
            return (bytes != -1) ? Status::Ok : Status::Error;
        }

        std::string TcpClient::receive(size_t max_len) {
            if (!isConnected()) return "";
            std::vector<char> buffer(max_len);
            int               bytes = cb_net.receive(_socket, buffer.data(), max_len);
            if (bytes <= 0) {
                disconnect();
                return "";
            }
            return std::string(buffer.data(), bytes);
        }

        void TcpClient::disconnect() {
            if (_socket != -1) {
                cb_net.close(_socket);
                _socket = -1;
            }
        }

        TcpClient::TcpClient(int socket) : _socket(socket) {
            // Subsystem already initialized by the server
        }

        // --- TcpServer Implementation ---

        TcpServer::TcpServer() : _server_sock(-1) { cb_net.init(); }

        TcpServer::~TcpServer() { stop(); }

        Status TcpServer::listen(int port) {
            // Note: In a full implementation, cb_net.listen would handle
            // socket(), bind(), and listen() system calls.
            // For now, we assume cb_net provides a listener socket.
            _server_sock = cb_net.connect("0.0.0.0", port, CB_NET_TCP);
            return (_server_sock != -1) ? Status::Ok : Status::Error;
        }

        std::unique_ptr<TcpClient> TcpServer::accept() {
            if (_server_sock == -1) return nullptr;

            // We assume cb_net.receive or a dedicated cb_net.accept
            // returns the new client socket ID.
            // Here we simulate the blocking accept call:
            int client_fd = -1;

            /* Implementation logic (Simplified for general purpose):
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            client_fd = ::accept(_server_sock, (struct sockaddr*)&client_addr, &addr_len);
            */

            if (client_fd == -1) return nullptr;

            // Return a managed pointer to a new TcpClient wrapping the new socket
            return std::unique_ptr<TcpClient>(new TcpClient(client_fd));
        }

        void TcpServer::stop() {
            if (_server_sock != -1) {
                cb_net.close(_server_sock);
                _server_sock = -1;
            }
            cb_net.cleanup();
        }
    }    // namespace net
}    // namespace ct